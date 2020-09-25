//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCTRENDLINE_H__
#define __VIZCTRENDLINE_H__
//-----------------------------------------------------------------------------
#include "VizIGraphicsItemBasedOnShape.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCTrendLine : public VizIGraphicsItemBasedOnShape
{
	Q_OBJECT
	Q_PROPERTY(double startValue READ startValue WRITE setStartValue USER true)
	Q_PROPERTY(double endValue READ endValue WRITE setEndValue USER true)
	Q_PROPERTY(QDateTime startDate READ startDate WRITE setStartDate USER true)
	Q_PROPERTY(QDateTime endDate READ endDate WRITE setEndDate USER true)
	Q_PROPERTY(bool leftExtention READ leftExtention WRITE setLeftExtention USER true)
	Q_PROPERTY(bool rightExtention READ rightExtention WRITE setRightExtention USER true)

public:
	VizCTrendLine();
	virtual ~VizCTrendLine();

	virtual void resetToDefaults();

	f64 startValue() const;
	f64 endValue() const;
	QDateTime startDate() const;
	QDateTime endDate() const;
	bool leftExtention() const;
	bool rightExtention() const;
	
	void setStartValue(f64 value);
	void setEndValue(f64 value);
	void setStartDate(const QDateTime& date);
	void setEndDate(const QDateTime& date);
	void setLeftExtention(bool value);
	void setRightExtention(bool value);

	virtual void paint(QPainter* painter);
	virtual void build();
	virtual bool intersects(const QRectF& rect) const;
	virtual QString toolTip(const QPointF& p) const;
	virtual void ajustToOverlayWithoutScale(f64 itemMinY, f64 itemMaxY);
	virtual void yRange(bool& isEmpty, f64& minY, f64& maxY) const;

	virtual void endEdit(const VizSShapeTransform& transform);
	virtual void edit(const VizSShapeTransform& transform);
	virtual QList<QPointF> controlPoints() const;

private:
	virtual void paintRubberRands(QPainter* painter);
	void computeExtentionsPoints(const QPointF& p1, const QPointF& p2, QPointF* extentions);

	f64 StartValue;
	f64 EndValue;
	QDateTime StartDate;
	QDateTime EndDate;
	s32 StartDateIndex;
	s32 EndDateIndex;

	QPointF P1, P2;
	QPointF ExtentionPoints[4];

	bool LeftExtention;
	bool RightExtention;
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
