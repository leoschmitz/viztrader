//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCSecurityWidget.h"
#include "VizCGraphicsPanel.h"
#include "VizCOfferBookPanel.h"
#include "VizIChart.h"
#include "VizIIndicator.h"
#include "VizMimeFormat.h"
#include "VizCPropertyEditor.h"
#include "VizCSceneInspector.h"
#include "VizCUndoManager.h"
#include "VizCWorkArea.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QMetaProperty>
#include <QApplication>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
const int Width = 50;
const int Height = 25;
//-----------------------------------------------------------------------------
VizCSecurityWidget::VizCSecurityWidget(const QString& label, bool isClosable, VizCWorkArea* workArea, QWidget* parent) : QWidget(parent), IsClosable(isClosable), HorizontalScale(0), WorkArea(workArea)
{
	Q_ASSERT(WorkArea);

	setWindowTitle(label);
	//setAttribute(Qt::WA_StaticContents);

	setAttribute(Qt::WA_DeleteOnClose);
	//setMouseTracking(true);
	setAcceptDrops(true);

	Splitter = new QSplitter(this);
	Splitter->setOrientation(Qt::Vertical);
	Splitter->setChildrenCollapsible(false);

	HorizontalScale = new VizCHorizontalScale(this, WorkArea);
	QObject::connect(HorizontalScale, SIGNAL(horizontalScaleChange(bool, bool, bool, bool)), this, SLOT(slotHorizontalScaleChange(bool, bool, bool, bool)));

	QVBoxLayout* layoutV = new QVBoxLayout;
	layoutV->addWidget(Splitter);
	layoutV->addWidget(HorizontalScale);
	layoutV->setMargin(0);
	layoutV->setSpacing(0);
	setLayout(layoutV);

	DataFacade = io::VizCDataFacade::instance();

	QObject::connect(DataFacade, SIGNAL(securityChanged(const QString&, bool)), this, SLOT(securityChanged(const QString&, bool)));
	QObject::connect(DataFacade, SIGNAL(securityPercentageLoaded(const QString&, int)), this, SLOT(securityPercentageLoaded(const QString&, int)));

	//setFocusPolicy(Qt::StrongFocus);

}
//-----------------------------------------------------------------------------
VizCSecurityWidget::~VizCSecurityWidget()
{
}
//-----------------------------------------------------------------------------
void VizCSecurityWidget::securityChanged(const QString& securitySymbol, bool input)
{
	qDebug() << securitySymbol << input;

	if(input)
	{
		availableSecuritySymbol(securitySymbol);
	}
	else
	{
		unavailableSecuritySymbol(securitySymbol);
	}

	//setFocus(Qt::ActiveWindowFocusReason);
}
//-----------------------------------------------------------------------------
void VizCSecurityWidget::securityPercentageLoaded(const QString& securitySymbol, int percent)
{

	//qDebug() << percent;

	for(s32 i = 0; i < countInnerPanel(); i++)
	{
		VizCGraphicsPanel* graphicspanel = qobject_cast<VizCGraphicsPanel*>(innerPanel(i));
		if(graphicspanel)
		{
			foreach(VizIGraphicsItem* item, graphicspanel->items())
			{
				VizIGraphicsItemBasedOnSecurity* itemBasedOnSecurity = qobject_cast<VizIGraphicsItemBasedOnSecurity*>(item);
				if(itemBasedOnSecurity && itemBasedOnSecurity->securitySymbol() == securitySymbol)
				{
					graphicspanel->setPercentageLoaded(percent);
				}
			}
		}
	}
}
//-----------------------------------------------------------------------------
void VizCSecurityWidget::availableSecuritySymbol(const QString& securitySymbol)
{
	bool have = false;

	for(s32 i = 0; i < countInnerPanel(); i++)
	{
		VizCGraphicsPanel* graphicspanel = qobject_cast<VizCGraphicsPanel*>(innerPanel(i));
		if(graphicspanel)
		{
			foreach(VizIGraphicsItem* item, graphicspanel->items())
			{
				VizIGraphicsItemBasedOnSecurity* itemBasedOnSecurity = qobject_cast<VizIGraphicsItemBasedOnSecurity*>(item);
				if(itemBasedOnSecurity && itemBasedOnSecurity->securitySymbol() == securitySymbol)
				{
					graphicspanel->setLoadingState(false);
					have = true;
				}
			}
		}
	}

	if(have)
	{
		qDebug() << securitySymbol << "in cache";
		slotHorizontalScaleChange(true, true, true, true);
	}
}
//-----------------------------------------------------------------------------
void VizCSecurityWidget::unavailableSecuritySymbol(const QString& securitySymbol)
{
	bool have = false;

	for(s32 i = 0; i < countInnerPanel(); i++)
	{
		VizCGraphicsPanel* graphicspanel = qobject_cast<VizCGraphicsPanel*>(innerPanel(i));
		if(graphicspanel)
		{
			foreach(VizIGraphicsItem* item, graphicspanel->items())
			{
				VizIGraphicsItemBasedOnSecurity* itemBasedOnSecurity = qobject_cast<VizIGraphicsItemBasedOnSecurity*>(item);
				if(itemBasedOnSecurity && itemBasedOnSecurity->securitySymbol() == securitySymbol)
				{
					graphicspanel->setLoadingState(true);
					have = true;
				}
			}
		}
	}

	if(have)
	{
		qDebug() << securitySymbol << "out cache";
		bool ok = DataFacade->loadSecurity(securitySymbol);
		Q_ASSERT(ok);
	}

}
//-----------------------------------------------------------------------------
VizCGraphicsPanel* VizCSecurityWidget::privateAddGraphicsPanel(bool isClosable, s32 index)
{
	VizCGraphicsPanel* ip = new VizCGraphicsPanel(this, isClosable, WorkArea);

	Splitter->insertWidget(index, ip);

	VizIInnerPanel* maximizedInnerPanel = 0;
	for(s32 i = 0; i < countInnerPanel(); i++)
	{
		if(innerPanel(i)->isMaximized())
		{
			maximizedInnerPanel = innerPanel(i);
			break;
		}
	}

	if(maximizedInnerPanel)
	{
		maximizedInnerPanel->showNormal();
		showMaximizeInnerPanel(maximizedInnerPanel);

		ip->showMaximized();
		showMaximizeInnerPanel(ip);
	}

	manageTitleButtons();

	return ip;
}
//-----------------------------------------------------------------------------
VizCGraphicsPanel* VizCSecurityWidget::newGraphicsPanel(bool isClosable)
{
	return privateAddGraphicsPanel(isClosable, -1);
}
//-----------------------------------------------------------------------------
VizCOfferBookPanel* VizCSecurityWidget::newOfferBookPanel(bool isClosable)
{
	VizCOfferBookPanel* ip = new VizCOfferBookPanel(this, isClosable, WorkArea);

	Splitter->addWidget(ip);

	VizIInnerPanel* maximizedInnerPanel = 0;
	for(s32 i = 0; i < countInnerPanel(); i++)
	{
		if(innerPanel(i)->isMaximized())
		{
			maximizedInnerPanel = innerPanel(i);
			break;
		}
	}

	if(maximizedInnerPanel)
	{
		maximizedInnerPanel->showNormal();
		showMaximizeInnerPanel(maximizedInnerPanel);

		ip->showMaximized();
		showMaximizeInnerPanel(ip);

	}

	manageTitleButtons();

	return ip;
}
//-----------------------------------------------------------------------------
void VizCSecurityWidget::changeInnerPanel(VizIInnerPanel* w, int index)
{
	Q_ASSERT(w);

	Splitter->insertWidget(index, w);

	manageTitleButtons();

}
//-----------------------------------------------------------------------------
s32 VizCSecurityWidget::indexOfPanel(VizIInnerPanel* panel) const
{
	return Splitter->indexOf(panel);
}
//-----------------------------------------------------------------------------
VizIInnerPanel* VizCSecurityWidget::innerPanel(int index) const
{
	VizIInnerPanel* w = qobject_cast<VizIInnerPanel*>(Splitter->widget(index));
	Q_ASSERT(w);
	return w;
}
//-----------------------------------------------------------------------------
void VizCSecurityWidget::closeInnerPanel(int index)
{
	VizIInnerPanel* w = innerPanel(index);
	closeInnerPanel(w);
}
//-----------------------------------------------------------------------------
void VizCSecurityWidget::closeInnerPanel(VizIInnerPanel* w)
{
	Q_ASSERT(w);
	if(w->isClosable())
	{
		w->setVisible(false);
		w->setParent(0);
		w->close();
		manageTitleButtons();
		updateData();
		updateHorizontalScale();
	}
}
//-----------------------------------------------------------------------------
void VizCSecurityWidget::showMaximizeInnerPanel(VizIInnerPanel* w)
{
	Q_ASSERT(w);
	bool isMaximized = w->isMaximized();
	for(s32 i = 0; i < Splitter->count(); i++)
	{
		Splitter->widget(i)->setHidden(isMaximized);
	}
	w->setHidden(false);
}
//-----------------------------------------------------------------------------
void VizCSecurityWidget::manageTitleButtons()
{
	s32 c = Splitter->count();
	if(c <= 1)
	{
		innerPanel(0)->setButtonsActive(false);
	}
	else
	{
		for(s32 i = 0; i < c; i++)
		{
			innerPanel(i)->setButtonsActive(true);
		}
	}
}
//-----------------------------------------------------------------------------
s32 VizCSecurityWidget::countInnerPanel() const
{
	return Splitter->count();
}
//-----------------------------------------------------------------------------
VizCHorizontalScale* VizCSecurityWidget::horizontalScale() const
{
	return HorizontalScale;
}
//-----------------------------------------------------------------------------
bool VizCSecurityWidget::isClosable() const
{
	return IsClosable;
}
//-----------------------------------------------------------------------------
void VizCSecurityWidget::ajustSecurityPeriods()
{
	for(s32 i = 0; i < countInnerPanel(); i++)
	{
		VizCGraphicsPanel* graphicspanel = qobject_cast<VizCGraphicsPanel*>(innerPanel(i));
		if(graphicspanel)
		{
			foreach(VizIGraphicsItem* item, graphicspanel->items())
			{
				VizIGraphicsItemBasedOnSecurity* itemBasedOnSecurity = qobject_cast<VizIGraphicsItemBasedOnSecurity*>(item);
				if(itemBasedOnSecurity)
				{
					itemBasedOnSecurity->ajustPeriods();
				}
			}
		}
	}
}
//-----------------------------------------------------------------------------
void VizCSecurityWidget::slotHorizontalScaleChange(bool rulerValuesIndexChange, bool rulerValuesChange, bool periodicityChange, bool marginChange)
{

	/*
	static s32 count = 0;
	qDebug() << "slotHorizontalScaleChange" << count << this;
	count++;
	*/

	if(periodicityChange)
	{
		for(s32 i = 0; i < countInnerPanel(); i++)
		{
			VizCGraphicsPanel* graphicspanel = qobject_cast<VizCGraphicsPanel*>(innerPanel(i));
			if(graphicspanel)
			{
				foreach(VizIGraphicsItem* item, graphicspanel->items())
				{
					VizIGraphicsItemBasedOnSecurity* itemBasedOnSecurity = qobject_cast<VizIGraphicsItemBasedOnSecurity*>(item);
					if(itemBasedOnSecurity)
					{
						itemBasedOnSecurity->updateData();
					}
				}
			}
		}
	}
	if(rulerValuesChange || marginChange)
	{
		updateHorizontalScale();
	}
	
	
	for(s32 i = 0; i < countInnerPanel(); i++)
	{
		VizCGraphicsPanel* graphicspanel = qobject_cast<VizCGraphicsPanel*>(innerPanel(i));
		if(graphicspanel)
		{
			graphicspanel->updateVerticalScale();
			if(!periodicityChange)
			{
				graphicspanel->repaint();
			}
		}
	}

}
//-----------------------------------------------------------------------------
void VizCSecurityWidget::updateHorizontalScale()
{
	bool findNotLinear = false;
	VizIChart* chartNotLinear = 0; 
	VizIChart* mainChart = 0; 
	QList<VizIGraphicsItemBasedOnSecurity*> itemBasedOnSecurity;
	
	for(s32 i = 0; i < countInnerPanel(); i++)
	{
		VizCGraphicsPanel* graphicspanel = qobject_cast<VizCGraphicsPanel*>(innerPanel(i));
		if(graphicspanel)
		{
			foreach(VizIGraphicsItem* item, graphicspanel->items())
			{
				VizIGraphicsItemBasedOnSecurity* ibos = qobject_cast<VizIGraphicsItemBasedOnSecurity*>(item);
				if(ibos)
				{
					VizIChart* chart = qobject_cast<VizIChart*>(ibos);

					if(chart && !graphicspanel->isClosable())
					{
						mainChart = chart;
					}

					if(chart && !chart->isLinear())
					{
						chartNotLinear = chart;
						findNotLinear = true;
						break;
					}
					itemBasedOnSecurity.append(ibos);
				}
			}
			if(findNotLinear)
			{
				break;
			}
		}
	}

	QVector<QDateTime> values;
	if(findNotLinear)
	{
		const QVector<core::VizCTradePeriod>& sp = chartNotLinear->periods();
		for(s32 i = HorizontalScale->leftMargin(); i < sp.size() - HorizontalScale->rightMargin(); i++)
		{
			values.append(sp[i].TimeStamp);
		}
	}
	else
	{
		QMap<QDateTime, bool> map;
		foreach(VizIGraphicsItemBasedOnSecurity* ibos, itemBasedOnSecurity)
		{
			const QVector<core::VizCTradePeriod>& p = ibos->periods();
			for(s32 i = HorizontalScale->leftMargin(); i < p.size() - HorizontalScale->rightMargin(); i++)
			{
				map[p[i].TimeStamp] = true;
			}
		}

		values = map.keys().toVector();
	}

	
	//tem que gerar o left e right margin tambem aqui
	//s32 begin = HorizontalScale->leftMargin();
	//s32 end = values.size() - HorizontalScale->rightMargin() - 1;

	/*s32 begin = 0;
	s32 end = values.size() - 1;

	QDateTime first = values[begin];
	QDateTime last = values[end];
	switch(HorizontalScale->periodicity())
	{
		case VESP_DAILY:
		{
			for(s32 i = 1; i < HorizontalScale->leftMargin() + 1; i++)
			{
				QDateTime dummy = first;
				dummy.addDays(-i);
				values.prepend(dummy);

			}
			for(s32 i = 1; i < HorizontalScale->rightMargin() + 1; i++)
			{
				QDateTime dummy = last;
				dummy.addDays(i);
				values.append(dummy);
			}
			break;
		}
		case VESP_WEEKLY:
		{
			for(s32 i = 1; i < HorizontalScale->leftMargin() + 1; i++)
			{
				QDateTime dummy = first.addDays(-i * 7);
				values.prepend(dummy);

			}
			for(s32 i = 1; i < HorizontalScale->rightMargin() + 1; i++)
			{
				QDateTime dummy = last.addDays(i * 7);
				values.append(dummy);
			}
			break;
		}
		case VESP_MONTHLY:
		{
			for(s32 i = 1; i < HorizontalScale->leftMargin() + 1; i++)
			{
				QDateTime dummy = first.addMonths(-i);
				values.prepend(dummy);

			}
			for(s32 i = 1; i < HorizontalScale->rightMargin() + 1; i++)
			{
				QDateTime dummy = last.addMonths(i);
				values.append(dummy);
			}
			break;
		}
		case VESP_QUARTERLY:
		{
			for(s32 i = 1; i < HorizontalScale->leftMargin() + 1; i++)
			{
				QDateTime dummy = first.addMonths(-i * 3);
				values.prepend(dummy);

			}
			for(s32 i = 1; i < HorizontalScale->rightMargin() + 1; i++)
			{
				QDateTime dummy = last.addMonths(i * 3);
				values.append(dummy);
			}
			break;
		}
		case VESP_YEARLY:
		{
			for(s32 i = 1; i < HorizontalScale->leftMargin() + 1; i++)
			{
				QDateTime dummy = first.addYears(-i);
				values.prepend(dummy);

			}
			for(s32 i = 1; i < HorizontalScale->rightMargin() + 1; i++)
			{
				QDateTime dummy = last.addYears(i);
				values.append(dummy);
			}
			break;
		}
		default:
		{
			Q_ASSERT(false);
			break;
		}
	};*/

	//vizObjectBlockSignals(HorizontalScale, true);


	Q_ASSERT(mainChart);

	HorizontalScale->setPeriodsToPreviewChart(mainChart->periods());

	HorizontalScale->setRulerValues(values);
	
	//vizObjectBlockSignals(HorizontalScale, false);
	
	ajustSecurityPeriods();

}
//-----------------------------------------------------------------------------
void VizCSecurityWidget::updateData()
{
	
	for(s32 i = 0; i < countInnerPanel(); i++)
	{
		VizIInnerPanel* innerpanel = innerPanel(i);
		innerpanel->updateData();
	}
	
}
//-----------------------------------------------------------------------------
void VizCSecurityWidget::wheelEvent(QWheelEvent* event)
{

	s32 numDegrees = event->delta() / 8.0;
	s32 numSteps = numDegrees / 15.0;

	//qDebug() << numSteps;
	
	HorizontalScale->pan(-numSteps);

}
//-----------------------------------------------------------------------------
void VizCSecurityWidget::dragEnterEvent(QDragEnterEvent* event)
{

	bool contains = Splitter->geometry().contains(event->pos());

	if(contains && event->mimeData()->hasFormat(VizInnerPanelMimeFormat))
	{
		event->acceptProposedAction();
	}
	else
	if(contains && event->mimeData()->hasFormat(VizIGraphicsItemMimeFormat))
	{
		event->acceptProposedAction();
	}

}
//-----------------------------------------------------------------------------
void VizCSecurityWidget::dropEvent(QDropEvent* event)
{
	if(event->mimeData()->hasFormat(VizInnerPanelMimeFormat))
	{
		manageInnerPanelDropEvent(event);
	}
	else
	if(event->mimeData()->hasFormat(VizIGraphicsItemMimeFormat))
	{
		manageGraphicsItemDropEvent(event);
	}
}
//-----------------------------------------------------------------------------
void VizCSecurityWidget::manageGraphicsItemDropEvent(QDropEvent* event)
{
	Qt::DropAction dropAction = event->proposedAction();
	QPoint endLocation = event->pos();

	if(!Splitter->geometry().contains(endLocation) || (dropAction != Qt::MoveAction && dropAction != Qt::CopyAction))
		return;

	QByteArray itemData = event->mimeData()->data(VizIGraphicsItemMimeFormat);
	QDataStream dataStream(&itemData, QIODevice::ReadOnly);
	s32 graphicsItemIndex;
	dataStream >> graphicsItemIndex;

	VizCGraphicsPanel* startWidget = qobject_cast<VizCGraphicsPanel*>(event->source());
	Q_ASSERT(startWidget);

	s32 startIndex = Splitter->indexOf(startWidget);
	s32 currentIndex;
	s32 nextIndex;
	findInnerPanel(endLocation, false, false, currentIndex, nextIndex);
	VizCGraphicsPanel* endWidget = qobject_cast<VizCGraphicsPanel*>(innerPanel(currentIndex));
	
	//Q_ASSERT(endWidget);
	//aqui se nao for um VizCGraphicsPanel entao nao dar um beep
	if(!endWidget)
	{
		QApplication::beep();
		return;
	}

	bool isOutside;
	if(startWidget == endWidget)
	{
		isOutside = false;
	}
	else
	{
		isOutside = true;
	}


	Q_ASSERT(graphicsItemIndex != -1);
	
	VizIGraphicsItem* graphicsItem = startWidget->items().at(graphicsItemIndex);
	Q_ASSERT(graphicsItem);

	bool isCopyAction = dropAction == Qt::CopyAction;
	bool isMoveAction = dropAction == Qt::MoveAction;

	if(isMoveAction)
	{
		if(!isOutside)
		{
			QApplication::beep();
		}
		else
		{
			if(startWidget->isPossibleRemoveItem(graphicsItem))
			{

				VizIGraphicsItem* newGraphicsItem = endWidget->cloneItem(graphicsItem, false);
				//AQUI TA EMITINDO COMANDO UNDO
				endWidget->addItem(newGraphicsItem);
				startWidget->removeItem(graphicsItem);

				WorkArea->setSelectedGraphicsPanel(endWidget);
				WorkArea->setSelectedVizOBject(newGraphicsItem);
				WorkArea->updateScene();

				/*
				VizCSceneInspector::instance()->setGraphicsPanel(endWidget);
				VizCPropertyEditor::instance()->setObject(newGraphicsItem);
				VizCSceneInspector::instance()->sceneChanged();
				*/
			}
			else
			{
				QApplication::beep();
			}
		}
	}
	else
	if(isCopyAction)
	{
		if(!endWidget->isPossibleAddItem(graphicsItem))
		{
			QApplication::beep();
		}
		else
		{
			VizIGraphicsItem* newGraphicsItem = endWidget->cloneItem(graphicsItem, false);
			endWidget->addItem(newGraphicsItem);

			/*VizCSceneInspector::instance()->setGraphicsPanel(endWidget);
			VizCPropertyEditor::instance()->setObject(newGraphicsItem);
			VizCSceneInspector::instance()->sceneChanged();
			*/

			WorkArea->setSelectedGraphicsPanel(endWidget);
			WorkArea->setSelectedVizOBject(newGraphicsItem);
			WorkArea->updateScene();


			//endWidget->repaint();
		}
	}

	event->acceptProposedAction();

}
//-----------------------------------------------------------------------------
void VizCSecurityWidget::manageInnerPanelDropEvent(QDropEvent* event)
{
	Qt::DropAction dropAction = event->proposedAction();
	QPoint endLocation = event->pos();

	if(!Splitter->geometry().contains(endLocation) || (dropAction != Qt::MoveAction && dropAction != Qt::CopyAction))
		return;

	VizIInnerPanel* startWidget = qobject_cast<VizIInnerPanel*>(event->source());
	Q_ASSERT(startWidget);

	bool isOutside;
	if(startWidget->parentWidget() == Splitter)
	{
		isOutside = false;
	}
	else
	{
		isOutside = true;
	}

	s32 startIndex = Splitter->indexOf(startWidget);
	s32 currentIndex;
	s32 nextIndex;
	bool isCopyAction = dropAction == Qt::CopyAction;
	bool isMoveAction = dropAction == Qt::MoveAction;
	findInnerPanel(endLocation, isOutside, isCopyAction, currentIndex, nextIndex);
	VizIInnerPanel* endWidget = innerPanel(currentIndex);
	Q_ASSERT(endWidget);

	if(isMoveAction)
	{
		if(!isOutside)
		{
			changeInnerPanel(startWidget, nextIndex);
		}
		else
		{
			if(startWidget->isClosable())
			{
				VizCSecurityWidget* ppother = qobject_cast<VizCSecurityWidget*>(startWidget->parentWidget()->parentWidget());
				if(startWidget->isMaximized())
				{
					startWidget->showNormal();
					ppother->showMaximizeInnerPanel(startWidget);
				}
				if(endWidget->isMaximized())
				{
					endWidget->showNormal();
					showMaximizeInnerPanel(endWidget);
				}

				VizIInnerPanel* newWidget = addCopyInnerPanel(startWidget, nextIndex);
				//changeInnerPanel(newWidget, nextIndex);
				//newWidget->setVisible(true);

				ppother->closeInnerPanel(startWidget);

				WorkArea->setSelectedVizOBject(newWidget);
				VizCGraphicsPanel* gp = qobject_cast<VizCGraphicsPanel*>(newWidget);
				if(gp)
				{
					WorkArea->setSelectedGraphicsPanel(gp);
				}
			}
			else
			{
				QApplication::beep();
			}

		}
	}
	else
	if(isCopyAction)
	{
		if(!isOutside)
		{
			VizIInnerPanel* newWidget = addCopyInnerPanel(startWidget, nextIndex);
			//changeInnerPanel(newWidget, nextIndex);
			//newWidget->setVisible(true);

			WorkArea->setSelectedVizOBject(newWidget);
			VizCGraphicsPanel* gp = qobject_cast<VizCGraphicsPanel*>(newWidget);
			if(gp)
			{
				WorkArea->setSelectedGraphicsPanel(gp);
			}

		}
		else
		{
			if(endWidget->isMaximized())
			{
				endWidget->showNormal();
				showMaximizeInnerPanel(endWidget);
			}
			VizIInnerPanel* newWidget = addCopyInnerPanel(startWidget, nextIndex);
			//changeInnerPanel(newWidget, nextIndex);
			//newWidget->setVisible(true);

			WorkArea->setSelectedVizOBject(newWidget);
			VizCGraphicsPanel* gp = qobject_cast<VizCGraphicsPanel*>(newWidget);
			if(gp)
			{
				WorkArea->setSelectedGraphicsPanel(gp);
			}


		}
	}

	event->acceptProposedAction();
	
}
//-----------------------------------------------------------------------------
VizIInnerPanel* VizCSecurityWidget::addCopyInnerPanel(VizIInnerPanel* innerpanel, s32 index)
{
	Q_ASSERT(innerpanel);

	VizCGraphicsPanel* graphicspanel = qobject_cast<VizCGraphicsPanel*>(innerpanel);
	if(graphicspanel)
	{
		VizCGraphicsPanel* newGraphicsPanel = privateAddGraphicsPanel(true, index);

		for(s32 i = 0; i < graphicspanel->metaObject()->propertyCount(); i++)
		{
			QMetaProperty p = graphicspanel->metaObject()->property(i);
			if(p.isUser())
			{
				newGraphicsPanel->setProperty(p.name(), graphicspanel->property(p.name()));
			}
		}

		foreach(VizIGraphicsItem* item, graphicspanel->items())
		{
			if(item->isRoot())
			{
				VizIGraphicsItem* newGraphicsItem = newGraphicsPanel->cloneItem(item, true);
				newGraphicsPanel->addItem(newGraphicsItem);
			}
		}

		return newGraphicsPanel;
	}
	else
	{
		//aqui vem o copiar do book de oferta e outros mais
		Q_ASSERT(true);
		return 0;
	}

}
//-----------------------------------------------------------------------------
void VizCSecurityWidget::findInnerPanel(const QPoint& p, bool isOutside, bool isCopyAction, s32& currentIndex, s32& nextIndex)
{
	if(isCopyAction)
		isOutside = true;

	currentIndex = nextIndex = -1;
	s32 count = Splitter->count();
	for(s32 i = 0; i < count; i++)
	{
		QRect geometry = Splitter->widget(i)->geometry();
		geometry.translate(QPoint(Splitter->geometry().x(), Splitter->geometry().y()));
		geometry.setHeight(geometry.height() + Splitter->handleWidth());
		geometry.setWidth(geometry.width() + Splitter->handleWidth());

		if(geometry.contains(p))
		{
			if(isOutside)
			{
				QRect tmp1 = geometry;
				tmp1.setHeight(tmp1.height() / 2);
				if(tmp1.contains(p))
				{
					currentIndex = nextIndex = i;
				}
				else
				{
					currentIndex = i;
					nextIndex = i + 1;
				}
			}
			else
			{
				currentIndex = nextIndex = i;
			}
			return;
		}
	}
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
