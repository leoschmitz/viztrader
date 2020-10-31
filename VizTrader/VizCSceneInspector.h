//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCSCENEINSPECTOR_H__
#define __VIZCSCENEINSPECTOR_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include "VizCGraphicsPanel.h"
#include <QTreeWidget>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCSceneInspectorItem;
class VizIGraphicsItem;
//-----------------------------------------------------------------------------
class VizCSceneInspector : public QTreeWidget
{
	Q_OBJECT

public:
	VizCSceneInspector();
	~VizCSceneInspector();
	//VizCGraphicsPanel* graphicsPanel() const;

public slots:	
	void setGraphicsPanel(VizCGraphicsPanel* innerPanel);
	void sceneChanged();

signals:
	void vizObjectSelected(QObject* obj);

protected:
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void dragEnterEvent(QDragEnterEvent* event);
	virtual void dragMoveEvent(QDragMoveEvent* event);
	virtual void dropEvent(QDropEvent* event);

private:
	QTreeWidgetItem* recursiveAddItem(VizIGraphicsItem* item);
	void ajustSelectPropertyInItems();
	QPoint MousePressPosition;
	bool ItemClicked;
	VizCGraphicsPanel* GraphicsPanel;

private slots:
	void slotItemClicked(QTreeWidgetItem* item, int column);
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
