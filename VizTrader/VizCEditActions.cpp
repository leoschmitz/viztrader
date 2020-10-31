//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCEditActions.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
VizCEditActions* VizCEditActions::Instance = 0;
//-----------------------------------------------------------------------------
VizCEditActions* VizCEditActions::instance()
{
	if(!Instance)
	{
		Instance = new VizCEditActions;
	}

	return Instance;
}
//-----------------------------------------------------------------------------
VizCEditActions::VizCEditActions() :
UndoAction(0),
RedoAction(0),
CutAction(0),
CopyAction(0),
PasteAction(0),
DeleteAction(0)
{
}
//-----------------------------------------------------------------------------
VizCEditActions::~VizCEditActions()
{
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
