//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCVOLUMEINDICATOR_H__
#define __VIZCVOLUMEINDICATOR_H__
//-----------------------------------------------------------------------------
#include "VizIIndicator.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCVolumeIndicator : public VizIIndicator
{
	Q_OBJECT

public:
	VizCVolumeIndicator();
	virtual ~VizCVolumeIndicator();

	virtual void resetToDefaults();
	virtual void updateData();
	virtual QString toolTip(const QPointF& p) const;

};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
