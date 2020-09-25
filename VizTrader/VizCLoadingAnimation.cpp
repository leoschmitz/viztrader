//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCLoadingAnimation.h"
#include "VizMath.h"
#include "VizCSettings.h"
#include <QDebug>
#include <QPainter>
#include <QApplication>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
const s32 InnerCircleRadius = 12;
const s32 OuterCircleRadius = 22;
/*
static const s32 MinAlpha = 80;
static const s32 MaxAlpha = 230;
*/
const s32 MinAlpha = 50;
const s32 MaxAlpha = 180;
const f32 StepAngle = 360.0f / 8.0f;
const f32 StepAlpha = (MaxAlpha-MinAlpha) / 8.0f;
//-----------------------------------------------------------------------------
VizCLoadingAnimation::VizCLoadingAnimation(QWidget* parent) : QWidget(parent)
{
	setFocusPolicy(Qt::NoFocus);
	Alpha = MaxAlpha;
	Timer = new QTimer(this);
	connect(Timer, SIGNAL(timeout()), this, SLOT(updateAlpha()));
	setFixedSize(QSize(OuterCircleRadius*2+(InnerCircleRadius)+2, OuterCircleRadius*2+(InnerCircleRadius)+2));
}
//-----------------------------------------------------------------------------
VizCLoadingAnimation::~VizCLoadingAnimation()
{
}
//-----------------------------------------------------------------------------
void VizCLoadingAnimation::setPercentageLoaded(s32 percent)
{
	PercentageLoaded = percent;
}
//-----------------------------------------------------------------------------
void VizCLoadingAnimation::startAnimation()
{
	PercentageLoaded = 0;
	setVisible(true);
	Timer->start(100);

}
//-----------------------------------------------------------------------------
void VizCLoadingAnimation::stopAnimation()
{
	setVisible(false);
	Timer->stop();
}
//-----------------------------------------------------------------------------
void VizCLoadingAnimation::updateAlpha()
{
	Alpha -= StepAlpha;
	if(Alpha < MinAlpha)
	{
		Alpha = MaxAlpha;
		Alpha -= StepAlpha;
	}

	repaint();

}
//-----------------------------------------------------------------------------
void VizCLoadingAnimation::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.translate(width()*0.5f, height()*0.5f);

	QColor black = Qt::black;
	QColor white = Qt::white;

	for(f32 i = 0, alpha = Alpha; i < 360; i += StepAngle, alpha -= StepAlpha)
	{
		f32 angle = core::degToRad(i);
		f32 x = cosf(angle) * OuterCircleRadius-(InnerCircleRadius*0.5f);
		f32 y = sinf(angle) * OuterCircleRadius-(InnerCircleRadius*0.5f);

		if(alpha < MinAlpha)
		{
			alpha = MaxAlpha;
		}

		white.setAlpha(alpha);
		black.setAlpha(alpha);
		painter.setPen(black);
		painter.setBrush(white);
		painter.drawEllipse(x, y, InnerCircleRadius, InnerCircleRadius);

	}
	painter.setCompositionMode(QPainter::CompositionMode_Difference);

	QFont defaultFont;
	defaultFont.fromString(io::VizCSettings::instance()->value("DefaultFont", QApplication::font()).toString());

	//defaultFont.setPointSizeF(6);

	painter.setFont(defaultFont);

	
	QString s;
	s.sprintf("%.2d%", PercentageLoaded);
	white.setAlpha(MaxAlpha);
	painter.setPen(white);
	painter.drawText(-width()*0.5f,-height()*0.5f,width(), height(), Qt::AlignCenter, s);
	
	/*painter.setPen(Qt::black);
	painter.setBrush(Qt::NoBrush);
	painter.drawRect(-width()*0.5f,-height()*0.5f,width(), height());
	*/
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
