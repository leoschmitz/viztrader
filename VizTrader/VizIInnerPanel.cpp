//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizIInnerPanel.h"
#include "VizCSecurityWidget.h"
#include "VizIGraphicsItem.h"
#include "VizMath.h"
#include <QPainter>
#include <QHBoxLayout>
#include <QPalette>
#include <QStyle>
#include <QBitmap>
#include <QApplication>
#include <QMetaProperty>
#include <QDebug>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
VizIInnerPanel::VizIInnerPanel(VizCSecurityWidget* parent, VizCWorkArea* workArea) : QWidget(parent), TitleBar(0), Parent(parent), IsSelected(false), MousePressInTitleBar(false), Clipboard(0), WorkArea(workArea)
{
	Q_ASSERT(Parent);
	Q_ASSERT(WorkArea);

//	vizObjectSetName(this, tr(""));
//	vizObjectSetSelected(this, false);


	Clipboard = VizCClipboard::instance();
	ToolManager = VizCToolManager::instance();
	Settings = io::VizCSettings::instance();
	EditActions = VizCEditActions::instance();
	
	UndoManager = WorkArea->undoManager();
	UndoStack = UndoManager->newUndoStack();
	
	createEditMenu();

	resetToDefaults();

}
//-----------------------------------------------------------------------------
VizIInnerPanel::~VizIInnerPanel()
{
}
//-----------------------------------------------------------------------------
void VizIInnerPanel::resetToDefaults()
{
	TitleBarVisible = true;
	if(TitleBar)
	{
		TitleBar->setVisible(TitleBarVisible);
	}
}
//-----------------------------------------------------------------------------
QString VizIInnerPanel::titleBarName() const
{
	return tr("Painel Interno Vazio");
}
//-----------------------------------------------------------------------------
void VizIInnerPanel::setButtonsActive(bool activated)
{
	TitleBar->setButtonsActive(activated);
}
//-----------------------------------------------------------------------------
bool VizIInnerPanel::isTitleBarVisible() const
{
	return TitleBarVisible;
}
//-----------------------------------------------------------------------------
void VizIInnerPanel::setTitleBarVisible(bool value)
{
	if(TitleBarVisible != value)
	{
		TitleBarVisible = value;
		TitleBar->setVisible(TitleBarVisible);
		//repaint();
	}
}
//-----------------------------------------------------------------------------
bool VizIInnerPanel::isSelected() const
{
	return IsSelected;
}
//-----------------------------------------------------------------------------
void VizIInnerPanel::setSelected(bool selected)
{
	if(IsSelected != selected)
	{
		IsSelected = selected;
		TitleBar->repaint();
		//repaint();
	}
}
//-----------------------------------------------------------------------------
QSize VizIInnerPanel::minimumSizeHint() const
{
	return QSize(50, TitleBar->minimumSizeHint().height());
}
//-----------------------------------------------------------------------------
QSize VizIInnerPanel::sizeHint() const
{
	return QSize(50, TitleBar->sizeHint().height());
}
//-----------------------------------------------------------------------------
bool VizIInnerPanel::isClosable() const
{
	return TitleBar->isClosable();
}
//-----------------------------------------------------------------------------
bool VizIInnerPanel::isAnyThingSelected() const
{
	if(IsSelected)
	{
		return true;
	}
	return false;
}
//-----------------------------------------------------------------------------
bool VizIInnerPanel::isMaximized() const
{
	return TitleBar->isMaximized();
}
//-----------------------------------------------------------------------------
void VizIInnerPanel::showMaximized()
{
	TitleBar->setButtonsState(true);
	QWidget::showMaximized();
}
//-----------------------------------------------------------------------------
void VizIInnerPanel::showNormal()
{
	TitleBar->setButtonsState(false);
	QWidget::showNormal();
}
//-----------------------------------------------------------------------------
void VizIInnerPanel::slotCloseInnerPanelClick()
{
	if(isMaximized())
	{
		showNormal();
		Parent->showMaximizeInnerPanel(this);
	}
	if(isAnyThingSelected())
	{
		WorkArea->setSelectedVizOBject(0);
	}
	Parent->closeInnerPanel(this);
}
//-----------------------------------------------------------------------------
void VizIInnerPanel::slotMaximizeInnerPanelClick()
{
	if(!TitleBar->isButtonsActive())
	{
		return;
	}

	if(isMaximized())
	{
		showNormal();
	}
	else
	{
		showMaximized();
	}

	Parent->showMaximizeInnerPanel(this);
	
	
	if(!isAnyThingSelected())
	{
		WorkArea->setSelectedVizOBject(this);
	}
}
//-----------------------------------------------------------------------------
bool VizIInnerPanel::mousePositionInTitleBar(const QPoint& p)
{
	return TitleBarVisible && p.y() <= TitleBar->titleBarHeight() && !TitleBar->buttonsContains(p);
}
//-----------------------------------------------------------------------------
void VizIInnerPanel::paintEvent(QPaintEvent* event)
{
	TitleBar->setGeometry(TitleBar->x(), TitleBar->y(), geometry().width(), TitleBar->titleBarHeight());

}
//-----------------------------------------------------------------------------
void VizIInnerPanel::createEditMenu()
{

	EditMenu = new QMenu;

	EditMenu->addAction(EditActions->UndoAction);
	EditMenu->addAction(EditActions->RedoAction);
	EditMenu->addSeparator();
	EditMenu->addAction(EditActions->CutAction);
	EditMenu->addAction(EditActions->CopyAction);
	EditMenu->addAction(EditActions->PasteAction);
	EditMenu->addAction(EditActions->DeleteAction);


}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------


