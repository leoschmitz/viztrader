//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCMINUSDIRECTIONALINDICATOR_H__
#define __VIZCMINUSDIRECTIONALINDICATOR_H__
//-----------------------------------------------------------------------------
#include "VizIIndicator.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCMinusDirectionalIndicator : public VizIIndicator
{
	Q_OBJECT

	Q_PROPERTY(int timePeriod READ timePeriod WRITE setTimePeriod USER true)

public:
	VizCMinusDirectionalIndicator();
	virtual ~VizCMinusDirectionalIndicator();

	s32 timePeriod() const;
	void setTimePeriod(s32 period);

	virtual void resetToDefaults();
	virtual void updateData();
	virtual QString toolTip(const QPointF& p) const;

private:
	s32 TimePeriod;

};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
