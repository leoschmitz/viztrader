//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Solu��es em Visualiza��o LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCHEIKINASHICANDLESTICKSCHART_H__
#define __VIZCHEIKINASHICANDLESTICKSCHART_H__
//-----------------------------------------------------------------------------
#include "VizIChart.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCHeikinAshiCandlesticksChart : public VizIChart
{
	Q_OBJECT
public:
	VizCHeikinAshiCandlesticksChart();
	virtual ~VizCHeikinAshiCandlesticksChart();
	virtual bool isLinear() const;
	virtual void build();
	virtual void paint(QPainter* painter);
	virtual bool intersects(const QRectF& rect) const;
	virtual void ajustToOverlayWithoutScale(f64 itemMinY, f64 itemMaxY);

private:
	QVector<QPointF> LinesUp;
	QVector<QPointF> LinesDown;
	QVector<QRectF> RectsUp;
	QVector<QRectF> RectsDown;
	QVector<QRectF> FilledRectsUp;
	QVector<QRectF> FilledRectsDown;

	QVector<QPointF> LinesUpLod;
	QVector<QPointF> LinesDownLod;
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------