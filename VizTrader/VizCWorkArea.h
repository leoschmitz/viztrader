//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCWORKAREA_H__
#define __VIZCWORKAREA_H__
//-----------------------------------------------------------------------------
#include <QMdiArea>
#include <QObject>
#include "VizTypes.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCPropertyEditor;
class VizCSceneInspector;
class VizCUndoView;
class VizCGraphicsPanel;
class VizCMdiSubWindow;
class VizCUndoManager;
class VizCDataInspector;
//-----------------------------------------------------------------------------
class VizCWorkArea : public QObject
{
	Q_OBJECT

public:
	VizCWorkArea();
	~VizCWorkArea();

	QMdiArea* mdiArea();
	VizCPropertyEditor* propertyEditor();
	VizCSceneInspector* sceneInspector();
	VizCDataInspector* dataInspector();
	VizCUndoView* undoView();
	VizCUndoManager* undoManager();
	void clearSelection();
	bool setSelectedVizOBject(QObject* object);
	QObject* selectedVizOBject() const;
	bool setSelectedGraphicsPanel(VizCGraphicsPanel* panel);
	VizCGraphicsPanel* selectedGraphicsPanel() const;

	void updateActiveSubWindow();
	void updateScene();
	void updateProperty(const QObject* obj, const QString& propertyName);
	void updateProperties(const QObject* obj, const QStringList& propertiesName);
signals:
	void vizObjectAvailable();

private:

	QMdiSubWindow* ActiveSubWindow;
	QObject* SelectedVizObject;
	VizCGraphicsPanel* SelectedGraphicsPanel;
	QMdiArea* MdiArea;
	
	VizCPropertyEditor* PropertyEditor;
	VizCSceneInspector* SceneInspector;
	VizCDataInspector* DataInspector;
	VizCUndoView* UndoView;
	VizCUndoManager* UndoManager;

private slots:
	void slotVizObjectSelected(QObject* obj);
	void subWindowActivated(QMdiSubWindow* subWindow);

};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
