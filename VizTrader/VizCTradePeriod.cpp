//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCTradePeriod.h"
#include "VizMath.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace core
{
//-----------------------------------------------------------------------------
VizCTradePeriod::VizCTradePeriod() : OpenPrice(0.0), ClosePrice(0.0), MaxPrice(0.0), MinPrice(0.0), TotalVolume(0.0), IndicatorValue(0.0), TimeStamp(QDateTime()), IsValid(false)
{
}
//-----------------------------------------------------------------------------
VizCTradePeriod::VizCTradePeriod(f64 openPrice, f64 closePrice, f64 maxPrice, f64 minPrice, f64 totalVolume, const QDateTime& timeStamp) : OpenPrice(openPrice), ClosePrice(closePrice), MaxPrice(maxPrice), MinPrice(minPrice), TotalVolume(totalVolume), TimeStamp(timeStamp), IsValid(true)
{
}
//-----------------------------------------------------------------------------
VizCTradePeriod::VizCTradePeriod(f64 indicatorValue, const QDateTime& timeStamp) : IndicatorValue(indicatorValue), TimeStamp(timeStamp), IsValid(true)
{
}
//-----------------------------------------------------------------------------
VizCTradePeriod::VizCTradePeriod(const VizCTradePeriod& other)
{
	set(other);
}
//-----------------------------------------------------------------------------
VizCTradePeriod::~VizCTradePeriod()
{
}
//-----------------------------------------------------------------------------
bool VizCTradePeriod::isValid() const
{
	//return TimeStamp.isValid();
	return IsValid;
}
//-----------------------------------------------------------------------------
void VizCTradePeriod::set(f64 openPrice, f64 closePrice, f64 maxPrice, f64 minPrice, f64 totalVolume, const QDateTime& timeStamp)
{
	IsValid = true;
	OpenPrice = openPrice;
	ClosePrice = closePrice;
	MaxPrice = maxPrice;
	MinPrice = minPrice;
	TotalVolume = totalVolume;
	IndicatorValue = 0.0;
	TimeStamp = timeStamp;
}
//-----------------------------------------------------------------------------
void VizCTradePeriod::set(f64 indicatorValue, const QDateTime& timeStamp)
{
	IsValid = true;
	OpenPrice = 0.0;
	ClosePrice = 0.0;
	MaxPrice = 0.0;
	MinPrice = 0.0;
	TotalVolume = 0.0;
	IndicatorValue = indicatorValue;
	TimeStamp = timeStamp;
}
//-----------------------------------------------------------------------------
void VizCTradePeriod::get(f64& openPrice, f64& closePrice, f64& maxPrice, f64& minPrice, f64& totalVolume, QDateTime& timeStamp) const
{
	openPrice = OpenPrice;
	closePrice = ClosePrice;
	maxPrice = MaxPrice;
	minPrice = MinPrice;
	timeStamp = TimeStamp;
	totalVolume = TotalVolume;
}
//-----------------------------------------------------------------------------
void VizCTradePeriod::get(f64& indicatorValue, QDateTime& timeStamp) const
{
	indicatorValue = IndicatorValue;
	timeStamp = TimeStamp;
}
//-----------------------------------------------------------------------------
void VizCTradePeriod::set(const VizCTradePeriod& other)
{
	IsValid = other.IsValid;
	OpenPrice = other.OpenPrice;
	ClosePrice = other.ClosePrice;
	MaxPrice = other.MaxPrice;
	MinPrice = other.MinPrice;
	IndicatorValue = other.IndicatorValue;
	TotalVolume = other.TotalVolume;
	TimeStamp = other.TimeStamp;
}
//-----------------------------------------------------------------------------
void VizCTradePeriod::get(VizCTradePeriod& other) const
{
	other.OpenPrice = OpenPrice;
	other.ClosePrice = ClosePrice;
	other.MaxPrice = MaxPrice;
	other.MinPrice = MinPrice;
	other.IndicatorValue = IndicatorValue;
	other.TimeStamp = TimeStamp;
}
//-----------------------------------------------------------------------------
VizCTradePeriod& VizCTradePeriod::operator=(const VizCTradePeriod& other)
{
	IsValid = other.IsValid;
	OpenPrice = other.OpenPrice;
	ClosePrice = other.ClosePrice;
	MaxPrice = other.MaxPrice;
	MinPrice = other.MinPrice;
	TotalVolume = other.TotalVolume;
	IndicatorValue = other.IndicatorValue;
	TimeStamp = other.TimeStamp;
	return *this;
}
//-----------------------------------------------------------------------------
VizCTradePeriod VizCTradePeriod::toLogarithmic() const
{
	VizCTradePeriod other;
	other.IsValid = IsValid;
	other.OpenPrice = core::clampLog10f(OpenPrice);
	other.ClosePrice = core::clampLog10f(ClosePrice);
	other.MaxPrice = core::clampLog10f(MaxPrice);
	other.MinPrice = core::clampLog10f(MinPrice);
	other.TotalVolume = core::clampLog10f(TotalVolume);
	other.IndicatorValue = core::clampLog10f(IndicatorValue);
	return other;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
