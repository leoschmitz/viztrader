//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCMOUNTAINCHART_H__
#define __VIZCMOUNTAINCHART_H__
//-----------------------------------------------------------------------------
#include "VizIChart.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCMountainChart : public VizIChart
{
	Q_OBJECT
public:
	VizCMountainChart();
	virtual ~VizCMountainChart();

	virtual bool isLinear() const;
	virtual void build();
	virtual void paint(QPainter* painter);
	virtual bool intersects(const QRectF& rect) const;
	virtual void ajustToOverlayWithoutScale(f64 itemMinY, f64 itemMaxY);

private:
	QVector<QPointF> LinesUp;
	QVector<QPointF> LinesDown;
	QVector<QPolygonF> PolygonsUp;
	QVector<QPolygonF> PolygonsDown;
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
