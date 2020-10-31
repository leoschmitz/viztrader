//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCRELATIVESTRENGTHINDEXINDICATOR_H__
#define __VIZCRELATIVESTRENGTHINDEXINDICATOR_H__
//-----------------------------------------------------------------------------
#include "VizIIndicator.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCRelativeStrengthIndexIndicator : public VizIIndicator
{
	Q_OBJECT

	Q_PROPERTY(int timePeriod READ timePeriod WRITE setTimePeriod USER true)
	Q_PROPERTY(int priceField READ propertyGetPriceField WRITE propertySetPriceField USER true)

public:
	VizCRelativeStrengthIndexIndicator();
	virtual ~VizCRelativeStrengthIndexIndicator();

	s32 timePeriod() const;
	void setTimePeriod(s32 period);
	VizEPriceField priceField() const;
	void setPriceField(VizEPriceField field);

	virtual void resetToDefaults();
	virtual void updateData();
	virtual QString toolTip(const QPointF& p) const;

	virtual void addRelativeItems(VizCGraphicsPanel* graphicsPanel);

private:
	s32 TimePeriod;
	VizEPriceField PriceField;

	int propertyGetPriceField() const;
	void propertySetPriceField(int value);

};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
//Q_DECLARE_METATYPE(viz::gui::VizCRelativeStrengthIndexIndicator*)
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
