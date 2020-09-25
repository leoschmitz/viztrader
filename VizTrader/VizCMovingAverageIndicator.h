//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCMOVINGAVERAGEINDICATOR_H__
#define __VIZCMOVINGAVERAGEINDICATOR_H__
//-----------------------------------------------------------------------------
#include "VizIIndicator.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCMovingAverageIndicator : public VizIIndicator
{
	Q_OBJECT

	Q_PROPERTY(int timePeriod READ timePeriod WRITE setTimePeriod USER true)
	Q_PROPERTY(int priceField READ propertyGetPriceField WRITE propertySetPriceField USER true)
	Q_PROPERTY(int movingAverageMethod READ propertyGetMovingAverageMethod WRITE propertySetMovingAverageMethod USER true)
	Q_PROPERTY(int horizontalShift READ horizontalShift WRITE setHorizontalShift USER true)
	Q_PROPERTY(double verticalShift READ verticalShift WRITE setVerticalShift USER true)

public:
	VizCMovingAverageIndicator();
	virtual ~VizCMovingAverageIndicator();

	s32 timePeriod() const;
	void setTimePeriod(s32 period);
	VizEPriceField priceField() const;
	void setPriceField(VizEPriceField field);
	VizEMovingAverageMethod movingAverageMethod() const;
	void setMovingAverageMethod(VizEMovingAverageMethod method);
	s32 horizontalShift() const;
	void setHorizontalShift(s32 value);
	f64 verticalShift() const;
	void setVerticalShift(f64 value);

	virtual void resetToDefaults();
	virtual void updateData();
	virtual void build();
	virtual QString toolTip(const QPointF& p) const;
	virtual void yRange(bool& isEmpty, f64& minY, f64& maxY) const;

private:
	s32 TimePeriod;
	VizEPriceField PriceField;
	VizEMovingAverageMethod MovingAverageMethod;
	s32 HorizontalShift;
	f64 VerticalShift;

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
