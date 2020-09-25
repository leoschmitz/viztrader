//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCEDITACTIONS_H__
#define __VIZCEDITACTIONS_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include <QAction>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCEditActions
{
public:
	static VizCEditActions* instance();

	QAction* UndoAction;
	QAction* RedoAction;
	QAction* CutAction;
	QAction* CopyAction;
	QAction* PasteAction;
	QAction* DeleteAction;


private:
	VizCEditActions();
	~VizCEditActions();

	static VizCEditActions* Instance;
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
