//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCLINECHART_H__
#define __VIZCLINECHART_H__
//-----------------------------------------------------------------------------
#include "VizIChart.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCLineChart : public VizIChart
{
	Q_OBJECT
public:
	VizCLineChart();
	virtual ~VizCLineChart();

	virtual bool isLinear() const;
	virtual void build();
	virtual void paint(QPainter* painter);
	virtual bool intersects(const QRectF& rect) const;
	virtual void ajustToOverlayWithoutScale(f64 itemMinY, f64 itemMaxY);

private:
	QVector<QPointF> LinesUp;
	QVector<QPointF> LinesDown;
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
