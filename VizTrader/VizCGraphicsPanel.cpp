//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCGraphicsPanel.h"
#include "VizCSecurityWidget.h"
#include "VizMath.h"
#include "VizMimeFormat.h"
#include "VizIChart.h"
#include "VizCApplicationSettings.h"
#include "VizGUIUtils.h"
#include "VizCUndoView.h"
#include "VizIIndicator.h"
#include "VizCDataInspector.h"
#include <QPainter>
#include <QToolTip>
#include <QHBoxLayout>
#include <QApplication>
#include <QMetaProperty>
#include <QLabel>
#include <QDebug>
#include <QMovie>
#include <QMimeData>
#include <QDrag>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
const s32 SelectToleranceInPixel = 5;
const s32 ControlPointSize = 7;
VizCGraphicsPanel* VizCGraphicsPanel::ZoomBoxCurrentGraphicsPanel = 0;
VizCGraphicsPanel* VizCGraphicsPanel::CrossHairsCurrentGraphicsPanel = 0;
//-----------------------------------------------------------------------------
VizCGraphicsPanel::VizCGraphicsPanel(VizCSecurityWidget* parent, bool isClosable, VizCWorkArea* workArea) : VizIInnerPanel(parent, workArea), FakeItem(0), BlockCreateUndoCommandPropertyChanged(false), LoadingState(false)
{
	setAttribute(Qt::WA_DeleteOnClose);
	setAcceptDrops(false);
	setFocusPolicy(Qt::StrongFocus);

	QHBoxLayout* layoutH = new QHBoxLayout;

	TitleBar = new VizCInnerPanelTitleBar(this, isClosable);
	connect(TitleBar, SIGNAL(InnerPanelMaximizeButtonClick()), this, SLOT(slotMaximizeInnerPanelClick()));
	connect(TitleBar, SIGNAL(InnerPanelCloseButtonClick()), this, SLOT(slotCloseInnerPanelClick()));

	VerticalScale = new VizCVerticalScale(this, WorkArea);
	HorizontalScale = Parent->horizontalScale();
	Dummy = new QWidget(this);
	VerticalScale->stackUnder(TitleBar);
	Dummy->stackUnder(TitleBar);

	QObject::connect(VerticalScale, SIGNAL(scaleChange(bool)), this, SLOT(slotItemChanged(bool)));

	layoutH->addWidget(Dummy);
	layoutH->addWidget(VerticalScale);
	layoutH->setMargin(0);
	layoutH->setSpacing(0);

	setLayout(layoutH);
	
	setProperty("name", tr("Painel Interno"));

	MultipleSelectMenu = new QMenu(this);
	connect(MultipleSelectMenu, SIGNAL(triggered(QAction*)), this, SLOT(slotMultipleSelectMenu(QAction*)));

	setMouseTracking(true);
	Dummy->setMouseTracking(true);
	TitleBar->setMouseTracking(true);
	VerticalScale->setMouseTracking(true);

	LoadingAnimation = new VizCLoadingAnimation(this);
	LoadingAnimation->stopAnimation();

	resetToDefaults();

	connect(VizCApplicationSettings::instance(), SIGNAL(settingsChange()), this, SLOT(slotAplicationSettingsChange()));

	QObject::connect(this, SIGNAL(createUndoCommandAddItem(VizIGraphicsItem*, VizIGraphicsItem*)), this, SLOT(slotCreateUndoCommandAddItem(VizIGraphicsItem*, VizIGraphicsItem*)));
	QObject::connect(this, SIGNAL(createUndoCommandRemoveItem(VizIGraphicsItem*, VizIGraphicsItem*)), this, SLOT(slotCreateUndoCommandRemoveItem(VizIGraphicsItem*, VizIGraphicsItem*)));

	QObject::connect(this, SIGNAL(createUndoCommandPropertyChanged(QObject*, const QString&, const QVariant&, const QVariant&, bool)), this, SLOT(slotCreateUndoCommandPropertyChanged(QObject*, const QString&, const QVariant&, const QVariant&, bool)));
	QObject::connect(this, SIGNAL(createUndoCommandPropertiesChanged(QObject*, const QStringList&, const QVariantList&, const QVariantList&, bool, const QString&)), this, SLOT(slotCreateUndoCommandPropertiesChanged(QObject*, const QStringList&, const QVariantList&, const QVariantList&, bool, const QString&)));

	QObject::connect(VerticalScale, SIGNAL(createUndoCommandPropertyChanged(QObject*, const QString&, const QVariant&, const QVariant&, bool)), this, SLOT(slotCreateUndoCommandPropertyChanged(QObject*, const QString&, const QVariant&, const QVariant&, bool)));
	QObject::connect(VerticalScale, SIGNAL(createUndoCommandPropertiesChanged(QObject*, const QStringList&, const QVariantList&, const QVariantList&, bool, const QString&)), this, SLOT(slotCreateUndoCommandPropertiesChanged(QObject*, const QStringList&, const QVariantList&, const QVariantList&, bool, const QString&)));

	QObject::connect(HorizontalScale, SIGNAL(createUndoCommandPropertyChanged(QObject*, const QString&, const QVariant&, const QVariant&, bool)), this, SLOT(slotCreateUndoCommandPropertyChanged(QObject*, const QString&, const QVariant&, const QVariant&, bool)));
	QObject::connect(HorizontalScale, SIGNAL(createUndoCommandPropertiesChanged(QObject*, const QStringList&, const QVariantList&, const QVariantList&, bool, const QString&)), this, SLOT(slotCreateUndoCommandPropertiesChanged(QObject*, const QStringList&, const QVariantList&, const QVariantList&, bool, const QString&)));
}
//-----------------------------------------------------------------------------
VizCGraphicsPanel::~VizCGraphicsPanel()
{
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::blockCreateUndoCommandPropertyChanged(bool block)
{
	BlockCreateUndoCommandPropertyChanged = block;
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::setLoadingState(bool value)
{
	LoadingState = value;
	if(value)
	{
		LoadingAnimation->startAnimation();
	}
	else
	{
		LoadingAnimation->stopAnimation();
	}
}
//-----------------------------------------------------------------------------
bool VizCGraphicsPanel::isLoadingState() const
{
	return LoadingState;
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::setPercentageLoaded(int percent)
{
	LoadingAnimation->setPercentageLoaded(percent);
	//repaint();

}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::focusInEvent(QFocusEvent* event)
{
	UndoManager->setActiveStack(UndoStack);
	WorkArea->undoView()->setStack(UndoStack);


	VizIInnerPanel::focusInEvent(event);
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::resetToDefaults()
{

	QStringList names;
	QVariantList oldValues;
	QVariantList newValues;

	gui::vizObjectGetAllPropertiesNamesAndValues(this, names, oldValues);

	VizIInnerPanel::resetToDefaults();
	//BackGroundColor = Qt::white;
	BackGroundColor = QColor(Settings->value("Appearance/DefaultFontAndColor/BackgroundColor").toString());
	ShowValuesInTheTitleBar = true;
	UseColorOfPlotInValuesOfTitleBar = true;
	slotAplicationSettingsChange();

	gui::vizObjectGetAllPropertiesNamesAndValues(this, names, newValues);

	emit createUndoCommandPropertiesChanged(this, names, oldValues, newValues, true, tr("Valores Padrão"));

	repaint();
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::slotAplicationSettingsChange()
{
	Antialiasing = Settings->value("Performance/Render/AntiAliasing").toBool();

	UseGradientToFillShapes = Settings->value("Performance/Render/UseGradientToFillShapes").toBool();
	foreach(VizIGraphicsItem* item, Items)
	{
		item->setFillRectWithGradient(UseGradientToFillShapes);
	}

	//repaint();
}
//-----------------------------------------------------------------------------
bool VizCGraphicsPanel::showValuesInTheTitleBar() const
{
	return ShowValuesInTheTitleBar;
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::setShowValuesInTheTitleBar(bool value)
{
	if(ShowValuesInTheTitleBar != value)
	{
		bool oldValue = ShowValuesInTheTitleBar;
		ShowValuesInTheTitleBar = value;

		repaint();

		emit createUndoCommandPropertyChanged(this, "showValuesInTheTitleBar", oldValue, ShowValuesInTheTitleBar, false);
	}
}
//-----------------------------------------------------------------------------
bool VizCGraphicsPanel::useColorOfPlotInValuesOfTitleBar() const
{
	return UseColorOfPlotInValuesOfTitleBar;
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::setUseColorOfPlotInValuesOfTitleBar(bool value)
{
	if(UseColorOfPlotInValuesOfTitleBar != value)
	{
		bool oldValue = UseColorOfPlotInValuesOfTitleBar;
		UseColorOfPlotInValuesOfTitleBar = value;

		repaint();

		emit createUndoCommandPropertyChanged(this, "useColorOfPlotInValuesOfTitleBar", oldValue, UseColorOfPlotInValuesOfTitleBar, false);
	}
}
//-----------------------------------------------------------------------------
VizCVerticalScale* VizCGraphicsPanel::verticalScale()
{
	return VerticalScale;
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::setBackGroundColor(const QColor& color)
{
	if(color != BackGroundColor)
	{
		QColor oldValue = BackGroundColor;

		BackGroundColor = color;
		repaint();

		emit createUndoCommandPropertyChanged(this, "backGroundColor", oldValue, BackGroundColor, false);
	}
}
//-----------------------------------------------------------------------------
QColor VizCGraphicsPanel::backGroundColor() const
{
	return BackGroundColor;
}
//-----------------------------------------------------------------------------
VizIGraphicsItemBasedOnSecurity* VizCGraphicsPanel::privateNewGraphicsItemBasedOnSecurity(const QString& name, const QString& securitySymbol, viz::VizEScaleInYOptions scaleInYOptions, bool isRemovable)
{
	VizIGraphicsItemBasedOnSecurity* item = 0;
	int id = QMetaType::type(qPrintable(name));
	if(id == -1)
	{
		Q_ASSERT(false);
		return 0;
	}

	void* myClassPtr = QMetaType::create(id);
	Q_ASSERT(myClassPtr);
	item = static_cast<VizIGraphicsItemBasedOnSecurity*>(myClassPtr);

	item->setSecuritySymbol(securitySymbol);
	item->setIsRemovable(isRemovable);
	item->setScaleInYOptions(scaleInYOptions);
	item->setFillRectWithGradient(UseGradientToFillShapes);

	return item;
}
//-----------------------------------------------------------------------------
VizIGraphicsItemBasedOnShape* VizCGraphicsPanel::privateNewVizIGraphicsItemBasedOnShape(const QString& name, viz::VizEScaleInYOptions scaleInYOptions, bool isRemovable)
{
	VizIGraphicsItemBasedOnShape* item = 0;
	int id = QMetaType::type(qPrintable(name));
	if(id == -1)
	{
		Q_ASSERT(false);
		return 0;
	}

	void* myClassPtr = QMetaType::create(id);
	Q_ASSERT(myClassPtr);
	item = static_cast<VizIGraphicsItemBasedOnShape*>(myClassPtr);
	item->setIsRemovable(isRemovable);
	item->setScaleInYOptions(scaleInYOptions);
	item->setFillRectWithGradient(UseGradientToFillShapes);


	return item;
}
//-----------------------------------------------------------------------------
VizIGraphicsItemBasedOnSecurity* VizCGraphicsPanel::newGraphicsItemBasedOnSecurity(const QString& name, const QString& securitySymbol, viz::VizEScaleInYOptions scaleInYOptions, bool isRemovable)
{
	bool isEmpty = Items.empty();

	VizIGraphicsItemBasedOnSecurity* item = privateNewGraphicsItemBasedOnSecurity(name, securitySymbol, scaleInYOptions, isRemovable);

	item->setWorkArea(WorkArea);
	item->setHorizontalScale(HorizontalScale);
	item->setVerticalScale(VerticalScale);
	item->updateData();
	Items.append(item);

	connectItemFromChangedEvent(item);
	Parent->updateHorizontalScale();
	updateVerticalScale();
	
	
	if(WorkArea->selectedGraphicsPanel() == this)
	{
		WorkArea->updateScene();
	}

	if(!isEmpty)
	{
		emit createUndoCommandAddItem(item, 0);
	}

	
	VizIIndicator* indicator = qobject_cast<VizIIndicator*>(item);
	if(indicator)
	{
		indicator->blockSignals(true);
		indicator->addRelativeItems(this);
		indicator->blockSignals(false);
	}

	repaint();

	return item;
}
//-----------------------------------------------------------------------------
VizIGraphicsItemBasedOnShape* VizCGraphicsPanel::newVizIGraphicsItemBasedOnShape(const QString& name, viz::VizEScaleInYOptions scaleInYOptions, bool isRemovable)
{
	bool isEmpty = Items.empty();

	VizIGraphicsItemBasedOnShape* item = privateNewVizIGraphicsItemBasedOnShape(name, scaleInYOptions, isRemovable);
	
	item->setWorkArea(WorkArea);
	item->setHorizontalScale(HorizontalScale);
	item->setVerticalScale(VerticalScale);
	Items.append(item);

	connectItemFromChangedEvent(item);
	if(WorkArea->selectedGraphicsPanel() == this)
	{
		WorkArea->updateScene();
	}

	if(!isEmpty)
	{
		emit createUndoCommandAddItem(item, 0);
	}

	repaint();

	return item;
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::connectItemFromChangedEvent(VizIGraphicsItem* item)
{
	QObject::connect(item, SIGNAL(itemChanged(bool)), this, SLOT(slotItemChanged(bool)));
	QObject::connect(item, SIGNAL(createUndoCommandPropertyChanged(QObject*, const QString&, const QVariant&, const QVariant&, bool)), this, SLOT(slotCreateUndoCommandPropertyChanged(QObject*, const QString&, const QVariant&, const QVariant&, bool)));
	QObject::connect(item, SIGNAL(createUndoCommandPropertiesChanged(QObject*, const QStringList&, const QVariantList&, const QVariantList&, bool, const QString&)), this, SLOT(slotCreateUndoCommandPropertiesChanged(QObject*, const QStringList&, const QVariantList&, const QVariantList&, bool, const QString&)));
	QObject::connect(item, SIGNAL(createUndoCommandParentChanged(VizIGraphicsItem*, VizIGraphicsItem*, VizIGraphicsItem*)), this, SLOT(slotCreateUndoCommandParentChanged(VizIGraphicsItem*, VizIGraphicsItem*, VizIGraphicsItem*)));
	
	//item->blockSignals(false);
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::disconnectItemFromChangedEvent(VizIGraphicsItem* item)
{
	
	QObject::disconnect(item, SIGNAL(itemChanged(bool)), this, SLOT(slotItemChanged(bool)));
	QObject::disconnect(item, SIGNAL(createUndoCommandPropertyChanged(QObject*, const QString&, const QVariant&, const QVariant&, bool)), this, SLOT(slotCreateUndoCommandPropertyChanged(QObject*, const QString&, const QVariant&, const QVariant&, bool)));
	QObject::disconnect(item, SIGNAL(createUndoCommandPropertiesChanged(QObject*, const QStringList&, const QVariantList&, const QVariantList&, bool, const QString&)), this, SLOT(slotCreateUndoCommandPropertiesChanged(QObject*, const QStringList&, const QVariantList&, const QVariantList&, bool, const QString&)));
	QObject::disconnect(item, SIGNAL(createUndoCommandParentChanged(VizIGraphicsItem*, VizIGraphicsItem*, VizIGraphicsItem*)), this, SLOT(slotCreateUndoCommandParentChanged(VizIGraphicsItem*, VizIGraphicsItem*, VizIGraphicsItem*)));
	
	//item->blockSignals(true);
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::connectItemsFromChangedEvent()
{
	foreach(VizIGraphicsItem* item, Items)
	{
		connectItemFromChangedEvent(item);
	}
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::disconnectItemsFromChangedEvent()
{
	foreach(VizIGraphicsItem* item, Items)
	{
		disconnectItemFromChangedEvent(item);
	}
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::slotItemChanged(bool needUpdateVerticalScale)
{
	if(needUpdateVerticalScale)
	{
		updateVerticalScale();
	}
	repaint();
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::slotCreateUndoCommandPropertyChanged(QObject* item, const QString& propertyName, const QVariant& oldValue, const QVariant& newValue, bool needUpdateVerticalScale)
{
	if(!BlockCreateUndoCommandPropertyChanged)
	{
		Q_ASSERT(item);
		VizCObjectChangePropertyCommand* command = new VizCObjectChangePropertyCommand(item, propertyName, oldValue, newValue, needUpdateVerticalScale, WorkArea);
		QObject::connect(command, SIGNAL(itemChanged(bool)), this, SLOT(slotItemChanged(bool)));
		UndoStack->push(command);
	}

}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::slotCreateUndoCommandPropertiesChanged(QObject* item, const QStringList& propertiesNames, const QVariantList& oldValues, const QVariantList& newValues, bool needUpdateVerticalScale, const QString& commandAliasName)
{
	if(!BlockCreateUndoCommandPropertyChanged)
	{
		Q_ASSERT(item);
		VizCObjectChangePropertyCommand* command = new VizCObjectChangePropertyCommand(item, propertiesNames, oldValues, newValues, needUpdateVerticalScale, commandAliasName, WorkArea);
		QObject::connect(command, SIGNAL(itemChanged(bool)), this, SLOT(slotItemChanged(bool)));
		UndoStack->push(command);
	}
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::slotCreateUndoCommandParentChanged(VizIGraphicsItem* item, VizIGraphicsItem* oldParent, VizIGraphicsItem* newParent)
{
	Q_ASSERT(item);
	VizCGraphicsItemChangeParentCommand* command = new VizCGraphicsItemChangeParentCommand(item, oldParent, newParent, WorkArea);
	QObject::connect(command, SIGNAL(itemChanged(bool)), this, SLOT(slotItemChanged(bool)));
	UndoStack->push(command);
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::slotCreateUndoCommandAddItem(VizIGraphicsItem* item, VizIGraphicsItem* parent)
{
	Q_ASSERT(item);
	UndoStack->push(new VizCGraphicsItemAddCommand(item, parent, this, WorkArea));

}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::slotCreateUndoCommandRemoveItem(VizIGraphicsItem* item, VizIGraphicsItem* parent)
{
	Q_ASSERT(item);
	UndoStack->push(new VizCGraphicsItemRemoveCommand(item, parent, this, WorkArea));
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::removeFakeItem()
{
	if(FakeItem)
	{
		FakeItem->setParentItem(0);
		//delete FakeItem;
		FakeItem->deleteLater();
		FakeItem = 0;
	}
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::cancelAddItem()
{
	removeFakeItem();
	repaint();
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::createFakeItem()
{
	removeFakeItem();

	QString name = ToolManager->addingItemName();
	int id = QMetaType::type(qPrintable(name));
	Q_ASSERT(id != -1);

	void* myClassPtr = QMetaType::create(id);
	Q_ASSERT(myClassPtr);
	FakeItem = static_cast<VizIGraphicsItemBasedOnShape*>(myClassPtr);

	FakeItem->setWorkArea(WorkArea);
	FakeItem->setHorizontalScale(HorizontalScale);
	FakeItem->setVerticalScale(VerticalScale);
	FakeItem->setIsRemovable(true);
	FakeItem->setScaleInYOptions(VESIYO_DEFAULT);


	/*VizIGraphicsItemBasedOnSecurity* parent = 0;
	if(Settings->value("General/Edit/AddItemAsAChildOfTheNearestItem").toBool())
	{
		parent = findNearestParentItem(MousePressPositionMappedInWindowDiscrete);
	}
	FakeItem->setParentItem(parent);
	FakeItem->setSnapToPrice(Settings->value("General/Edit/SnapToPriceAfterAddedItem").toBool());
	*/

	WorkArea->setSelectedVizOBject(0);
	if(WorkArea->selectedGraphicsPanel() == this)
	{
		WorkArea->updateScene();
	}

}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::addItemBasedOnFakeItem()
{

	bool isEmpty = Items.empty();

	//FakeItem->setParentItem(0);
	VizIGraphicsItemBasedOnSecurity* parent = 0;
	if(Settings->value("General/Edit/AddItemAsAChildOfTheNearestItem").toBool())
	{
		parent = findNearestParentItem(MousePressPositionMappedInWindowDiscrete);
	}
	
	VizIGraphicsItemBasedOnShape* item = 0;

	QString name = ToolManager->addingItemName();
	item = privateNewVizIGraphicsItemBasedOnShape(name, VESIYO_DEFAULT, true);

	item->setWorkArea(WorkArea);
	item->setHorizontalScale(HorizontalScale);
	item->setVerticalScale(VerticalScale);
	Items.append(item);

	//item->beginEdit();
	item->edit(CurrentShapeTransform);
	item->endEdit(CurrentShapeTransform);
	item->setParentItem(parent);
	item->setSnapToPrice(Settings->value("General/Edit/SnapToPriceAfterAddedItem").toBool());
	
	connectItemFromChangedEvent(item);
	
	removeFakeItem();

	if(Settings->value("General/Edit/ReturnToSelectModeAfterAddedItem").toBool())
	{
		ToolManager->selectTool(VETS_SELECTION);
	}
	WorkArea->setSelectedVizOBject(item);
	if(WorkArea->selectedGraphicsPanel() == this)
	{
		WorkArea->updateScene();
	}
	
	if(!isEmpty)
	{
		emit createUndoCommandAddItem(item, item->parentItem());
	}
}
//-----------------------------------------------------------------------------
VizIGraphicsItemBasedOnSecurity* VizCGraphicsPanel::findNearestParentItem(const QPointF& mousepos)
{
	VizIGraphicsItemBasedOnSecurity* parent = 0;
	f64 y = std::numeric_limits<f64>::max();

	foreach(VizIGraphicsItem* item, Items)
	{
		VizIGraphicsItemBasedOnSecurity* itemBasedOnSecurity = qobject_cast<VizIGraphicsItemBasedOnSecurity*>(item);
		if(itemBasedOnSecurity)
		{
			f64 nearest;
			bool isValid;
			itemBasedOnSecurity->nearestYValue(mousepos, nearest, isValid);
			if(!isValid)
				continue;
			f64 dist_y = core::fabs(mousepos.y() - nearest);
			
			if(dist_y < y)
			{
				parent = itemBasedOnSecurity;
				y = dist_y;
			}
		}		
	}

	return parent;
}
//-----------------------------------------------------------------------------
const QList<VizIGraphicsItem*>& VizCGraphicsPanel::items() const
{
	return Items;
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::addItem(VizIGraphicsItem* item, bool isRemovable)
{
	
	Q_ASSERT(!(item->parentItem() && !containItem(item->parentItem())));

	recursiveAddItem(item);
	item->setIsRemovable(isRemovable);

	Parent->updateData();
	Parent->updateHorizontalScale();
	updateVerticalScale();

	if(WorkArea->selectedGraphicsPanel() == this)
	{
		WorkArea->updateScene();
	}
	if(WorkArea->selectedVizOBject() == qobject_cast<QObject*>(item))
	{
		WorkArea->setSelectedVizOBject(0);
	}

	emit createUndoCommandAddItem(item, item->parentItem());

	repaint();
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::recursiveAddItem(VizIGraphicsItem* item)
{
	bool endRecursion = false;
	if(!item->hasChilds())
	{
		endRecursion = true;
	}

	item->setWorkArea(WorkArea);
	item->setHorizontalScale(HorizontalScale);
	item->setVerticalScale(VerticalScale);
	item->setIsRemovable(true);
	Items.append(item);
	
	connectItemFromChangedEvent(item);

	if(!endRecursion)
	{
		foreach(VizIGraphicsItem* children, item->childItems())
		{
			recursiveAddItem(children);
		}
	}
}
//-----------------------------------------------------------------------------
bool VizCGraphicsPanel::isPossibleRemoveItem(VizIGraphicsItem* item)
{
	bool ok = true;

	recursiveIsPossibleRemoveItem(item, ok);

	return ok;
}
//-----------------------------------------------------------------------------
bool VizCGraphicsPanel::isPossibleAddItem(VizIGraphicsItem* item)
{
	bool ok = true;

	VizIChart* isChart = qobject_cast<VizIChart*>(item);
	if(isChart)
	{
		foreach(VizIGraphicsItem* i, Items)
		{
			VizIChart* chart = qobject_cast<VizIChart*>(i);
			if(chart)
			{
				ok = chart->securitySymbol() != isChart->securitySymbol();
				if(!ok) break;
			}
		}
	}

	return ok;
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::recursiveIsPossibleRemoveItem(VizIGraphicsItem* item, bool& ok)
{
	bool endRecursion = false;
	if(!item->hasChilds())
	{
		endRecursion = true;
	}

	if(!item->isRemovable())
	{
		ok = false;
		return;
	}

	if(!endRecursion)
	{
		foreach(VizIGraphicsItem* children, item->childItems())
		{
			recursiveIsPossibleRemoveItem(children, ok);
		}
	}
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::removeItem(VizIGraphicsItem* item)
{
	VizIGraphicsItem* oldParent = item->parentItem();


	recursiveRemoveItem(item);
	item->blockSignals(true);
	item->setParentItem(0);
	item->blockSignals(false);

	Parent->updateData();
	Parent->updateHorizontalScale();
	updateVerticalScale();


	if(WorkArea->selectedGraphicsPanel() == this)
	{
		WorkArea->updateScene();
	}
	if(WorkArea->selectedVizOBject() == qobject_cast<QObject*>(item))
	{
		WorkArea->setSelectedVizOBject(0);
	}

	emit createUndoCommandRemoveItem(item, oldParent);

	repaint();

}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::recursiveRemoveItem(VizIGraphicsItem* item)
{
	bool endRecursion = false;
	if(!item->hasChilds())
	{
		endRecursion = true;
	}

	//if(item->isRemovable())
	{
		s32 index = itemIndex(item);
		if(index != -1)
		{
			disconnectItemFromChangedEvent(item);
			
			item->setHorizontalScale(0);
			item->setVerticalScale(0);
			Items.removeAt(index);

			//isso perde a hierarquia
			//item->setParentItem(0);
			//isso faz a remocao
			//item->deleteLater();
		}
	}

	if(!endRecursion)
	{
		foreach(VizIGraphicsItem* children, item->childItems())
		{
			recursiveRemoveItem(children);
		}
	}
}
//-----------------------------------------------------------------------------
VizIGraphicsItem* VizCGraphicsPanel::cloneItem(VizIGraphicsItem* item, bool preserveIsClosableProperty)
{
	disconnectItemsFromChangedEvent();
	VizIGraphicsItem* newitem = recursiveCloneItem(0, item, preserveIsClosableProperty);
	connectItemsFromChangedEvent();

	return newitem;
}
//-----------------------------------------------------------------------------
VizIGraphicsItem* VizCGraphicsPanel::recursiveCloneItem(VizIGraphicsItem* parent, VizIGraphicsItem* item, bool preserveIsClosableProperty)
{
	bool endRecursion = false;
	if(!item->hasChilds())
	{
		endRecursion = true;
	}

	VizIGraphicsItem* newitem;
	if(qobject_cast<VizIGraphicsItemBasedOnSecurity*>(item))
	{
		VizIGraphicsItemBasedOnSecurity* gi = qobject_cast<VizIGraphicsItemBasedOnSecurity*>(item);
		Q_ASSERT(gi);
		newitem = privateNewGraphicsItemBasedOnSecurity(gi->metaObject()->className(), gi->securitySymbol(), gi->scaleInYOptions(), preserveIsClosableProperty ? gi->isRemovable() : true);
		Q_ASSERT(newitem);
	}
	else
	if(qobject_cast<VizIGraphicsItemBasedOnShape*>(item))
	{
		VizIGraphicsItemBasedOnShape* gi = qobject_cast<VizIGraphicsItemBasedOnShape*>(item);
		Q_ASSERT(gi);
		newitem = privateNewVizIGraphicsItemBasedOnShape(gi->metaObject()->className(), gi->scaleInYOptions(), preserveIsClosableProperty ? gi->isRemovable() : true);
		Q_ASSERT(newitem);
	}
	else
	{
		Q_ASSERT(true);
	}

	newitem->setHorizontalScale(HorizontalScale);
	newitem->setVerticalScale(VerticalScale);

	for(s32 i = 0; i < item->metaObject()->propertyCount(); i++)
	{
		QMetaProperty p = item->metaObject()->property(i);
		if(p.isUser())
		{
			newitem->setProperty(p.name(), item->property(p.name()));
		}
	}

	if(!endRecursion)
	{
		foreach(VizIGraphicsItem* child, item->childItems())
		{
			recursiveCloneItem(newitem, child, preserveIsClosableProperty);
		}
	}

	newitem->setParentItem(parent);

	return newitem;
}
//-----------------------------------------------------------------------------
s32 VizCGraphicsPanel::itemIndex(VizIGraphicsItem* item)
{
	Q_ASSERT(item);
	for(s32 i = 0; i < Items.size(); i++)
	{
		if(Items[i] == item)
		{
			return i;
		}
	}
	return -1;
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::updateVerticalScale()
{
	f64 maxY = std::numeric_limits<f64>::min();
	f64 minY = std::numeric_limits<f64>::max();
	bool isEmpty = true;
	f64 LastTradeValue = -1;
	bool isValidLastTradeValue = false;
	VizIGraphicsItemBasedOnSecurity* overlayWithoutScaleItem = 0;
	if(VerticalScale->isAutoScale())
	{
		foreach(VizIGraphicsItem* item, Items)
		{
			VizIGraphicsItemBasedOnSecurity* itemBasedOnSecurity = qobject_cast<VizIGraphicsItemBasedOnSecurity*>(item);
			if(!itemBasedOnSecurity)
			{
				continue;
			}
			if(!itemBasedOnSecurity->isRemovable())
			{
				itemBasedOnSecurity->lastTrade(LastTradeValue, isValidLastTradeValue);
				if(!isValidLastTradeValue)
				{
					continue;
				}
			}
			
			if(itemBasedOnSecurity->scaleInYOptions() == viz::VESIYO_OVERLAY_WITHOUT_SCALE)
			{
				overlayWithoutScaleItem = itemBasedOnSecurity;
				continue;
			}
			f64 itemMaxY = std::numeric_limits<f64>::min();
			f64 itemMinY = std::numeric_limits<f64>::max();
			itemBasedOnSecurity->yRange(isEmpty, itemMinY, itemMaxY);
			maxY = qMax<f64>(maxY, itemMaxY);
			minY = qMin<f64>(minY, itemMinY);
		}

		if(isEmpty)
		{
			if(overlayWithoutScaleItem)
			{
				//somente o item com overlapwithoutscale
				overlayWithoutScaleItem->yRange(isEmpty, minY, maxY);
			}
			else
			{
				//maxY = VerticalScale->maximum() - VerticalScale->border();
				//minY = VerticalScale->minimum() + VerticalScale->border();
				maxY = VerticalScale->maximum();
				minY = VerticalScale->minimum();
			}
		}

		VerticalScale->blockSignals(true);
		
		VerticalScale->setRange(minY, maxY);
		VerticalScale->setLastTrade(LastTradeValue, isValidLastTradeValue);
		
		WorkArea->updateProperty(VerticalScale, "minimum");
		WorkArea->updateProperty(VerticalScale, "maximum");

		VerticalScale->blockSignals(false);

		//repaint();

	}
}
//-----------------------------------------------------------------------------
QList<VizIGraphicsItem*> VizCGraphicsPanel::itemsAt(const QPointF& pos)
{
	QSizeF size = mapToViewPort(QSize(SelectToleranceInPixel, SelectToleranceInPixel));
	QPointF p(pos.x() - (size.width() * 0.5), pos.y() - (size.height() * 0.5));
	QRectF rect(p, size);

	QList<VizIGraphicsItem*> List;
	foreach(VizIGraphicsItem* item, Items)
	{

		if(!item->isVisible())
			continue;

		VizIGraphicsItemBasedOnShape* itemBasedOnShape = qobject_cast<VizIGraphicsItemBasedOnShape*>(item);
		if(itemBasedOnShape)
		{
			if(controlPointsContains(itemBasedOnShape->controlPoints(), pos) != -1)
			{
				List.append(item);
				continue;
			}
		}		

		if(item->intersects(rect))
		{
			List.append(item);
		}
	}
	return List;
}
//-----------------------------------------------------------------------------
s32 VizCGraphicsPanel::controlPointsContains(const QList<QPointF>& points, const QPointF& point)
{
	QSizeF pointSize = mapToViewPort(QSize(SelectToleranceInPixel, SelectToleranceInPixel));
	QPointF pointPos(point.x() - (pointSize.width() * 0.5), point.y() - (pointSize.height() * 0.5));
	QRectF pointRect(pointPos, pointSize);
	QSizeF controlPointSize = mapToViewPort(QSize(ControlPointSize, ControlPointSize));
	for(s32 i = 0; i < points.size(); i++)
	{
		const QPointF& p = points[i];
		QPointF topleft(p.x() - (controlPointSize.width() * 0.5), p.y() - (controlPointSize.height() * 0.5));
		QRectF constrolPointRect(topleft, controlPointSize);
		if(pointRect.intersects(constrolPointRect))
		{
			return i;
		}
	}
	return -1;
}
//-----------------------------------------------------------------------------
bool VizCGraphicsPanel::containItem(const QString& name) const
{
	foreach(VizIGraphicsItem* item, Items)
	{
		if(vizObjectName(item) == name)
		{
			return true;
		}
	}
	return false;
}
//-----------------------------------------------------------------------------
bool VizCGraphicsPanel::containItem(const VizIGraphicsItem* item) const
{
	foreach(VizIGraphicsItem* i, Items)
	{
		if(i == item)
		{
			return true;
		}
	}
	return false;
}
//-----------------------------------------------------------------------------
VizIGraphicsItem* VizCGraphicsPanel::findSelectedItem(const QList<VizIGraphicsItem*>& list) const
{
	foreach(VizIGraphicsItem* item, list)
	{
		if(vizObjectIsSelected(item))
		{
			return item;
		}
	}
	return 0;
}
//-----------------------------------------------------------------------------
QPointF VizCGraphicsPanel::mapToViewPort(const QPoint& pos, bool discrete)
{
	QTransform transform = transformWindowToViewPort();

	f64 x = pos.x();
	f64 y = pos.y();
	f64 tx;
	f64 ty;
	transform.map(x, y, &tx, &ty);

	if(discrete)
	{
		s32 x_discret = static_cast<s32>(tx + gui::CellHalfWidth);
		tx = x_discret;
	}

	return QPointF(tx, ty);


}
//-----------------------------------------------------------------------------
QSizeF VizCGraphicsPanel::mapToViewPort(const QSize& size)
{

	QTransform transform = transformWindowToViewPort();

	QRectF r(0, 0, size.width(), size.height());
	QRectF t = transform.mapRect(r);
	QSizeF s(t.width(), t.height());

	return s;

}
//-----------------------------------------------------------------------------
QTransform VizCGraphicsPanel::transformViewPortToWindow() const
{
	QRectF viewport = gui::graphicsPanelViewPort(VerticalScale, HorizontalScale);
	QRect window = Dummy->geometry();

	f64 vx = static_cast<f64>(window.x());
	f64 vy = static_cast<f64>(window.y());
	f64 vw = static_cast<f64>(window.width());
	f64 vh = static_cast<f64>(window.height());

	f64 wx = viewport.left();
	f64 wy = viewport.top();
	f64 ww = core::fabs(viewport.right() - viewport.left());
	f64 wh = viewport.top() - viewport.bottom();

	f64 scaleW = vw / ww;
	f64 scaleH = -1.0 * (vh / wh);
	QTransform transform(scaleW, 0, 0, scaleH, vx - wx * scaleW, vy - wy * scaleH);

	return transform;
}
//-----------------------------------------------------------------------------
QTransform VizCGraphicsPanel::transformWindowToViewPort() const
{
	QRectF viewport = gui::graphicsPanelViewPort(VerticalScale, HorizontalScale);
	QRect window = Dummy->geometry();

	f64 vx = static_cast<f64>(window.x());
	f64 vy = static_cast<f64>(window.y());
	f64 vw = static_cast<f64>(window.width());
	f64 vh = static_cast<f64>(window.height());

	f64 wx = viewport.left();
	f64 wy = viewport.top();
	f64 ww = core::fabs(viewport.right() - viewport.left());
	f64 wh = viewport.top() - viewport.bottom();

	f64 scaleW = ww / vw;
	f64 scaleH = -1.0 * (wh / vh);
	QTransform transform(scaleW, 0, 0, scaleH, wx - vx * scaleW, wy - vy * scaleH);

	return transform;

}
//-----------------------------------------------------------------------------
bool VizCGraphicsPanel::event(QEvent* event)
{
	if(event->type() == QEvent::ToolTip)
	{
		QHelpEvent* helpEvent = static_cast<QHelpEvent *>(event);
		MousePosition = helpEvent->pos();
		MousePositionMappedInWindowDiscrete = mapToViewPort(MousePosition, true);
		MousePositionMappedInWindowContinuum = mapToViewPort(MousePosition, false);

		if(Dummy->geometry().contains(MousePosition))
		{
			QList<VizIGraphicsItem*> items = itemsAt(MousePositionMappedInWindowContinuum);
			if(!items.empty())
			{
				VizIGraphicsItem* item = findSelectedItem(items);
				if(!item)
				{
					item = items.first();
				}
				QToolTip::showText(helpEvent->globalPos(), item->toolTip(MousePositionMappedInWindowDiscrete));
			}
			else
			{
				if(TitleBarVisible)
				{
					s32 h = TitleBar->titleBarHeight();
					if(MousePosition.y() <= h)
					{
						if(!TitleBar->buttonsContains(MousePosition))
						{
							QToolTip::showText(helpEvent->globalPos(), titleBarName());
						}
					}
				}
				else
				{
					QToolTip::showText(helpEvent->globalPos(), "");
				}
			}
		}
	}
	return QWidget::event(event);
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::mouseDoubleClickEvent(QMouseEvent* event)
{
	if(event->button() != Qt::LeftButton)
	{
		return;
	}

	MousePosition = event->pos();
	if(mousePositionInTitleBar(MousePosition))
	{
		slotMaximizeInnerPanelClick();
	}

	QWidget::mouseDoubleClickEvent(event);
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::mousePressEvent(QMouseEvent* event)
{
	/*if(event->button() != Qt::LeftButton)
	{
		return;
	}*/
	if(!Dummy->geometry().contains(event->pos()))
	{
		return;
	}

	MousePressPosition = event->pos();
	MousePosition = MousePressPosition;
	MousePressPositionMappedInWindowDiscrete = mapToViewPort(MousePressPosition, true);
	MousePressPositionMappedInWindowContinuum = mapToViewPort(MousePressPosition, false);

	MousePositionMappedInWindowDiscrete = MousePressPositionMappedInWindowDiscrete;
	MousePositionMappedInWindowContinuum = MousePressPositionMappedInWindowContinuum;
	MousePressInTitleBar = mousePositionInTitleBar(MousePressPosition);

	CurrentDragType = VEDT_NONE;
	CurrentShapeTransform.ActiveControlPointIndex = -1;
	CurrentShapeTransform.StartPostion = MousePressPositionMappedInWindowDiscrete;
	CurrentShapeTransform.CurrentPostion = MousePositionMappedInWindowDiscrete;
	CurrentShapeTransform.Type = VESTT_NONE;

	if(ToolManager->isAddingItem())
	{

		WorkArea->setSelectedGraphicsPanel(this);

		//qDebug() << "click::isAddingItem";
		createFakeItem();
		CurrentShapeTransform.Type = VESTT_ADDING_NEW_ITEM;
		//FakeItem->beginEdit();
		FakeItem->edit(CurrentShapeTransform);
		repaint();
	}
	else
	if(ToolManager->isToolSelected(VETS_SELECTION))
	{
		WorkArea->setSelectedGraphicsPanel(this);

		QList<VizIGraphicsItem*> items = itemsAt(MousePositionMappedInWindowContinuum);
		if(items.empty())
		{
			//AQUI TA SELECIONANDO UM PANEL
			if(WorkArea->setSelectedVizOBject(this))
			{
				if(WorkArea->selectedGraphicsPanel() == this)
				{
					WorkArea->updateScene();
				}
				//repaint();
			}
			if(MousePressInTitleBar)
			{
				//qDebug() << "DRAG PANEL";
				CurrentDragType = VEDT_PANEL;		
			}
		}
		else
		{
			//AQUI TA SELECIONANDO UM ITEM
			VizIGraphicsItem* selectedItem = findSelectedItem(items);
			if(selectedItem)
			{
				//AQUI JA TEM UM SELECIONADO PREVIAMENTE
				//AQUI FAZ EDICAO
				VizIGraphicsItemBasedOnShape* itemBasedOnShape = qobject_cast<VizIGraphicsItemBasedOnShape*>(selectedItem);
				if(itemBasedOnShape)
				{

					s32 index = controlPointsContains(itemBasedOnShape->controlPoints(), MousePositionMappedInWindowContinuum);
					if(index != -1)
					{
						//qDebug() << "EDICAO DRAG CONTROL POINT";
						CurrentDragType = VEDT_CONTROL_POINT;
						CurrentShapeTransform.Type = VESTT_CONTROL_POINTS;
						CurrentShapeTransform.ActiveControlPointIndex = index;
					}
					else
					{
						//qDebug() << "EDICAO DRAG SHAPE";
						CurrentDragType = VEDT_SHAPE;
						CurrentShapeTransform.Type = VESTT_MOVE;
						CurrentShapeTransform.ActiveControlPointIndex = index;
					}

					itemBasedOnShape->beginEdit(CurrentShapeTransform);
				}
				else
				{
					CurrentDragType = VEDT_SECURITY;
					//qDebug() << "EDICAO DRAG SECURITY";
				}
			}
			else
			{
				//AQUI FAZ SELECAO
				if(items.size() > 1)
				{
					//TEM MULTIPLOS
					MultipleSelectMenu->clear();
					foreach(VizIGraphicsItem* item, items)
					{
						QString name = vizObjectName(item);
						name = tr("Selecionar %1").arg(name);
						QAction* action = new QAction(name, item);
						if(item->isSelected())
						{
							action->setCheckable(true);
							action->setChecked(true);
						}
						MultipleSelectMenu->addAction(action);
					}
					MultipleSelectMenu->exec(QCursor::pos());
				}
				else
				{
					//TEM UM SO
					if(WorkArea->setSelectedVizOBject(items.first()))
					{
						//repaint();
					}
					
					if(WorkArea->selectedGraphicsPanel() == this)
					{
						WorkArea->updateScene();
					}

					VizIGraphicsItemBasedOnShape* itemBasedOnShape = qobject_cast<VizIGraphicsItemBasedOnShape*>(items.first());
					if(itemBasedOnShape)
					{
						//qDebug() << "SELECT DRAG SHAPE";
						CurrentDragType = VEDT_SHAPE;
						CurrentShapeTransform.Type = VESTT_MOVE;
						itemBasedOnShape->beginEdit(CurrentShapeTransform);
					}
					else
					{
						//qDebug() << "SELECT DRAG SECURITY";
						CurrentDragType = VEDT_SECURITY;

					}
				}
			}
		}
	}
	else
	if(ToolManager->isToolSelected(VETS_ZOOM_BOX))
	{
		CurrentDragType = VEDT_ZOOM_BOX;
	}
	else
	if(ToolManager->isToolSelected(VETS_PAN))
	{
		CurrentDragType = VECT_PAN;
		MousePan = 0.0;

	}

	if(event->button() == Qt::RightButton)
	{
		EditMenu->exec(QCursor::pos());
	}


	//QWidget::mousePressEvent(event);
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::mouseMoveEvent(QMouseEvent* event)
{
	QPoint _p = QPoint(event->pos().x(), Dummy->geometry().y());

	if(!Dummy->geometry().contains(_p) && !ToolManager->isToolSelected(VETS_PAN))
	{
		return;
	}

	MousePosition = event->pos();
	MousePositionMappedInWindowDiscrete = mapToViewPort(MousePosition, true);
	MousePositionMappedInWindowContinuum = mapToViewPort(MousePosition, false);

	if(ToolManager->isToolChecked(VETC_CROSSHAIRS))
	{
		CrossHairsCurrentGraphicsPanel = this;
		repaint();
	}
	
	CurrentShapeTransform.StartPostion = MousePressPositionMappedInWindowDiscrete;
	CurrentShapeTransform.CurrentPostion = MousePositionMappedInWindowDiscrete;

	bool isEditingShape = CurrentDragType == VEDT_CONTROL_POINT || CurrentDragType == VEDT_SHAPE;
	if(!isEditingShape && (MousePosition - MousePressPosition).manhattanLength() < QApplication::startDragDistance())
	{
		return;
	}

	if(ToolManager->isAddingItem() && FakeItem)
	{
		//qDebug() << "move::isAddingItem";
		//Q_ASSERT(FakeItem);
		FakeItem->edit(CurrentShapeTransform);
		repaint();
	}
	else
	if(ToolManager->isToolSelected(VETS_SELECTION))
	{
		switch(CurrentDragType)
		{
			case VEDT_NONE:
			{
				break;
			}
			case VEDT_CONTROL_POINT:
			{
				//qDebug() << "VEDT_CONTROL_POINT";
				VizIGraphicsItemBasedOnShape* item = qobject_cast<VizIGraphicsItemBasedOnShape*>(WorkArea->selectedVizOBject());
				Q_ASSERT(item);
				item->edit(CurrentShapeTransform);
				repaint();
				break;
			}
			case VEDT_SHAPE:
			{
				//qDebug() << "VEDT_SHAPE";
				VizIGraphicsItemBasedOnShape* item = qobject_cast<VizIGraphicsItemBasedOnShape*>(WorkArea->selectedVizOBject());
				Q_ASSERT(item);
				item->edit(CurrentShapeTransform);
				repaint();
				break;
			}
			case VEDT_SECURITY:
			{
				//qDebug() << "VEDT_SECURITY";
				QByteArray itemData;
				QDataStream dataStream(&itemData, QIODevice::WriteOnly);
				VizIGraphicsItem* graphicsItem = qobject_cast<VizIGraphicsItem*>(WorkArea->selectedVizOBject());
				Q_ASSERT(graphicsItem);
				QMimeData* mimeData = new QMimeData;
				s32 index = graphicsItemIndex(graphicsItem);

				//qDebug() << "index: " << index << graphicsItem;

				dataStream << index;
				mimeData->setData(VizIGraphicsItemMimeFormat, itemData);

				QDrag* drag = new QDrag(this);
				drag->setMimeData(mimeData);
				Qt::DropActions dropActionIn;
				dropActionIn = Qt::MoveAction | Qt::CopyAction;
				Qt::DropAction dropActionOut = drag->exec(dropActionIn, Qt::MoveAction);		

				CurrentDragType = VEDT_NONE;

				break;
			}
			case VEDT_PANEL:
			{
				//qDebug() << "VEDT_PANEL";
				if(MousePressInTitleBar)
				{
					QByteArray itemData;
					QDataStream dataStream(&itemData, QIODevice::WriteOnly);
					VizCGraphicsPanel* innerPanel = qobject_cast<VizCGraphicsPanel*>(WorkArea->selectedVizOBject());
					Q_ASSERT(innerPanel == this);

					QMimeData* mimeData = new QMimeData;
					mimeData->setData(VizInnerPanelMimeFormat, itemData);

					QDrag* drag = new QDrag(this);
					drag->setMimeData(mimeData);
					Qt::DropActions dropActionIn;
					dropActionIn = Qt::MoveAction | Qt::CopyAction;
					Qt::DropAction dropActionOut = drag->exec(dropActionIn, Qt::MoveAction);
					//importante
					MousePressInTitleBar = false;
				}

				break;
			}
		}
	}
	else
	if(ToolManager->isToolSelected(VETS_PAN))
	{
		if(CurrentDragType == VECT_PAN)
		{
			f64 delta = MousePan != 0.0 ? MousePosition.x() - MousePan : 0.0;

			delta = delta > 0.0 ? 1.0 : -1.0;

			//qDebug() << delta;

			HorizontalScale->pan(static_cast<s32>(delta));

			MousePan = MousePosition.x();
		}
	}
	else
	if(ToolManager->isToolSelected(VETS_ZOOM_BOX))
	{
		if(CurrentDragType == VEDT_ZOOM_BOX)
		{
			ZoomBoxCurrentGraphicsPanel = this;
			repaint();
		}
	}

	//////////////////////////////////CU
	VizIGraphicsItemBasedOnSecurity* baseItem = baseGraphicsItem();
	if(baseItem)
	{
		const QVector<core::VizCTradePeriod>& periods = baseItem->periods();
		s32 index = static_cast<s32>(MousePositionMappedInWindowDiscrete.x());

		if(core::isInRange(index, 0, periods.size()-1))
		{
			const core::VizCTradePeriod& period = periods[index];
			const QString& securitySymbol = baseItem->securitySymbol();
			WorkArea->dataInspector()->setData(securitySymbol, MousePositionMappedInWindowDiscrete.y(), period);
		}

	}
	
		
	QWidget::mouseMoveEvent(event);

}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::mouseReleaseEvent(QMouseEvent* event)
{

	QPoint _p = QPoint(event->pos().x(), Dummy->geometry().y());
	if(Dummy->geometry().contains(_p))
	{
		MousePosition = event->pos();
		MousePositionMappedInWindowDiscrete = mapToViewPort(MousePosition, true);
		MousePositionMappedInWindowContinuum = mapToViewPort(MousePosition, false);

		CurrentShapeTransform.StartPostion = MousePressPositionMappedInWindowDiscrete;
		CurrentShapeTransform.CurrentPostion = MousePositionMappedInWindowDiscrete;
	}


	if(ToolManager->isAddingItem() && FakeItem)
	{
		//qDebug() << "release::isAddingItem";
		//Q_ASSERT(FakeItem);
		
		FakeItem->endEdit(CurrentShapeTransform);
		addItemBasedOnFakeItem();
	}
	else
	if(ToolManager->isToolSelected(VETS_SELECTION))
	{
		switch(CurrentDragType)
		{
			case VEDT_CONTROL_POINT:
			case VEDT_SHAPE:
			{

				VizIGraphicsItemBasedOnShape* item = qobject_cast<VizIGraphicsItemBasedOnShape*>(WorkArea->selectedVizOBject());
				Q_ASSERT(item);
				item->endEdit(CurrentShapeTransform);
				repaint();
				break;
			}
		}
	}
	else
	if(ToolManager->isToolSelected(VETS_ZOOM_BOX))
	{
		if(CurrentDragType == VEDT_ZOOM_BOX)
		{
			ZoomBoxCurrentGraphicsPanel = 0;
			f64 minY = core::min(MousePressPositionMappedInWindowDiscrete.y(), MousePositionMappedInWindowDiscrete.y());
			f64 maxY = core::max(MousePressPositionMappedInWindowDiscrete.y(), MousePositionMappedInWindowDiscrete.y());

			s32 minX = core::min(MousePressPositionMappedInWindowDiscrete.x(), MousePositionMappedInWindowDiscrete.x());
			s32 maxX = core::max(MousePressPositionMappedInWindowDiscrete.x(), MousePositionMappedInWindowDiscrete.x());

			if(!core::equals(minX, minX) || !core::equals(minY, maxY))
			{
				bool as = VerticalScale->isAutoScale();
				if(as)
				{
					VerticalScale->setAutoScale(false);
				}
				VerticalScale->blockSignals(true);
				VerticalScale->setRange(minY, maxY);
				VerticalScale->blockSignals(false);
				HorizontalScale->zoomBox(minX, maxX);
				if(as)
				{
					VerticalScale->blockSignals(true);
					VerticalScale->setAutoScale(true);
					VerticalScale->blockSignals(false);
				}
			}
			else
			{
				QApplication::beep();
				repaint();
			}
		}
	}

	MousePressInTitleBar = false;
	CurrentDragType = VEDT_NONE;
	
	CurrentShapeTransform.Type = VESTT_NONE;
	CurrentShapeTransform.ActiveControlPointIndex = -1;



	//QWidget::mouseReleaseEvent(event);
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::leaveEvent(QEvent* event)
{

	ZoomBoxCurrentGraphicsPanel = 0;
	CrossHairsCurrentGraphicsPanel = 0;
	repaint();

	WorkArea->dataInspector()->clearData();

	QWidget::leaveEvent(event);
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::keyPressEvent(QKeyEvent* event)
{
	QString troca = "";

	switch(event->key())
	{
		case Qt::Key_1:
		{
			troca = "viz::gui::VizCLineChart";
			break;
		}
		case Qt::Key_2:
		{
			troca = "viz::gui::VizCCandlesticksChart";
			break;
		}
		case Qt::Key_3:
		{
			troca = "viz::gui::VizCBarsChart";
			break;
		}
		case Qt::Key_4:
		{
			troca = "viz::gui::VizCHeikinAshiBarsChart";
			break;
		}
		case Qt::Key_5:
		{
			troca = "viz::gui::VizCHeikinAshiCandlesticksChart";
			break;
		}
	}
	

	if(troca != "")
	{
		VizIChart* graphicsItem = qobject_cast<VizIChart*>(WorkArea->selectedVizOBject());
		if(graphicsItem)
		{
			VizIChart* newgi = changeChartType(graphicsItem, troca);
			WorkArea->setSelectedVizOBject(newgi);
		}

	}

	QWidget::keyPressEvent(event);
}
//-----------------------------------------------------------------------------
s32 VizCGraphicsPanel::graphicsItemIndex(VizIGraphicsItem* gi) const
{
	for(s32 i = 0; i < Items.size(); i++)
	{
		const VizIGraphicsItem* item = Items[i];
		if(item == gi)
		{
			return i;
		}
	}
	return -1;
}
//-----------------------------------------------------------------------------
VizIGraphicsItemBasedOnSecurity* VizCGraphicsPanel::baseGraphicsItem() const
{
	foreach(VizIGraphicsItem* item, Items)
	{
		VizIGraphicsItemBasedOnSecurity* ibos = qobject_cast<VizIGraphicsItemBasedOnSecurity*>(item);
		if(ibos)
		{
			if(!ibos->isRemovable())
			{
				return ibos;
			}
		}
	}
	return 0;

}
//-----------------------------------------------------------------------------
VizIChart* VizCGraphicsPanel::changeChartType(VizIChart* chart, const QString& newChartTypeName)
{
	//nao funfa direito ainda, quando troca ele perde o evento de atualizacao ??/
	//aqui tambem tem que arrumar pra nao dar problema em chamar o update varias vezes 
	
	disconnectItemsFromChangedEvent();

	bool isRemovable = chart->isRemovable();
	VizIChart* newitem = qobject_cast<VizIChart*>(privateNewGraphicsItemBasedOnSecurity(newChartTypeName, chart->securitySymbol(), chart->scaleInYOptions(), isRemovable));
	Q_ASSERT(newitem);
	newitem->setHorizontalScale(HorizontalScale);
	newitem->setVerticalScale(VerticalScale);

	for(s32 i = 0; i < chart->metaObject()->propertyCount(); i++)
	{
		QMetaProperty p = chart->metaObject()->property(i);
		if(p.isUser())
		{
			newitem->setProperty(p.name(), chart->property(p.name()));
		}
	}

	//trocar os filhos do item
/*
	foreach(VizIGraphicsItem* child, chart->childItems())
	{
		VizIGraphicsItem* clone = recursiveCloneItem(newitem, child);
		//recursiveAddCopyGraphicsItem(newitem, child, isRemovable);
	}

	bool isSelected = chart->isSelected();

	chart->setIsRemovable(true);
	removeItem(chart);
	chart->deleteLater();

	connectItemsFromChangedEvent();

	if(isSelected)
	{
		WorkArea->setSelectedVizOBject(newitem);
	}
*/

	return newitem;
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::slotMultipleSelectMenu(QAction* action)
{

	WorkArea->setSelectedVizOBject(action->parent());
	if(WorkArea->selectedGraphicsPanel() == this)
	{
		WorkArea->updateScene();
	}

}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::paintEvent(QPaintEvent* event)
{
	QTransform transform = transformViewPortToWindow();

	QRectF viewport = gui::graphicsPanelViewPort(VerticalScale, HorizontalScale);

	QPainter painter(this);

	painter.setTransform(transform);

	painter.fillRect(viewport, BackGroundColor);
	painter.setBackground(BackGroundColor);

	bool isSemiLogarithmicScale = VerticalScale->isSemiLogarithmicScale();

	if(VerticalScale->isGridVisible())
	{
		QVector<QPointF> pointPairs;
		const QVector<f64>& rulesValues = VerticalScale->rulerValues();

		for(s32 i = 0; i < rulesValues.size(); i++)
		{
			f64 rv = rulesValues[i];
			if(isSemiLogarithmicScale)
			{
				rv = core::clampLog10f(rv);
			}
			QPointF p1(viewport.left(), rv);
			QPointF p2(viewport.right(), rv);
			pointPairs.append(p1);
			pointPairs.append(p2);
		}
		qreal pw = 0.0;
		VizEPenWidth PenWidth = VerticalScale->gridPenWidth();
		VizEPenStyle PenStyle = VerticalScale->gridPenStyle();
		if(PenWidth != viz::VEPW_1)
		{
			pw = static_cast<qreal>(PenWidth);
		}
		QPen pen(VerticalScale->gridColor(), pw, static_cast<Qt::PenStyle>(PenStyle));
		pen.setCosmetic(true);
		painter.setPen(pen);
		painter.drawLines(pointPairs);
	}

	if(HorizontalScale->isGridVisible())
	{
		QVector<QPointF> pointPairs;
		foreach(s32 index, HorizontalScale->visibleRulerValuesIndex())
		{
			QPointF p1(index, viewport.bottom());
			QPointF p2(index, viewport.top());
			pointPairs.append(p1);
			pointPairs.append(p2);
		}
		qreal pw = 0.0;
		VizEPenWidth PenWidth = HorizontalScale->gridPenWidth();
		VizEPenStyle PenStyle = HorizontalScale->gridPenStyle();
		if(PenWidth != viz::VEPW_1)
		{
			pw = static_cast<qreal>(PenWidth);
		}
		QPen pen(HorizontalScale->gridColor(), pw, static_cast<Qt::PenStyle>(PenStyle));
		pen.setCosmetic(true);
		painter.setPen(pen);
		painter.drawLines(pointPairs);
	}


	if(Antialiasing)
	{
		painter.setRenderHints(QPainter::Antialiasing, true);
		painter.setRenderHints(QPainter::TextAntialiasing, true);
	}

	foreach(VizIGraphicsItem* item, Items)
	{
		if(item->isRoot())
		{
			recursiveBuild(item);
			recursivePaint(&painter, item);
		}
		//item->paint(&painter);
	}

	if(FakeItem && FakeItem->isEditing())
	{
		FakeItem->paint(&painter);
	}

	if(Antialiasing)
	{
		painter.setRenderHints(QPainter::Antialiasing, false);
		painter.setRenderHints(QPainter::TextAntialiasing, false);
	}

	VizIGraphicsItemBasedOnShape* selectedObject = qobject_cast<VizIGraphicsItemBasedOnShape*>(WorkArea->selectedVizOBject());
	if(this == WorkArea->selectedGraphicsPanel() && selectedObject && vizObjectIsSelected(selectedObject) && selectedObject->isVisible() && !selectedObject->isEditing())
	{
		drawControlPoints(&painter, selectedObject->controlPoints());
	}

	if(this == CrossHairsCurrentGraphicsPanel && ToolManager->isToolChecked(VETC_CROSSHAIRS))
	{
		drawCrossHairs(&painter);
	}

	if(this == ZoomBoxCurrentGraphicsPanel && ToolManager->isToolSelected(VETS_ZOOM_BOX))
	{
		drawZoomBox(&painter);
	}


	QRect centerRect = LoadingAnimation->rect();
	centerRect.moveCenter(Dummy->rect().center());
	LoadingAnimation->setGeometry(centerRect);

	VizIInnerPanel::paintEvent(event);

}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::slotCloseInnerPanelClick()
{
	VizIInnerPanel::slotCloseInnerPanelClick();
	if(WorkArea->selectedGraphicsPanel() == this)
	{
		WorkArea->setSelectedGraphicsPanel(0);
	}

}
//-----------------------------------------------------------------------------
bool VizCGraphicsPanel::isAnyItemSelected() const
{
	foreach(VizIGraphicsItem* item, Items)
	{
		QVariant v = item->property("selected");
		Q_ASSERT(v.isValid());
		if(v.toBool() == true)
		{
			return true;
		}
	}

	return false;
}
//-----------------------------------------------------------------------------
bool VizCGraphicsPanel::isAnyThingSelected() const
{
	if(VizIInnerPanel::isAnyThingSelected())
	{
		return true;
	}

	QVariant v = VerticalScale->property("selected");
	Q_ASSERT(v.isValid());
	if(v.toBool() == true)
	{
		return true;
	}

	return isAnyItemSelected();
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::updateData()
{
	for(s32 i = 0; i < Items.size(); i++)
	{
		VizIGraphicsItemBasedOnSecurity* item = qobject_cast<VizIGraphicsItemBasedOnSecurity*>(Items[i]);
		if(item)
		{
			item->updateData();
		}
	}
}
//-----------------------------------------------------------------------------
QString VizCGraphicsPanel::titleBarName() const
{
	QList<VizIGraphicsItemBasedOnSecurity*> validItems;

	foreach(VizIGraphicsItem* item, Items)
	{
		if(!item->isRoot())
		{
			continue;
		}
		VizIGraphicsItemBasedOnSecurity* casted = qobject_cast<VizIGraphicsItemBasedOnSecurity*>(item);
		if(casted)
		{
			validItems.append(casted);
		}
	}

	if(validItems.empty())
	{
		return tr("Painel Interno Vazio");
	}


	s32 size = validItems.size();
	QString name = "";
	for(s32 i = 0; i < size; i++)
	{
		VizIGraphicsItemBasedOnSecurity* item = validItems[i];

		QString n = item->nameToTitleBar(ShowValuesInTheTitleBar, UseColorOfPlotInValuesOfTitleBar);
		name += n;

		if(i != size - 1)
		{
			name += ", ";
		}
	}
	return name;
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::recursiveBuild(VizIGraphicsItem* item)
{
	bool endRecursion = false;
	if(!item->hasChilds())
	{
		endRecursion = true;
	}

	item->build();

	if(!endRecursion)
	{
		foreach(VizIGraphicsItem* child, item->childItems())
		{
			recursiveBuild(child);
		}
	}
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::recursiveYRange(VizIGraphicsItem* item, bool& isEmpty, f64& minY, f64& maxY)
{
	bool endRecursion = false;
	if(!item->hasChilds())
	{
		endRecursion = true;
	}

	item->yRange(isEmpty, minY, maxY);

	if(!endRecursion)
	{
		foreach(VizIGraphicsItem* child, item->childItems())
		{
			recursiveYRange(child, isEmpty, minY, maxY);
		}
	}
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::recursiveAjustToOverlayWithoutScale(VizIGraphicsItem* item, f64 itemMinY, f64 itemMaxY)
{
	bool endRecursion = false;
	if(!item->hasChilds())
	{
		endRecursion = true;
	}

	item->ajustToOverlayWithoutScale(itemMinY, itemMaxY);

	if(!endRecursion)
	{
		foreach(VizIGraphicsItem* child, item->childItems())
		{
			recursiveAjustToOverlayWithoutScale(child, itemMinY, itemMaxY);
		}
	}
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::recursivePaint(QPainter* painter, VizIGraphicsItem* item)
{
	//filhos pinta depois
	//item->paint(painter);

	//bruxaria! nao mexer! :)
	if(!item->hasChilds())
	{
		if(!(item->parentItem() && item->parentItem()->scaleInYOptions() == viz::VESIYO_OVERLAY_WITHOUT_SCALE))
		{
			if(item->scaleInYOptions() == viz::VESIYO_OVERLAY_WITHOUT_SCALE)
			{
				bool isEmpty;
				f64 itemMaxY = std::numeric_limits<f64>::min();
				f64 itemMinY = std::numeric_limits<f64>::max();
				item->yRange(isEmpty, itemMinY, itemMaxY);
				item->ajustToOverlayWithoutScale(itemMinY, itemMaxY);
			}
		}
	}
	else
	{
		if(item->scaleInYOptions() == viz::VESIYO_OVERLAY_WITHOUT_SCALE)
		{
			bool isEmpty;
			f64 itemMaxY = std::numeric_limits<f64>::min();
			f64 itemMinY = std::numeric_limits<f64>::max();
			recursiveYRange(item, isEmpty, itemMinY, itemMaxY);
			recursiveAjustToOverlayWithoutScale(item, itemMinY, itemMaxY);
		}
		foreach(VizIGraphicsItem* child, item->childItems())
		{
			recursivePaint(painter, child);
		}
	}

	//filhos pinta antes
	if(item->isVisible())
	{
		item->paint(painter);
	}
	

}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::drawControlPoints(QPainter* painter, const QList<QPointF>& points)
{

	QColor shadowColor = gui::computeShadowColor(BackGroundColor);
	QSizeF size = mapToViewPort(QSize(ControlPointSize-1, ControlPointSize-1));
	foreach(QPointF p, points)
	{
		QPointF point(p.x() - (size.width() * 0.5), p.y() - (size.height() * 0.5));
		QRectF rect(point, size);
		painter->fillRect(rect, shadowColor);
	}

	painter->save();

	QSizeF sizePlusOne = mapToViewPort(QSize(ControlPointSize+2, ControlPointSize+2));
	QPen constrolPointsPen(Qt::white, 1.0, Qt::SolidLine);
	constrolPointsPen.setCosmetic(true);
	painter->setPen(constrolPointsPen);
	painter->setCompositionMode(QPainter::CompositionMode_Difference);
	foreach(QPointF p, points)
	{
		QPointF point(p.x() - (sizePlusOne.width() * 0.5), p.y() - (sizePlusOne.height() * 0.5));
		QRectF rect(point, sizePlusOne);
		painter->drawRect(rect);
	}

	painter->restore();
}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::drawCrossHairs(QPainter* painter)
{

	QRectF viewport = gui::graphicsPanelViewPort(VerticalScale, HorizontalScale);

	f64 value0 = MousePositionMappedInWindowDiscrete.x();
	f64 value1 = MousePositionMappedInWindowDiscrete.y();
	
	QPointF p11 = QPointF(viewport.left(), value1);
	QPointF p12 = QPointF(viewport.right(), value1);

	QPointF p01 = QPointF(value0, viewport.top());
	QPointF p02 = QPointF(value0, viewport.bottom());


	QColor shadowColor = gui::computeShadowColor(BackGroundColor);
	QPen shadowPen(shadowColor, 1.0, Qt::SolidLine);
	shadowPen.setCosmetic(true);
	painter->setPen(shadowPen);
	painter->drawLine(p01, p02);
	painter->drawLine(p11, p12);

}
//-----------------------------------------------------------------------------
void VizCGraphicsPanel::drawZoomBox(QPainter* painter)
{

	QRectF viewport = gui::graphicsPanelViewPort(VerticalScale, HorizontalScale);

	f64 minY = core::min(MousePressPositionMappedInWindowDiscrete.y(), MousePositionMappedInWindowDiscrete.y());
	f64 maxY = core::max(MousePressPositionMappedInWindowDiscrete.y(), MousePositionMappedInWindowDiscrete.y());
	s32 minX = core::min(MousePressPositionMappedInWindowDiscrete.x(), MousePositionMappedInWindowDiscrete.x());
	s32 maxX = core::max(MousePressPositionMappedInWindowDiscrete.x(), MousePositionMappedInWindowDiscrete.x());
	QRectF ZoomBox(QPointF(minX, minY), QSizeF(maxX - minX, maxY - minY));

	QColor shadowColor = gui::computeShadowColor(BackGroundColor);
	QPen shadowPen(shadowColor, 1.0, Qt::DashLine);
	shadowPen.setCosmetic(true);
	painter->setPen(shadowPen);
	painter->drawRect(ZoomBox);
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------


