//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCOfferBookPanel.h"
#include "VizCSecurityWidget.h"
#include "VizMimeFormat.h"
#include "VizCWorkArea.h"
#include <QPainter>
#include <QHelpEvent>
#include <QToolTip>
#include <QHBoxLayout>
#include <QPalette>
#include <QStyle>
#include <QBitmap>
#include <QApplication>
#include <QMetaProperty>
#include <QDebug>
#include <QLabel>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
VizCOfferBookPanel::VizCOfferBookPanel(VizCSecurityWidget* parent, bool isClosable, VizCWorkArea* workArea) : VizIInnerPanel(parent, workArea)
{
	setAttribute(Qt::WA_DeleteOnClose);
	setAcceptDrops(false);
	setFocusPolicy(Qt::StrongFocus);

	TitleBar = new VizCInnerPanelTitleBar(this, isClosable);
	connect(TitleBar, SIGNAL(InnerPanelMaximizeButtonClick()), this, SLOT(slotMaximizeInnerPanelClick()));
	connect(TitleBar, SIGNAL(InnerPanelCloseButtonClick()), this, SLOT(slotCloseInnerPanelClick()));

	Dummy = new QLabel(this);
	Dummy->setText("VizCOfferBookPanel");

	QHBoxLayout* layoutH = new QHBoxLayout;
	layoutH->addWidget(Dummy);
	layoutH->setMargin(0);
	layoutH->setSpacing(0);
	setLayout(layoutH);

	Dummy->stackUnder(TitleBar);

	setProperty("name", tr("Book de Oferta"));

	ToolManager = VizCToolManager::instance();

	setMouseTracking(true);
	Dummy->setMouseTracking(true);
	TitleBar->setMouseTracking(true);

	resetToDefaults();

}
//-----------------------------------------------------------------------------
VizCOfferBookPanel::~VizCOfferBookPanel()
{
}
//-----------------------------------------------------------------------------
bool VizCOfferBookPanel::event(QEvent* event)
{
	if(event->type() == QEvent::ToolTip)
	{
		QHelpEvent* helpEvent = static_cast<QHelpEvent *>(event);
		MousePosition = helpEvent->pos();
		if(mousePositionInTitleBar(MousePosition))
		{
			QToolTip::showText(helpEvent->globalPos(), titleBarName());
		}
		else
		{
			QToolTip::showText(helpEvent->globalPos(), "");
		}
	}
	return QWidget::event(event);
}
//-----------------------------------------------------------------------------
void VizCOfferBookPanel::mouseDoubleClickEvent(QMouseEvent* event)
{
	if(event->button() != Qt::LeftButton)
	{
		return;
	}

	MousePosition = event->pos();
	if(mousePositionInTitleBar(MousePosition))
	{
		slotMaximizeInnerPanelClick();
	}
	QWidget::mouseDoubleClickEvent(event);
}
//-----------------------------------------------------------------------------
void VizCOfferBookPanel::mousePressEvent(QMouseEvent* event)
{
	if(event->button() != Qt::LeftButton)
	{
		return;
	}

	MousePosition = event->pos();
	MousePressPosition = MousePosition;
	MousePressInTitleBar = mousePositionInTitleBar(MousePressPosition);

	if(WorkArea->setSelectedVizOBject(this))
	{
		repaint();
	}

	QWidget::mousePressEvent(event);
}
//-----------------------------------------------------------------------------
void VizCOfferBookPanel::mouseMoveEvent(QMouseEvent* event)
{
	if(!(event->buttons() & Qt::LeftButton))
	{
		return;
	}
	MousePosition = event->pos();
	if((MousePosition - MousePressPosition).manhattanLength() < QApplication::startDragDistance())
	{
		return;
	}

	if(MousePressInTitleBar)
	{
		QByteArray itemData;
		QDataStream dataStream(&itemData, QIODevice::WriteOnly);

		VizCOfferBookPanel* innerPanel = qobject_cast<VizCOfferBookPanel*>(WorkArea->selectedVizOBject());
		Q_ASSERT(innerPanel == this);

		QMimeData* mimeData = new QMimeData;
		mimeData->setData(VizInnerPanelMimeFormat, itemData);

		QDrag* drag = new QDrag(this);
		drag->setMimeData(mimeData);
		Qt::DropActions dropActionIn;
		dropActionIn = Qt::MoveAction | Qt::CopyAction;
		Qt::DropAction dropActionOut = drag->exec(dropActionIn, Qt::MoveAction);
		//importante
		MousePressInTitleBar = false;
	}
	
	QWidget::mouseMoveEvent(event);
}
//-----------------------------------------------------------------------------
void VizCOfferBookPanel::mouseReleaseEvent(QMouseEvent* event)
{
	MousePressInTitleBar = false;

	QWidget::mouseReleaseEvent(event);
}
//-----------------------------------------------------------------------------
void VizCOfferBookPanel::updateData()
{
}
//-----------------------------------------------------------------------------
void VizCOfferBookPanel::resetToDefaults()
{
	VizIInnerPanel::resetToDefaults();
}
//-----------------------------------------------------------------------------
QString VizCOfferBookPanel::titleBarName() const
{
	return tr("Book de Oferta");
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------


