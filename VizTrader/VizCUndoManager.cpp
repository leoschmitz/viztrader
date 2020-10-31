//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCUndoManager.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
VizCUndoManager::VizCUndoManager() : QUndoGroup()
{
	UndoAction = createUndoAction(this);
	//UndoAction = createUndoAction(this, tr("Desfa&zer"));
	//UndoAction->setIcon(QIcon(":/VizTrader/Resources/editundo.png"));
	//UndoAction->setShortcut(QKeySequence("Ctrl+Z"));

	RedoAction = createRedoAction(this);
	//RedoAction = createRedoAction(this, tr("Re&fazer"));
	//RedoAction->setIcon(QIcon(":/VizTrader/Resources/editredo.png"));
	//RedoAction->setShortcut(QKeySequence("Shift+Ctrl+Z"));
}
//-----------------------------------------------------------------------------
VizCUndoManager::~VizCUndoManager()
{
}
//-----------------------------------------------------------------------------
QUndoStack* VizCUndoManager::newUndoStack()
{
	QUndoStack* stack = new QUndoStack;
	addStack(stack);
	return stack;
}
//-----------------------------------------------------------------------------
QAction* VizCUndoManager::undoAction() const
{
	return UndoAction;
}
//-----------------------------------------------------------------------------
QAction* VizCUndoManager::redoAction() const
{
	return RedoAction;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
