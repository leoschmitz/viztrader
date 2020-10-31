//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCPREVIEWGRAPHICS_H__
#define __VIZCPREVIEWGRAPHICS_H__
//-----------------------------------------------------------------------------
#include "VizCTradePeriod.h"
#include <QFrame>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
enum VizEPreviewGraphicsType
{
	VEPGT_INDICATOR = 0,
	VEPGT_CHART
};
//-----------------------------------------------------------------------------
class VizCPreviewGraphics : public QFrame
{
	Q_OBJECT
public:
	VizCPreviewGraphics();
	~VizCPreviewGraphics();

	void setPeriods(const QVector<core::VizCTradePeriod>& periods);
	void setRenderType(VizEPreviewGraphicsType type);

	virtual void paintEvent(QPaintEvent* event);

private:
	VizEPreviewGraphicsType Type;
	QVector<core::VizCTradePeriod> Periods;
	f64 MinY;
	f64 MaxY;
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
