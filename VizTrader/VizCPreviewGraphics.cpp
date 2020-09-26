//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCPreviewGraphics.h"
#include "VizCSettings.h"
#include <QDebug>
#include <QPen>
#include <QVariant>
#include <QPainter>
#include <QPainterPath>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
VizCPreviewGraphics::VizCPreviewGraphics() : QFrame(), MinY(0), MaxY(0), Type(viz::gui::VEPGT_CHART)
{
	setFrameStyle(QFrame::Box | QFrame::Sunken);
	setLineWidth(1);
}
//-----------------------------------------------------------------------------
VizCPreviewGraphics::~VizCPreviewGraphics()
{
}
//-----------------------------------------------------------------------------
void VizCPreviewGraphics::setPeriods(const QVector<core::VizCTradePeriod>& periods)
{
	Periods = periods;
	MaxY = std::numeric_limits<f64>::min();
	MinY = std::numeric_limits<f64>::max();
	foreach(const core::VizCTradePeriod& Period, periods)
	{
		if(Period.ClosePrice > MaxY)
			MaxY = Period.ClosePrice;
		if(Period.ClosePrice < MinY)
			MinY = Period.ClosePrice;
	}

	f64 border = -(MaxY-MinY) * 0.05;
	MaxY = MaxY - border;
	MinY = MinY + border;

	repaint();
}
//-----------------------------------------------------------------------------
void VizCPreviewGraphics::setRenderType(VizEPreviewGraphicsType type)
{
	Type = type;
	repaint();

}
//-----------------------------------------------------------------------------
void VizCPreviewGraphics::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);

	painter.setRenderHints(QPainter::Antialiasing, true);
	painter.setRenderHints(QPainter::TextAntialiasing, true);

	QColor BackGroundColor = Qt::white;
	QColor ShadownColor = Qt::black;//Qt::darkGray;
	QColor ForegroundColor0 = QColor(80, 254, 253);
	QColor ForegroundColor1 = QColor(80, 254, 253);
	QColor ForegroundColor2 = QColor(62, 136, 243);

	if(Type == VEPGT_INDICATOR)
	{
		BackGroundColor = Qt::white;
		ShadownColor = Qt::black;
		ForegroundColor0 = Qt::red;
		ForegroundColor1 = ForegroundColor0.light(50);
		ForegroundColor2 = ForegroundColor0.light(100);
	}

	f64 window_width = static_cast<f64>(Periods.size());
	f64 window_height = static_cast<f64>(MaxY - MinY);
	f64 viewport_width = geometry().width();
	f64 viewport_height = geometry().height();

	painter.scale(viewport_width / window_width, -(viewport_height / window_height));
	painter.translate(0, -(window_height + MinY));


	//QLinearGradient gradientBackground(0, MaxY, 0, MinY);
	//gradientBackground.setColorAt(0.0, BackGroundColor1);
	//gradientBackground.setColorAt(1.0, BackGroundColor2);
	//painter.fillRect(QRectF(0, MinY, window_width, window_height), gradientBackground);
	
	
	painter.fillRect(QRectF(0, MinY, window_width, window_height), BackGroundColor);

	QPolygonF polyline;
	s32 i = 0;
	polyline << QPointF(i, MinY);
	
	for(i = 0; i < Periods.size(); i++)
	{
		polyline << QPointF(i + 0.5, Periods[i].ClosePrice);
	}
	polyline << QPointF(i, MinY);
	polyline << QPointF(0, MinY);

	QPainterPath path;
	path.addPolygon(polyline);

	QLinearGradient gradientFpreground(0, MaxY, 0, MinY);
	gradientFpreground.setColorAt(0.0, ForegroundColor1);
	gradientFpreground.setColorAt(1.0, ForegroundColor2);
	painter.fillPath(path, gradientFpreground);


	QPen pen2(ShadownColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
	pen2.setCosmetic(true);
	painter.setPen(pen2);
	painter.drawPolyline(polyline);

	QPen pen1(ForegroundColor0, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
	pen1.setCosmetic(true);
	painter.setPen(pen1);
	painter.drawPolyline(polyline);


	QFrame::paintEvent(event);

}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
