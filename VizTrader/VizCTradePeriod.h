//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCTRADEPERIOD_H__
#define __VIZCTRADEPERIOD_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include "VizMath.h"
#include <QDateTime>
//-----------------------------------------------------------------------------
namespace viz
{
namespace core
{
//-----------------------------------------------------------------------------
class VizCTradePeriod
{
public:
	VizCTradePeriod();
	VizCTradePeriod(f64 openPrice, f64 closePrice, f64 maxPrice, f64 minPrice, f64 totalVolume, const QDateTime& timeStamp);
	VizCTradePeriod(f64 indicatorValue, const QDateTime& timeStamp);
	VizCTradePeriod(const VizCTradePeriod& other);
	~VizCTradePeriod();
	bool isValid() const;
	void set(f64 openPrice, f64 closePrice, f64 maxPrice, f64 minPrice, f64 totalVolume, const QDateTime& timeStamp);
	void set(f64 indicatorValue, const QDateTime& timeStamp);
	void get(f64& openPrice, f64& closePrice, f64& maxPrice, f64& minPrice, f64& totalVolume, QDateTime& timeStamp) const;
	void get(f64& indicatorValue, QDateTime& timeStamp) const;
	void set(const VizCTradePeriod& other);
	void get(VizCTradePeriod& other) const;
	VizCTradePeriod& operator=(const VizCTradePeriod& other);
	VizCTradePeriod toLogarithmic() const;

	f64 OpenPrice;
	f64 ClosePrice;
	f64 MaxPrice;
	f64 MinPrice;
	f64 TotalVolume;
	f64 IndicatorValue;
    QDateTime TimeStamp;

private:
	bool IsValid;
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
