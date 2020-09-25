//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCBOLLINGERUPPERBANDINDICATOR_H__
#define __VIZCBOLLINGERUPPERBANDINDICATOR_H__
//-----------------------------------------------------------------------------
#include "VizIIndicator.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCBollingerBandsIndicator;
//-----------------------------------------------------------------------------
class VizCBollingerUpperBandIndicator : public VizIIndicator
{
	Q_OBJECT

	Q_PROPERTY(int timePeriod READ timePeriod WRITE setTimePeriod USER true)
	Q_PROPERTY(int priceField READ propertyGetPriceField WRITE propertySetPriceField USER true)
	Q_PROPERTY(int movingAverageMethod READ propertyGetMovingAverageMethod WRITE propertySetMovingAverageMethod USER true)
	Q_PROPERTY(int horizontalShift READ horizontalShift WRITE setHorizontalShift USER true)
	Q_PROPERTY(double standardDeviation READ standardDeviation WRITE setStandardDeviation USER true)

public:
	VizCBollingerUpperBandIndicator();
	virtual ~VizCBollingerUpperBandIndicator();

	s32 timePeriod() const;
	void setTimePeriod(s32 period);
	VizEPriceField priceField() const;
	void setPriceField(VizEPriceField field);
	VizEMovingAverageMethod movingAverageMethod() const;
	void setMovingAverageMethod(VizEMovingAverageMethod method);
	s32 horizontalShift() const;
	void setHorizontalShift(s32 value);
	f64 standardDeviation() const;
	void setStandardDeviation(f64 value);

	virtual void resetToDefaults();
	virtual void updateData();
	virtual void build();
	virtual QString toolTip(const QPointF& p) const;

private:
	f64 StandardDeviation;
	s32 TimePeriod;
	VizEPriceField PriceField;
	VizEMovingAverageMethod MovingAverageMethod;
	s32 HorizontalShift;

	void applyShiftProperties();
	int propertyGetPriceField() const;
	void propertySetPriceField(int value);
	int propertyGetMovingAverageMethod() const;
	void propertySetMovingAverageMethod(int value);
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
