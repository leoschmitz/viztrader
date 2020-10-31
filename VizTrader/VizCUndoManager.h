//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCUNDOMANAGER_H__
#define __VIZCUNDOMANAGER_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include <QUndoGroup>
#include <QUndoStack>
#include <QAction>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCUndoManager : public QUndoGroup
{
public:
	VizCUndoManager();
	~VizCUndoManager();
	
	QUndoStack* newUndoStack();
	QAction* undoAction() const;
	QAction* redoAction() const;

private:
	QAction* UndoAction;
	QAction* RedoAction;
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
