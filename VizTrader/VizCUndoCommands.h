//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCUNDOCOMMANDS_H__
#define __VIZCUNDOCOMMANDS_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include "VizIGraphicsItemBasedOnSecurity.h"
#include "VizIGraphicsItemBasedOnShape.h"
#include "VizCPropertiesAliasNames.h"
#include <QUndoCommand>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCGraphicsPanel;
class VizCWorkArea;
class VizCPropertiesAliasNames;
//-----------------------------------------------------------------------------
class VizIUndoCommand : public QObject, public QUndoCommand
{
	Q_OBJECT
public:
	VizIUndoCommand(VizCWorkArea* workArea);
	virtual ~VizIUndoCommand();

signals:
	void itemChanged(bool needUpdateVerticalScale);

protected:
	bool FirstTime;
	VizCWorkArea* WorkArea;
	VizCPropertiesAliasNames* PropertiesAliasNames;
};
//-----------------------------------------------------------------------------
class VizCGraphicsItemAddCommand : public VizIUndoCommand
{
	Q_OBJECT
public:
	VizCGraphicsItemAddCommand(VizIGraphicsItem* item, VizIGraphicsItem* oldParent, VizCGraphicsPanel* graphicsPanel, VizCWorkArea* workArea);
	~VizCGraphicsItemAddCommand();

	virtual void undo();
	virtual void redo();

private:
	VizIGraphicsItem* Item;
	VizIGraphicsItem* OldParent;
	VizCGraphicsPanel* GraphicsPanel;

	void recursiveDeleteItems(VizIGraphicsItem* item);
	
};
//-----------------------------------------------------------------------------
class VizCGraphicsItemRemoveCommand : public VizIUndoCommand
{
	Q_OBJECT
public:
	VizCGraphicsItemRemoveCommand(VizIGraphicsItem* item, VizIGraphicsItem* oldParent, VizCGraphicsPanel* graphicsPanel, VizCWorkArea* workArea);

	virtual void undo();
	virtual void redo();

private:
	VizIGraphicsItem* Item;
	VizIGraphicsItem* OldParent;
	VizCGraphicsPanel* GraphicsPanel;
};
//-----------------------------------------------------------------------------
class VizCObjectChangePropertyCommand : public VizIUndoCommand
{
	Q_OBJECT
public:
	VizCObjectChangePropertyCommand(QObject* item, const QString& propertyName, const QVariant& oldValue, const QVariant& newValue, bool needUpdateVerticalScale, VizCWorkArea* workArea);
	VizCObjectChangePropertyCommand(QObject* item, const QStringList& propertiesNames, const QVariantList& oldValues, const QVariantList& newValues, bool needUpdateVerticalScale, const QString& commandAliasName, VizCWorkArea* workArea);

	virtual void undo();
	virtual void redo();

private:
	bool NeedUpdateVerticalScale;
	QObject* Item;
	QStringList PropertiesNames;
	QVariantList OldValues;
	QVariantList NewValues;
	QString CommandAliasName;
};

//-----------------------------------------------------------------------------
class VizCGraphicsItemChangeParentCommand : public VizIUndoCommand
{
	Q_OBJECT
public:
	VizCGraphicsItemChangeParentCommand(VizIGraphicsItem* item, VizIGraphicsItem* oldParent, VizIGraphicsItem* newParent, VizCWorkArea* workArea);

	virtual void undo();
	virtual void redo();

private:
	VizIGraphicsItem* Item;
	VizIGraphicsItem* OldParent;
	VizIGraphicsItem* NewParent;
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
