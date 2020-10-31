//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCToolManager.h"
#include "VizCGraphicsPanel.h"
#include "VizCHorizontalLine.h"
#include "VizCTrendLine.h"
#include "VizCVerticalLine.h"
#include "VizCSemiLogTrendLine.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
VizCToolManager* VizCToolManager::Instance = 0;
//-----------------------------------------------------------------------------
VizCToolManager::VizCToolManager() : QObject()
{
	resetTools();
}
//-----------------------------------------------------------------------------
VizCToolManager::~VizCToolManager()
{
}
//-----------------------------------------------------------------------------
VizCToolManager* VizCToolManager::instance()
{
	if(!Instance)
	{
		Instance = new VizCToolManager;
	}
	return Instance;
}
//-----------------------------------------------------------------------------
bool VizCToolManager::isToolSelected(VizEToolsSelectable tool) const
{
	return MapSelectable[tool];
}
//-----------------------------------------------------------------------------
bool VizCToolManager::isToolChecked(VizEToolsCheckable tool) const
{
	return MapCheckable[tool];
}
//-----------------------------------------------------------------------------
void VizCToolManager::selectTool(VizEToolsSelectable tool)
{
	resetSelected();
	MapSelectable[tool] = true;
	emit changed();
	
}
//-----------------------------------------------------------------------------
void VizCToolManager::checkTool(VizEToolsCheckable tool)
{
	MapCheckable[tool] = !MapCheckable[tool];
	emit changed();
}
//-----------------------------------------------------------------------------
void VizCToolManager::resetSelected()
{
	MapSelectable[VETS_SELECTION] = false;
	MapSelectable[VETS_HORIZONTAL_LINE] = false;
	MapSelectable[VETS_VERTICAL_LINE] = false;
	MapSelectable[VETS_TREND_LINE] = false;
	MapSelectable[VETS_SEMI_LOG_TREND_LINE] = false;
	MapSelectable[VETS_ZOOM_BOX] = false;
	MapSelectable[VETS_PAN] = false;
}
//-----------------------------------------------------------------------------
void VizCToolManager::resetChecked()
{
	MapCheckable[VETC_CROSSHAIRS] = false;
	MapCheckable[VETC_WINDOW_AUTO_TILE] = false;
}
//-----------------------------------------------------------------------------
void VizCToolManager::resetTools()
{

	resetSelected();
	resetChecked();
	MapSelectable[VETS_SELECTION] = true;

	emit changed();

}
//-----------------------------------------------------------------------------
QString VizCToolManager::addingItemName() const
{
	QString name;

	if(isToolSelected(VETS_HORIZONTAL_LINE))
	{
		name = VizCHorizontalLine::staticMetaObject.className();
	}
	else
	if(isToolSelected(VETS_TREND_LINE))
	{
		name = VizCTrendLine::staticMetaObject.className();
	}
	else
	if(isToolSelected(VETS_VERTICAL_LINE))
	{
		name = VizCVerticalLine::staticMetaObject.className();
	}
	else
	if(isToolSelected(VETS_SEMI_LOG_TREND_LINE))
	{
		name = VizCSemiLogTrendLine::staticMetaObject.className();
	}

	return name;
}
//-----------------------------------------------------------------------------
bool VizCToolManager::isAddingItem() const
{
	if(isToolSelected(VETS_HORIZONTAL_LINE))
	{
		return true;
	}
	else
	if(isToolSelected(VETS_TREND_LINE))
	{
		return true;
	}
	else
	if(isToolSelected(VETS_VERTICAL_LINE))
	{
		return true;
	}
	else
	if(isToolSelected(VETS_SEMI_LOG_TREND_LINE))
	{
		return true;
	}
		
	return false;
}
//-----------------------------------------------------------------------------
QCursor VizCToolManager::currentMouseCursor()
{
	if(isToolSelected(VETS_HORIZONTAL_LINE))
	{
		//return QCursor(Qt::BlankCursor);
		return QCursor(Qt::ArrowCursor);
	}
	else
	if(isToolSelected(VETS_TREND_LINE))
	{
		return QCursor(Qt::CrossCursor);
	}
	else
	if(isToolSelected(VETS_VERTICAL_LINE))
	{
		return QCursor(Qt::CrossCursor);
	}
	else
	{
		return QCursor(Qt::ArrowCursor);
	}
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
