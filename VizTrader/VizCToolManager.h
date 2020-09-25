//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCTOOLMANAGER_H__
#define __VIZCTOOLMANAGER_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include <QMap>
#include <QList>
#include <QPointF>
#include <QCursor>
#include <QObject>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCGraphicsPanel;
//-----------------------------------------------------------------------------
enum VizEToolsSelectable
{
	VETS_SELECTION = 0,
	VETS_HORIZONTAL_LINE,
	VETS_VERTICAL_LINE,
	VETS_TREND_LINE,
	VETS_SEMI_LOG_TREND_LINE,
	VETS_ZOOM_BOX,
	VETS_PAN
};
//-----------------------------------------------------------------------------
enum VizEToolsCheckable
{
	VETC_CROSSHAIRS = 0,
	VETC_WINDOW_AUTO_TILE
};
//-----------------------------------------------------------------------------
class VizCToolManager : public QObject
{
	Q_OBJECT
public:
	static VizCToolManager* instance();

	QString addingItemName() const;
	bool isAddingItem() const;
	QCursor currentMouseCursor();
	bool isToolSelected(VizEToolsSelectable tool) const;
	bool isToolChecked(VizEToolsCheckable tool) const;
	void selectTool(VizEToolsSelectable tool);
	void checkTool(VizEToolsCheckable tool);
	void resetTools();

signals:
	void changed();

private:
	VizCToolManager();
	~VizCToolManager();
	void resetSelected();
	void resetChecked();

	QMap<VizEToolsSelectable, bool> MapSelectable;
	QMap<VizEToolsCheckable, bool> MapCheckable;

	static VizCToolManager* Instance;

};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
