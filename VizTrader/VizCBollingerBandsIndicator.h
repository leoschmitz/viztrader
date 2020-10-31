//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCBOLLINGERBANDSINDICATOR_H__
#define __VIZCBOLLINGERBANDSINDICATOR_H__
//-----------------------------------------------------------------------------
#include "VizIIndicator.h"
#include "VizCBollingerLowerBandIndicator.h"
#include "VizCBollingerUpperBandIndicator.h"
#include "VizCPropertyEditor.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCBollingerBandsIndicator : public VizIIndicator
{
	Q_OBJECT

	Q_PROPERTY(int timePeriod READ timePeriod WRITE setTimePeriod USER true)
	Q_PROPERTY(int priceField READ propertyGetPriceField WRITE propertySetPriceField USER true)
	Q_PROPERTY(int movingAverageMethod READ propertyGetMovingAverageMethod WRITE propertySetMovingAverageMethod USER true)
	Q_PROPERTY(int horizontalShift READ horizontalShift WRITE setHorizontalShift USER true)
	Q_PROPERTY(double standardDeviationUp READ standardDeviationUp WRITE setStandardDeviationUp USER true)
	Q_PROPERTY(double standardDeviationDown READ standardDeviationDown WRITE setStandardDeviationDown USER true)

public:
	VizCBollingerBandsIndicator();
	virtual ~VizCBollingerBandsIndicator();

	s32 timePeriod() const;
	void setTimePeriod(s32 period);
	VizEPriceField priceField() const;
	void setPriceField(VizEPriceField field);
	VizEMovingAverageMethod movingAverageMethod() const;
	void setMovingAverageMethod(VizEMovingAverageMethod method);
	s32 horizontalShift() const;
	void setHorizontalShift(s32 value);
	f64 standardDeviationUp() const;
	void setStandardDeviationUp(f64 value);
	f64 standardDeviationDown() const;
	void setStandardDeviationDown(f64 value);

	virtual void resetToDefaults();
	virtual void updateData();
	virtual void build();
	virtual QString toolTip(const QPointF& p) const;
	virtual void ajustToOverlayWithoutScale(f64 itemMinY, f64 itemMaxY);
	virtual void yRange(bool& isEmpty, f64& minY, f64& maxY) const;

	virtual void addRelativeItems(VizCGraphicsPanel* graphicsPanel);

private:
	f64 StandardDeviationUp;
	f64 StandardDeviationDown;
	s32 TimePeriod;
	VizEPriceField PriceField;
	VizEMovingAverageMethod MovingAverageMethod;
	s32 HorizontalShift;

	VizCBollingerUpperBandIndicator* bollingerUpperBand() const;
	VizCBollingerLowerBandIndicator* bollingerLowerBand() const;
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
