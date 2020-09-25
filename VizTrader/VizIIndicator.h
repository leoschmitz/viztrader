//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZIINDICATOR_H__
#define __VIZIINDICATOR_H__
//-----------------------------------------------------------------------------
#include "VizIGraphicsItemBasedOnSecurity.h"
#include "VizCTechnicalAnalysisFunctions.h"
#include <QColor>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizIIndicator : public VizIGraphicsItemBasedOnSecurity
{
	Q_OBJECT

	Q_PROPERTY(QColor color READ color WRITE setColor USER true)
	Q_PROPERTY(int indicatorStyle READ propertyGetindicatorStyle WRITE propertySetIndicatorStyle USER true)
	Q_PROPERTY(int penStyle READ propertyGetPenStyle WRITE propertySetPenStyle USER true)

public:
	VizIIndicator();
	~VizIIndicator();

	QColor color() const;
	void setColor(const QColor& color);
	viz::VizEIndicatorStyle indicatorStyle() const;
	void setIndicatorStyle(viz::VizEIndicatorStyle style);
	viz::VizEPenStyle penStyle() const;
	void setPenStyle(viz::VizEPenStyle penStyle);

	virtual void resetToDefaults();
	virtual QString toPlainText();
	virtual void build();
	virtual void paint(QPainter* painter);
	virtual bool intersects(const QRectF& rect) const;
	virtual void lastTrade(f64& value, bool& isValid) const;
	virtual void ajustPeriods();
	virtual void nearestYValue(const QPointF& pos, f64& value, bool& isValid);
	virtual void yRange(bool& isEmpty, f64& minY, f64& maxY) const;
	virtual void ajustToOverlayWithoutScale(f64 itemMinY, f64 itemMaxY);
	virtual QString nameToTitleBar(bool showLastTradeValues, bool useColorOfPlot) const;

	virtual void addRelativeItems(VizCGraphicsPanel* graphicsPanel);

protected:
	QVector<QPointF> Points;
	viz::VizEIndicatorStyle IndicatorStyle;
	viz::VizEPenStyle PenStyle;
	QColor Color;
	core::VizCTechnicalAnalysisFunctions* TechnicalAnalysisFunctions;
	
	void buildPoints(s32 rulesValuesIndexBegin, s32 rulesValuesIndexEnd);
	void buildHistogram(s32 rulesValuesIndexBegin, s32 rulesValuesIndexEnd);
	void buildLines(s32 rulesValuesIndexBegin, s32 rulesValuesIndexEnd);
	void paintPoints(QPainter* painter);
	void paintHistogram(QPainter* painter);
	void paintLines(QPainter* painter);

	void shiftRulesValuesIndex(s32 horizontalShift, s32& rulesValuesIndexBegin, s32& rulesValuesIndexEnd) const;

private:
	int propertyGetindicatorStyle() const;
	void propertySetIndicatorStyle(int value);
	int propertyGetPenStyle() const;
	void propertySetPenStyle(int value);
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
