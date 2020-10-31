//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZIINNERPANEL_H__
#define __VIZIINNERPANEL_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include "VizCInnerPanelTitleBar.h"
#include "VizCToolManager.h"
#include "VizCWorkArea.h"
#include "VizCSettings.h"
#include "VizCClipboard.h"
#include "VizCUndoManager.h"
#include "VizCUndoCommands.h"
#include "VizCEditActions.h"
#include <QMenu>
#include <QUndoStack>
#include <QWidget>
#include <QToolButton>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizIInnerPanel : public QWidget
{
	Q_OBJECT

	Q_PROPERTY(bool titleBarVisible READ isTitleBarVisible WRITE setTitleBarVisible USER true)
	Q_PROPERTY(bool selected READ isSelected WRITE setSelected)

public:
	VizIInnerPanel(VizCSecurityWidget* parent, VizCWorkArea* workArea);
	~VizIInnerPanel();

	Q_INVOKABLE virtual void resetToDefaults();
	virtual QString titleBarName() const; //?
	virtual bool isMaximized() const;
	bool isSelected() const;
	void setSelected(bool selected);
	bool isTitleBarVisible() const;
	void setTitleBarVisible(bool value);
	bool isClosable() const;
	QSize minimumSizeHint() const;
	QSize sizeHint() const;
	void setButtonsActive(bool activated);
	void setLoadingState(bool value);

protected:
	virtual bool event(QEvent* event) = 0;
	virtual void paintEvent(QPaintEvent* event);
	virtual void mouseDoubleClickEvent(QMouseEvent* event) = 0;
	virtual void mousePressEvent(QMouseEvent* event) = 0;
	virtual void mouseMoveEvent(QMouseEvent* event) = 0;
	virtual void mouseReleaseEvent(QMouseEvent* event) = 0;
	virtual void updateData() = 0;
	virtual bool isAnyThingSelected() const;
	bool mousePositionInTitleBar(const QPoint& p);

	VizCInnerPanelTitleBar* TitleBar;
	VizCToolManager* ToolManager;
	io::VizCSettings* Settings;
	VizCClipboard* Clipboard;
	VizCSecurityWidget* Parent;
	bool TitleBarVisible;
	bool IsSelected;

	QPoint MousePosition;
	QPoint MousePressPosition;
	bool MousePressInTitleBar;

	VizCWorkArea* WorkArea;

	VizCUndoManager* UndoManager;
	QUndoStack* UndoStack;

	QMenu* EditMenu;
	VizCEditActions* EditActions;

protected slots:
	void showMaximized();
	void showNormal();
	void slotCloseInnerPanelClick();
	void slotMaximizeInnerPanelClick();

private:
	void createEditMenu();
	
	friend class VizCSecurityWidget;
	friend class VizCInnerPanelTitleBar;

signals:
	void createUndoCommandPropertyChanged(QObject* obj, const QString& propertyName, const QVariant& oldValue, const QVariant& newValue, bool needUpdateVerticalScale);
	void createUndoCommandPropertiesChanged(QObject* obj, const QStringList& propertiesNames, const QVariantList& oldValues, const QVariantList& newValues, bool needUpdateVerticalScale, const QString& commandAliasName);
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
