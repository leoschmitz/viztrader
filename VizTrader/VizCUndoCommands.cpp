//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCUndoCommands.h"
#include "VizCGraphicsPanel.h"
#include "VizCPropertyEditor.h"
#include "VizCSceneInspector.h"
#include <QMetaObject>
#include <QPointer>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
VizIUndoCommand::VizIUndoCommand(VizCWorkArea* workArea) : QObject(), QUndoCommand(), WorkArea(workArea)
{
	PropertiesAliasNames = VizCPropertiesAliasNames::instance();
	FirstTime = true;
}
//-----------------------------------------------------------------------------
VizIUndoCommand::~VizIUndoCommand()
{
}
//-----------------------------------------------------------------------------
VizCObjectChangePropertyCommand::VizCObjectChangePropertyCommand(QObject* item, const QString& propertyName, const QVariant& oldValue, const QVariant& newValue, bool needUpdateVerticalScale, VizCWorkArea* workArea) : VizIUndoCommand(workArea), Item(item), NeedUpdateVerticalScale(needUpdateVerticalScale)
{
	PropertiesNames << propertyName;
	OldValues << oldValue;
	NewValues << newValue;

	CommandAliasName = PropertiesAliasNames->propertyAliasName(propertyName);
	setText(QObject::tr("%1: %2").arg(CommandAliasName).arg(vizObjectName(Item)));
}
//-----------------------------------------------------------------------------
VizCObjectChangePropertyCommand::VizCObjectChangePropertyCommand(QObject* item, const QStringList& propertiesNames, const QVariantList& oldValues, const QVariantList& newValues, bool needUpdateVerticalScale, const QString& commandAliasName, VizCWorkArea* workArea) : VizIUndoCommand(workArea), Item(item), NeedUpdateVerticalScale(needUpdateVerticalScale), PropertiesNames(propertiesNames), OldValues(oldValues), NewValues(newValues), CommandAliasName(commandAliasName)
{
	setText(QObject::tr("%1: %2").arg(CommandAliasName).arg(vizObjectName(Item)));
}
//-----------------------------------------------------------------------------
void VizCObjectChangePropertyCommand::undo()
{

	Item->blockSignals(true);

	s32 size = PropertiesNames.size();
	for(s32 i = 0; i < size; i++)
	{
		const QString& propertyName = PropertiesNames[i];
		const QVariant& oldValue = OldValues[i];
		bool ok = Item->setProperty(qPrintable(propertyName), oldValue);
		Q_ASSERT(ok);
	}

	WorkArea->updateProperties(Item, PropertiesNames);

	Item->blockSignals(false);

	emit itemChanged(NeedUpdateVerticalScale);

}
//-----------------------------------------------------------------------------
void VizCObjectChangePropertyCommand::redo()
{
	if(FirstTime)
	{
		FirstTime = false;
		return;
	}

	Item->blockSignals(true);

	s32 size = PropertiesNames.size();
	for(s32 i = 0; i < size; i++)
	{
		const QString& propertyName = PropertiesNames[i];
		const QVariant& newValue = NewValues[i];
		bool ok = Item->setProperty(qPrintable(propertyName), newValue);
		Q_ASSERT(ok);
	}

	WorkArea->updateProperties(Item, PropertiesNames);

	Item->blockSignals(false);

	emit itemChanged(NeedUpdateVerticalScale);

}
//-----------------------------------------------------------------------------
VizCGraphicsItemAddCommand::VizCGraphicsItemAddCommand(VizIGraphicsItem* item, VizIGraphicsItem* oldParent, VizCGraphicsPanel* graphicsPanel, VizCWorkArea* workArea) : VizIUndoCommand(workArea), Item(item), OldParent(oldParent), GraphicsPanel(graphicsPanel)
{
	setText(QObject::tr("Adicionar: %2").arg(vizObjectName(item)));
}
//-----------------------------------------------------------------------------
VizCGraphicsItemAddCommand::~VizCGraphicsItemAddCommand()
{
	recursiveDeleteItems(Item);
}
//-----------------------------------------------------------------------------
void VizCGraphicsItemAddCommand::recursiveDeleteItems(VizIGraphicsItem* item)
{
	bool endRecursion = false;
	if(!item->hasChilds())
	{
		endRecursion = true;
	}

	item->deleteLater();

	if(!endRecursion)
	{
		foreach(VizIGraphicsItem* children, item->childItems())
		{
			recursiveDeleteItems(children);
		}
	}
}
//-----------------------------------------------------------------------------
void VizCGraphicsItemAddCommand::undo()
{
	GraphicsPanel->blockSignals(true);
	GraphicsPanel->removeItem(Item);
	GraphicsPanel->blockSignals(false);

	WorkArea->updateScene();
	emit itemChanged(true);
}
//-----------------------------------------------------------------------------
void VizCGraphicsItemAddCommand::redo()
{
	if(FirstTime)
	{
		FirstTime = false;
		return;
	}

	GraphicsPanel->blockSignals(true);
	GraphicsPanel->addItem(Item);
	Item->blockSignals(true);
	Q_ASSERT(!(OldParent && !GraphicsPanel->containItem(OldParent)));
	Item->setParentItem(OldParent);
	Item->blockSignals(false);
	GraphicsPanel->blockSignals(false);

	WorkArea->updateScene();
	emit itemChanged(true);

}
//-----------------------------------------------------------------------------
VizCGraphicsItemRemoveCommand::VizCGraphicsItemRemoveCommand(VizIGraphicsItem* item, VizIGraphicsItem* oldParent, VizCGraphicsPanel* graphicsPanel, VizCWorkArea* workArea) : VizIUndoCommand(workArea), Item(item), OldParent(oldParent), GraphicsPanel(graphicsPanel)
{
	setText(QObject::tr("Remover %1").arg(vizObjectName(Item)));
}
//-----------------------------------------------------------------------------
void VizCGraphicsItemRemoveCommand::undo()
{
	GraphicsPanel->blockSignals(true);
	GraphicsPanel->addItem(Item);
	Item->blockSignals(true);
	Q_ASSERT(OldParent && !GraphicsPanel->containItem(OldParent));
	Item->setParentItem(OldParent);
	Item->blockSignals(false);
	GraphicsPanel->blockSignals(false);
	
	WorkArea->updateScene();
	emit itemChanged(true);
}
//-----------------------------------------------------------------------------
void VizCGraphicsItemRemoveCommand::redo()
{
	if(FirstTime)
	{
		FirstTime = false;
		return;
	}
	GraphicsPanel->blockSignals(true);
	GraphicsPanel->removeItem(Item);
	GraphicsPanel->blockSignals(false);
	
	WorkArea->updateScene();
	emit itemChanged(true);
}
//-----------------------------------------------------------------------------
VizCGraphicsItemChangeParentCommand::VizCGraphicsItemChangeParentCommand(VizIGraphicsItem* item, VizIGraphicsItem* oldParent, VizIGraphicsItem* newParent, VizCWorkArea* workArea): VizIUndoCommand(workArea), Item(item), OldParent(oldParent), NewParent(newParent)
{
	setText(QObject::tr("Mudar Hierarquia: %1").arg(vizObjectName(item)));
}
//-----------------------------------------------------------------------------
void VizCGraphicsItemChangeParentCommand::undo()
{
	Item->blockSignals(true);
	Item->setParentItem(OldParent);
	Item->blockSignals(false);

	WorkArea->updateScene();
	emit itemChanged(false);
}
//-----------------------------------------------------------------------------
void VizCGraphicsItemChangeParentCommand::redo()
{
	if(FirstTime)
	{
		FirstTime = false;
		return;
	}

	Item->blockSignals(true);
	Item->setParentItem(NewParent);
	Item->blockSignals(false);

	WorkArea->updateScene();
	emit itemChanged(false);
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
