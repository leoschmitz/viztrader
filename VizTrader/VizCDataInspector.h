//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCDATAINSPECTOR_H__
#define __VIZCDATAINSPECTOR_H__
//-----------------------------------------------------------------------------
#include <QTreeWidget>
#include "VizCTradePeriod.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCDataInspector : public QTreeWidget
{
public:
	VizCDataInspector();
	~VizCDataInspector();

	void setData(const QString& securitySymbol, f64 value, const core::VizCTradePeriod& period);

	void clearData();


};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
