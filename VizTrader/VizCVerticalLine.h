//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCVERTICALLINE_H__
#define __VIZCVERTICALLINE_H__
//-----------------------------------------------------------------------------
#include "VizIGraphicsItemBasedOnShape.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCVerticalLine : public VizIGraphicsItemBasedOnShape
{
	Q_OBJECT
	Q_PROPERTY(QDateTime value READ value WRITE setValue USER true)

public:
	VizCVerticalLine();
	virtual ~VizCVerticalLine();

	virtual void resetToDefaults();

	QDateTime value() const;
	void setValue(const QDateTime& value);

	virtual void paint(QPainter* painter);
	virtual void build();
	virtual bool intersects(const QRectF& rect) const;
	virtual QString toolTip(const QPointF& p) const;
	virtual void ajustToOverlayWithoutScale(f64 itemMinY, f64 itemMaxY);
	virtual void yRange(bool& isEmpty, f64& minY, f64& maxY) const;

	virtual void endEdit(const VizSShapeTransform& transform);
	virtual QList<QPointF> controlPoints() const;

private:
	virtual void paintRubberRands(QPainter* painter);

	QDateTime Value;
	QPointF P1, P2;
	s32 DateIndex;
	bool FindDateTimeInHorizontalScale;
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
