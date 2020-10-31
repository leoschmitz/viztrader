//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCSceneInspector.h"
#include "VizMimeFormat.h"
#include <QDebug>
#include <QHeaderView>
#include <QApplication>
#include <QMimeData>
#include <QDrag>
//-----------------------------------------------------------------------------
namespace viz
{
//-----------------------------------------------------------------------------
namespace gui
{
//-----------------------------------------------------------------------------
class VizCSceneInspectorItem : public QTreeWidgetItem
{
public:
	VizCSceneInspectorItem(QTreeWidgetItem* parent = 0, const QString& name = QString()) : QTreeWidgetItem(parent)
	{
		setText(0, name);
	}
	VizIGraphicsItem* Item;
};
//-----------------------------------------------------------------------------
VizCSceneInspector::VizCSceneInspector() : QTreeWidget(), GraphicsPanel(0)
{
	setAcceptDrops(true);
	setDragEnabled(true);

	//setAnimated(true);
	setColumnCount(2);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setItemsExpandable(true);
	setExpandsOnDoubleClick(true);
	//this->setHeaderHidden(true);
	setSortingEnabled(false);
	setWordWrap(true);
	setEditTriggers(NoEditTriggers);

	QStringList headers;
	headers << QObject::tr("Ítens") << QObject::tr("Visível");
	setHeaderLabels(headers);

	setRootIsDecorated(true);
	setAlternatingRowColors(true);

	setFocusPolicy(Qt::NoFocus);

	connect(this, SIGNAL(itemClicked(QTreeWidgetItem*, int)), SLOT(slotItemClicked(QTreeWidgetItem*, int)));

}
//-----------------------------------------------------------------------------
VizCSceneInspector::~VizCSceneInspector()
{
}
//-----------------------------------------------------------------------------
void VizCSceneInspector::mousePressEvent(QMouseEvent* event)
{
	QTreeWidget::mousePressEvent(event);

	//setFocus();

	if(event->button() != Qt::LeftButton)
	{
		return;
	}
	MousePressPosition = event->pos();
	
	/*
	ItemClicked = false;
	VizCSceneInspectorItem* sii = dynamic_cast<VizCSceneInspectorItem*>(itemAt(MousePressPosition));
	
	if(sii)
	{
		ItemClicked = true;
		VizCPropertyEditor::instance()->setObject(sii->Item);
		if(GraphicsPanel)
		{
			GraphicsPanel->setFocus();
		}
		//ajustSelectPropertyInItems();	
	}
	else
	{
		VizCPropertyEditor::instance()->setObject(0);
		ajustSelectPropertyInItems();	
	}
*/
	
}
//-----------------------------------------------------------------------------
void VizCSceneInspector::mouseMoveEvent(QMouseEvent* event)
{
	if(!ItemClicked || !(event->buttons() & Qt::LeftButton) || (event->pos() - MousePressPosition).manhattanLength() < QApplication::startDragDistance())
	{
		return;
	}

	QByteArray itemData;
	QDataStream dataStream(&itemData, QIODevice::WriteOnly);
	QMimeData* mimeData = new QMimeData;
	dataStream << MousePressPosition;
	mimeData->setData(VizIGraphicsItemMimeFormatInSceneInspector, itemData);

	QDrag* drag = new QDrag(this);
	drag->setMimeData(mimeData);
	Qt::DropActions dropActionIn;
	dropActionIn = Qt::MoveAction;
	Qt::DropAction dropActionOut = drag->exec(dropActionIn, Qt::MoveAction);

}
//-----------------------------------------------------------------------------
void VizCSceneInspector::dragEnterEvent(QDragEnterEvent* event)
{
	if(!ItemClicked || event->mimeData()->hasFormat(VizIGraphicsItemMimeFormatInSceneInspector))
	{
		event->acceptProposedAction();
	}
}
//-----------------------------------------------------------------------------
void VizCSceneInspector::dragMoveEvent(QDragMoveEvent* event)
{
	if(!ItemClicked || event->mimeData()->hasFormat(VizIGraphicsItemMimeFormatInSceneInspector))
	{
		event->acceptProposedAction();
	}
}
//-----------------------------------------------------------------------------
void VizCSceneInspector::dropEvent(QDropEvent* event)
{
	if(!ItemClicked || !event->mimeData()->hasFormat(VizIGraphicsItemMimeFormatInSceneInspector))
	{
		return;
	}

	QByteArray itemData = event->mimeData()->data(VizIGraphicsItemMimeFormatInSceneInspector);
	QDataStream dataStream(&itemData, QIODevice::ReadOnly);
	QPoint mousepos;
	dataStream >> mousepos;

	VizCSceneInspectorItem* itemSouce = dynamic_cast<VizCSceneInspectorItem*>(itemAt(mousepos));
	VizCSceneInspectorItem* itemDest = dynamic_cast<VizCSceneInspectorItem*>(itemAt(event->pos()));
	Q_ASSERT(itemSouce);

	if(itemSouce != itemDest)
	{	bool ok;
		if(itemDest)
		{
			//itemSouce virou filho do itemDest
			ok = itemSouce->Item->setParentItem(itemDest->Item);
		}
		else
		{
			//itemSouce virou root
			ok = itemSouce->Item->setParentItem(0);
		}
		if(!ok)
		{
			QApplication::beep();
		}
	}

	sceneChanged();
	GraphicsPanel->repaint();
}
//-----------------------------------------------------------------------------
void VizCSceneInspector::ajustSelectPropertyInItems()
{
	QTreeWidgetItemIterator it(this);
	while(*it)
	{
		VizCSceneInspectorItem* item = dynamic_cast<VizCSceneInspectorItem*>(*it);

		if(item)
		{
			setItemSelected(item, item->Item->isSelected());
		}
		
		++it;
	}
}
//-----------------------------------------------------------------------------
void VizCSceneInspector::sceneChanged()
{
	if(!GraphicsPanel)
		return;

	clear();

	QList<QTreeWidgetItem*> items;

	foreach(VizIGraphicsItem* item, GraphicsPanel->items())
	{
		if(item->isRoot())
		{
			QTreeWidgetItem* root = dynamic_cast<QTreeWidgetItem*>(recursiveAddItem(item));
			items.append(root);
		}
	}

	insertTopLevelItems(0, items);
	expandAll();
	
	ajustSelectPropertyInItems();
}
//-----------------------------------------------------------------------------
void VizCSceneInspector::setGraphicsPanel(VizCGraphicsPanel* graphicsPanel)
{

	if(!graphicsPanel)
	{
		clear();
		GraphicsPanel = 0;
		return;
	}

	GraphicsPanel = graphicsPanel;

	GraphicsPanel->setFocus();

	sceneChanged();


	/*if(GraphicsPanel == graphicsPanel)
	{
		return false;
	}
	
	if(!graphicsPanel)
	{
		clear();
		GraphicsPanel = 0;
		return false;
	}
	
	GraphicsPanel = graphicsPanel;

	GraphicsPanel->setFocus();

	sceneChanged();

	return true;*/
}
//-----------------------------------------------------------------------------
QTreeWidgetItem* VizCSceneInspector::recursiveAddItem(VizIGraphicsItem* item)
{
	VizCSceneInspectorItem* twi;
	bool stopRecursion = false;
	if(!item->hasChilds())
	{
		stopRecursion = true;
	}

	QString name = vizObjectName(item);
	twi = new VizCSceneInspectorItem(0, name);
	twi->setFlags(twi->flags() | Qt::ItemIsUserCheckable);
	twi->setToolTip(0, name);
	twi->Item = item;
	if(!item->isRemovable())
	{
		twi->setIcon(0, QIcon(":/VizTrader/Resources/locked.png"));
	}
	if(twi->Item->isVisible())
	{
		twi->setCheckState(1, Qt::Checked);
	}
	else
	{
		twi->setCheckState(1, Qt::Unchecked);
	}


	//twi->setFlags(Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);

	if(!stopRecursion)
	{
		foreach(VizIGraphicsItem* child, item->childItems())
		{
			QTreeWidgetItem* twc = recursiveAddItem(child);
			twi->addChild(twc);
		}
	}

	return twi;
}
//-----------------------------------------------------------------------------
void VizCSceneInspector::slotItemClicked(QTreeWidgetItem* item, int column)
{
	bool checked = false;
	if(item->checkState(column) == Qt::Checked)
	{
		checked = true;
	}


	ItemClicked = false;
	VizCSceneInspectorItem* sii = dynamic_cast<VizCSceneInspectorItem*>(item);
	if(sii)
	{
		ItemClicked = true;
		
		if(column == 1)
		{
			sii->Item->setVisible(checked);
		}
		//VizCPropertyEditor::instance()->setObject(sii->Item);
		emit vizObjectSelected(sii->Item);
		
		if(GraphicsPanel)
		{
			GraphicsPanel->setFocus();
		}
		//ajustSelectPropertyInItems();	
	}
	else
	{
		//VizCPropertyEditor::instance()->setObject(0);
		emit vizObjectSelected(0);
		ajustSelectPropertyInItems();	
	}
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------

