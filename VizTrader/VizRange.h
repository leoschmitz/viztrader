//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZRANGE_H__
#define __VIZRANGE_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace core
{
//-----------------------------------------------------------------------------
class VizRange
{
public:
	VizRange() : Minimum(0), Maximum(100)
	{
	}
	VizRange(s32 minimum, s32 maximum) : Minimum(minimum), Maximum(maximum)
	{
	}
	s32 minimum() const
	{
		return Minimum;
	}
	s32 maximum() const
	{
		return Maximum;
	}
	void setMinimum(s32 minimum)
	{
		Minimum = minimum;
	}
	void setMaximum(s32 maximum)
	{
		Maximum = maximum;
	}
	bool isValueInRange(s32 value)
	{
		return (value <= Maximum) && (value >= Minimum);
	}
	bool operator==(const VizRange& other) const
	{
		return other.Minimum == Minimum && other.Maximum == Maximum;
	}
	bool operator!=(const VizRange& other) const
	{
		return other.Minimum != Minimum || other.Maximum != Maximum;
	}
private:
	s32 Minimum;
	s32 Maximum;
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
