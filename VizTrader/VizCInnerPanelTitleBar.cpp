//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCInnerPanelTitleBar.h"
#include "VizIInnerPanel.h"
#include "VizMath.h"
#include <QPainter>
#include <QHBoxLayout>
#include <QPalette>
#include <QStyle>
#include <QBitmap>
#include <QApplication>
#include <QMetaProperty>
#include <QDebug>
#include <QTextDocument>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
const s32 TitleBarBorder = 10;
const f64 MinAlpha = 0.0;
const f64 MaxAlpha = 0.5;
const f64 TimeTick = 50;
const f64 ButtonAutoHideTimeHide = 1500;
const f64 ButtonAutoHideTimeShow = 150;
const f64 ButtonAutoHideTimeHideAlphaInc = (MaxAlpha - MinAlpha) / (ButtonAutoHideTimeHide / TimeTick);
const f64 ButtonAutoHideTimeShowAlphaInc = (MaxAlpha - MinAlpha) / (ButtonAutoHideTimeShow / TimeTick);
//-----------------------------------------------------------------------------
VizCInnerPanelTitleBar::VizCInnerPanelTitleBar(VizIInnerPanel* parent, bool isClosable) : QWidget(parent), IsClosable(isClosable), Parent(parent), IsMaximized(false), IsTitleButtonsActive(true)
{

	ButtonAutoHideAlpha = MaxAlpha;
	IsVisible = false;
	ButtonAutoHideInc = false;

	QPalette pal = palette();
	TitleBackgroundColor1 = pal.color(QPalette::Window);
	TitleBackgroundColor2 = QColor(0,0,0,0);
	TitleTextColor = pal.color(QPalette::Text);
	TitleBackgroundColorIsSelected = pal.color(QPalette::Highlight);
	TitleTextColorIsSelected = pal.color(QPalette::HighlightedText);
	
	/*
	TitleBackgroundColor1 = QColor::fromRgbF(0.941176, 0.941176, 0.941176, 1);
	TitleBackgroundColor2 = QColor::fromRgbF(0, 0, 0, 0);
	TitleTextColor = QColor::fromRgbF(0, 0, 0, 1);
	TitleBackgroundColorIsSelected = QColor::fromRgbF(0.2, 0.6, 1, 1);
	TitleTextColorIsSelected = QColor::fromRgbF(1, 1, 1, 1);
	*/

	
	setAttribute(Qt::WA_DeleteOnClose);
	Q_ASSERT(parent);
	Parent = parent;
	setupTitleBar();

	ajustButtonsAlphaChannel(ButtonAutoHideAlpha);

	QWidget::setVisible(true);

	ButtonAutoHideTimer = new QTimer(this);

	connect(ButtonAutoHideTimer, SIGNAL(timeout()), this, SLOT(computeButtonAutoHideAlphaChannel()));

	ButtonAutoHideTimer->start(TimeTick);


	setFocusPolicy(Qt::NoFocus);

}
//-----------------------------------------------------------------------------
VizCInnerPanelTitleBar::~VizCInnerPanelTitleBar()
{
	ButtonAutoHideTimer->stop();
}
//-----------------------------------------------------------------------------
void VizCInnerPanelTitleBar::computeButtonAutoHideAlphaChannel()
{
	if(ButtonAutoHideInc)
	{
		ButtonAutoHideAlpha += ButtonAutoHideTimeShowAlphaInc;
		if(ButtonAutoHideAlpha > MaxAlpha || core::equals(ButtonAutoHideAlpha, MaxAlpha))
		{
			ButtonAutoHideInc = false;
			ButtonAutoHideAlpha = MaxAlpha;
			ButtonAutoHideTimer->stop();
		}
	}
	else
	{
		ButtonAutoHideAlpha -= ButtonAutoHideTimeHideAlphaInc;
		if(ButtonAutoHideAlpha < MinAlpha || core::equals(ButtonAutoHideAlpha, MinAlpha))
		{
			//ButtonAutoHideInc = true;
			ButtonAutoHideAlpha = MinAlpha;
			ButtonAutoHideTimer->stop();
		}
	}

	ajustButtonsAlphaChannel(ButtonAutoHideAlpha);
	Parent->repaint();

}
//-----------------------------------------------------------------------------
void VizCInnerPanelTitleBar::enterEvent(QEvent* event)
{
	ButtonAutoHideInc = true;
	ButtonAutoHideTimer->stop();
	ButtonAutoHideTimer->start(TimeTick);

	QWidget::enterEvent(event);
}
//-----------------------------------------------------------------------------
void VizCInnerPanelTitleBar::leaveEvent(QEvent* event)
{
	
	ButtonAutoHideInc = false;
	ButtonAutoHideTimer->stop();
	ButtonAutoHideTimer->start(TimeTick);

	QWidget::leaveEvent(event);
}
//-----------------------------------------------------------------------------
void VizCInnerPanelTitleBar::setVisible(bool value)
{
	if(value != IsVisible)
	{
		IsVisible = value;
		Parent->repaint();
	}
}
//-----------------------------------------------------------------------------
bool VizCInnerPanelTitleBar::buttonsContains(const QPoint& pos) const
{
	return CloseButton->geometry().contains(pos) || MaximizeButton->geometry().contains(pos);
}
//-----------------------------------------------------------------------------
bool VizCInnerPanelTitleBar::isMaximized() const
{
	return IsMaximized;
}
//-----------------------------------------------------------------------------
QSize VizCInnerPanelTitleBar::minimumSizeHint() const
{
	return QSize(50, titleBarHeight());
}
//-----------------------------------------------------------------------------
QSize VizCInnerPanelTitleBar::sizeHint() const
{
	return QSize(50, titleBarHeight());
}
//-----------------------------------------------------------------------------
bool VizCInnerPanelTitleBar::isClosable() const
{
	return IsClosable;
}
//-----------------------------------------------------------------------------
s32 VizCInnerPanelTitleBar::titleBarHeight() const
{
	s32 h = qMax(16, fontMetrics().height() + TitleBarBorder);
	return h;
}
//-----------------------------------------------------------------------------
void VizCInnerPanelTitleBar::paintEvent(QPaintEvent* event)
{

	QPainter painter(this);

	//setFont(painter.font());

	f64 viewport_width = viewportWidth();
	s32 height = titleBarHeight();
	QRectF Rect(0, 0, viewport_width, height);
	const f64 size = 16.0;


	if(IsVisible)
	{
		QString name = Parent->titleBarName();
		QLinearGradient gradient(0, height - TitleBarBorder, 0, height);
		bool isSelected = Parent->isSelected();
		gradient.setColorAt(0.0, isSelected ? TitleBackgroundColorIsSelected : TitleBackgroundColor1);
		gradient.setColorAt(1.0, TitleBackgroundColor2);
		painter.setBrush(gradient);
		painter.setPen(Qt::NoPen);
		painter.drawRect(Rect);
		
		/*painter.setPen(isSelected ? TitleTextColorIsSelected : TitleTextColor);
		QTextOption to(Qt::AlignVCenter);
		QRectF _r = Rect;
		_r.setX(_r.x() + 5);
		_r.setWidth(_r.width() - size * 2);
		name = fontMetrics().elidedText(name, Qt::ElideRight, static_cast<s32>(_r.width()));
		painter.drawText(_r, name, to);
		*/

		QColor c = isSelected ? TitleTextColorIsSelected : TitleTextColor;
		QRectF _r = Rect;
		_r.setWidth(_r.width() - (size * 3));
		QTextDocument td;

		td.setDefaultStyleSheet(QString("span {color: %1;}").arg(c.name()));
		name = "<span>" + name + "</span>";;
		td.setHtml(name);
		td.drawContents(&painter, _r);
		
	}

	f64 y = (Rect.height() * 0.5 - size * 0.5);
	CloseButton->setGeometry(viewport_width - size - 5, y, size, size);
	MaximizeButton->setGeometry(CloseButton->x() - size, y, size, size);

}
//-----------------------------------------------------------------------------
void VizCInnerPanelTitleBar::ajustAlphaChannel(QPixmap& pix, f64 alphaValue)
{
	QPixmap alpha(pix.size());
	alpha.fill(Qt::transparent);
	QPainter painter(&alpha);
	QColor color(0,0,0,0);
	color.setAlphaF(alphaValue);
	painter.fillRect(0, 0, alpha.width(), alpha.height(), color);
	pix.setAlphaChannel(alpha.alphaChannel());

}
//-----------------------------------------------------------------------------
void VizCInnerPanelTitleBar::setupTitleBar()
{
	QPalette pal = palette();

	CloseButton = new QToolButton(this);
	CloseButton->setCursor(Qt::ArrowCursor);
	CloseButton->setAutoRaise(true);
	CloseButton->setToolTip(tr("Fechar"));
	CloseButton->setEnabled(IsClosable);
	CloseButton->setAttribute(Qt::WA_TranslucentBackground);
	pal.setBrush(CloseButton->backgroundRole(), Qt::transparent);
	CloseButton->setPalette(pal);
	
	MaximizeButton = new QToolButton(this);
	MaximizeButton->setCursor(Qt::ArrowCursor);
	MaximizeButton->setAutoRaise(true);
	MaximizeButton->setToolTip(tr("Maximizar"));
	MaximizeButton->setAttribute(Qt::WA_TranslucentBackground);
	pal.setBrush(MaximizeButton->backgroundRole(), Qt::transparent);
	MaximizeButton->setPalette(pal);

	connect(CloseButton, SIGNAL(clicked()), this, SIGNAL(InnerPanelCloseButtonClick()));
	connect(MaximizeButton, SIGNAL(clicked()), this, SIGNAL(InnerPanelMaximizeButtonClick()));

	ajustTitleBarAlphaChannel(MaxAlpha);

}
//-----------------------------------------------------------------------------
void VizCInnerPanelTitleBar::setButtonsActive(bool actived)
{
	IsTitleButtonsActive = actived;
	MaximizeButton->setDisabled(!IsTitleButtonsActive);
	if(IsClosable)
	{
		CloseButton->setDisabled(!IsTitleButtonsActive);
	}
}
//-----------------------------------------------------------------------------
f64 VizCInnerPanelTitleBar::viewportWidth() const
{
	return Parent->geometry().width();
}
//-----------------------------------------------------------------------------
bool VizCInnerPanelTitleBar::isButtonsActive() const
{
	return IsTitleButtonsActive;
}
//-----------------------------------------------------------------------------
void VizCInnerPanelTitleBar::setButtonsState(bool maximized)
{
	if(maximized)
	{
		IsMaximized = true;
		MaximizeButton->setIcon(MaximizeButtonPixmap[1]);
		MaximizeButton->setToolTip(tr("Minimizar"));
	}
	else
	{
		IsMaximized = false;
		MaximizeButton->setIcon(MaximizeButtonPixmap[0]);
		MaximizeButton->setToolTip(tr("Maximizar"));
	}

	ButtonAutoHideInc = false;
	ButtonAutoHideTimer->start(TimeTick);
}
//-----------------------------------------------------------------------------
void VizCInnerPanelTitleBar::ajustTitleBarAlphaChannel(f64 value)
{
	
	TitleBackgroundColor1.setAlphaF(value);
	TitleBackgroundColorIsSelected.setAlphaF(value);
	//TitleTextColor.setAlphaF(IsVisible ? 1.0 : 0.0);
	
	TitleTextColor.setAlphaF(value * 2.0);
	TitleTextColorIsSelected.setAlphaF(value * 2.0);
}
//-----------------------------------------------------------------------------
void VizCInnerPanelTitleBar::ajustButtonsAlphaChannel(f64 value)
{
	ClosePixmap = style()->standardIcon(QStyle::SP_TitleBarCloseButton).pixmap(QSize(16, 16));
	MaximizeButtonPixmap[1] = style()->standardIcon(QStyle::SP_TitleBarNormalButton).pixmap(QSize(16, 16));
	MaximizeButtonPixmap[0] = style()->standardIcon(QStyle::SP_TitleBarMaxButton).pixmap(QSize(16, 16));

	ajustAlphaChannel(ClosePixmap, value * 1.5);
	ajustAlphaChannel(MaximizeButtonPixmap[0], value * 1.5);
	ajustAlphaChannel(MaximizeButtonPixmap[1], value * 1.5);
	
	
	CloseButton->setIcon(ClosePixmap);
	if(IsMaximized)
	{
		MaximizeButton->setIcon(MaximizeButtonPixmap[1]);
	}
	else
	{
		MaximizeButton->setIcon(MaximizeButtonPixmap[0]);
	}
	
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
