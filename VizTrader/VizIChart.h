//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZICHART_H__
#define __VIZICHART_H__
//-----------------------------------------------------------------------------
#include "VizIGraphicsItemBasedOnSecurity.h"
#include "VizCSecurityPeriod.h"
#include <QColor>
#include <QVector>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizIChart : public VizIGraphicsItemBasedOnSecurity
{
	Q_OBJECT

	Q_PROPERTY(QColor upColor READ upColor WRITE setUpColor USER true)
	Q_PROPERTY(QColor downColor READ downColor WRITE setDownColor USER true)
	//Q_PROPERTY(int chartType READ propertyGetChartType WRITE propertySetChartType USER true)

public:
	VizIChart();
	~VizIChart();

	virtual QString toPlainText();
	virtual void resetToDefaults();
	virtual void nearestYValue(const QPointF& pos, f64& value, bool& isValid);
	virtual void yRange(bool& isEmpty, f64& minY, f64& maxY) const;
	virtual void lastTrade(f64& value, bool& isValid) const;
	virtual void ajustPeriods();
	virtual void updateData();
	virtual QString toolTip(const QPointF& p) const;
	virtual bool isLinear() const = 0;
	virtual QString nameToTitleBar(bool showLastTradeValues, bool useColorOfPlot) const;

	QColor upColor() const;
	QColor downColor() const;
	void setUpColor(const QColor& color);
	void setDownColor(const QColor& color);

protected:
	QColor UpColor;
	QColor DownColor;
	f64 lastTradeDifference() const;
/*
private slots:
	int propertyGetChartType();
	void propertySetChartType(int value);
	*/
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
