//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCWorkArea.h"
#include "VizCPropertyEditor.h"
#include "VizCSceneInspector.h"
#include "VizCUndoView.h"
#include "VizCGraphicsPanel.h"
#include "VizObjectUtils.h"
#include "VizCMdiSubWindow.h"
#include "VizCDataInspector.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
VizCWorkArea::VizCWorkArea() : QObject()
{
	PropertyEditor = new VizCPropertyEditor;
	SceneInspector = new VizCSceneInspector;
	DataInspector = new VizCDataInspector;
	UndoView = new VizCUndoView;
	MdiArea = new QMdiArea;
	UndoManager = new VizCUndoManager;
	ActiveSubWindow = 0;

	connect(SceneInspector, SIGNAL(vizObjectSelected(QObject*)), this, SLOT(slotVizObjectSelected(QObject*)));
	connect(MdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(subWindowActivated(QMdiSubWindow*)));

	//MdiArea->setWindowTitle(key);
	MdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	MdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	SelectedVizObject = 0;
	SelectedGraphicsPanel = 0;

}
//-----------------------------------------------------------------------------
VizCWorkArea::~VizCWorkArea()
{
	clearSelection();

	PropertyEditor->disconnect();
	SceneInspector->disconnect();
	UndoView->disconnect();
	MdiArea->disconnect();

	PropertyEditor->deleteLater();
	SceneInspector->deleteLater();
	UndoView->deleteLater();
	MdiArea->deleteLater();
	
}
//-----------------------------------------------------------------------------
void VizCWorkArea::clearSelection()
{
	PropertyEditor->setObject(0);
	SceneInspector->setGraphicsPanel(0);
	UndoView->setStack(0);

	if(SelectedVizObject)
	{
		vizObjectSetSelected(SelectedVizObject, false);
	}

	SelectedVizObject = 0;
	SelectedGraphicsPanel = 0;
}
//-----------------------------------------------------------------------------
bool VizCWorkArea::setSelectedVizOBject(QObject* object)
{

	Q_ASSERT(!(object && !isVizObject(object)));

	if(SelectedVizObject == object)
	{
		emit vizObjectAvailable();
		return false;
	}
	//tira o selected do atual
	if(SelectedVizObject)
	{
		vizObjectSetSelected(SelectedVizObject, false);
	}
	if(!object)
	{
		SelectedVizObject = 0;
		PropertyEditor->setObject(0);
		emit vizObjectAvailable();
		return true;
	}

	SelectedVizObject = object;
	//coloca o selected no atual
	if(SelectedVizObject)
	{
		vizObjectSetSelected(SelectedVizObject, true);
	}

	PropertyEditor->setObject(SelectedVizObject);
	emit vizObjectAvailable();
	return true;

}
//-----------------------------------------------------------------------------
bool VizCWorkArea::setSelectedGraphicsPanel(VizCGraphicsPanel* panel)
{

	if(SelectedGraphicsPanel == panel)
	{
		return false;
	}
	else
	{
		SelectedGraphicsPanel = panel;
	}

	SceneInspector->setGraphicsPanel(SelectedGraphicsPanel);

	return true;

}
//-----------------------------------------------------------------------------
void VizCWorkArea::updateActiveSubWindow()
{
	MdiArea->setActiveSubWindow(ActiveSubWindow);
}
//-----------------------------------------------------------------------------
VizCGraphicsPanel* VizCWorkArea::selectedGraphicsPanel() const
{
	return SelectedGraphicsPanel;
}
//-----------------------------------------------------------------------------
QObject* VizCWorkArea::selectedVizOBject() const
{
	return SelectedVizObject;
}
//-----------------------------------------------------------------------------
void VizCWorkArea::updateScene()
{
	SceneInspector->sceneChanged();
}
//-----------------------------------------------------------------------------
void VizCWorkArea::updateProperty(const QObject* obj, const QString& propertyName)
{
	PropertyEditor->propertyChanged(obj, propertyName);
}
//-----------------------------------------------------------------------------
void VizCWorkArea::updateProperties(const QObject* obj, const QStringList& propertiesName)
{
	PropertyEditor->propertiesChanged(obj, propertiesName);
}
//-----------------------------------------------------------------------------
QMdiArea* VizCWorkArea::mdiArea()
{
	return MdiArea;
}
//-----------------------------------------------------------------------------
VizCPropertyEditor* VizCWorkArea::propertyEditor()
{
	return PropertyEditor;
}
//-----------------------------------------------------------------------------
VizCSceneInspector* VizCWorkArea::sceneInspector()
{
	return SceneInspector;
}
//-----------------------------------------------------------------------------
VizCDataInspector* VizCWorkArea::dataInspector()
{
	return DataInspector;
}
//-----------------------------------------------------------------------------
VizCUndoView* VizCWorkArea::undoView()
{
	return UndoView;
}
//-----------------------------------------------------------------------------
VizCUndoManager* VizCWorkArea::undoManager()
{
	return UndoManager;
}

//-----------------------------------------------------------------------------
void VizCWorkArea::slotVizObjectSelected(QObject* obj)
{
	setSelectedVizOBject(obj);
}
//-----------------------------------------------------------------------------
void VizCWorkArea::subWindowActivated(QMdiSubWindow* subWindow)
{
	if(!subWindow)
		return;
	ActiveSubWindow = subWindow;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
