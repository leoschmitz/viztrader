//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizIGraphicsItemBasedOnSecurity.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
VizIGraphicsItemBasedOnSecurity::VizIGraphicsItemBasedOnSecurity() : VizIGraphicsItem(), DataFacade(0), SecurityPeriodsManipulator(0)
{
	DataFacade = io::VizCDataFacade::instance();
	SecurityPeriodsManipulator = core::VizCSecurityPeriodsManipulator::instance();

	resetToDefaults();
}
//-----------------------------------------------------------------------------
VizIGraphicsItemBasedOnSecurity::~VizIGraphicsItemBasedOnSecurity()
{
}
//-----------------------------------------------------------------------------
void VizIGraphicsItemBasedOnSecurity::resetToDefaults()
{
	VizIGraphicsItem::resetToDefaults();
	
	PenWidth = viz::VEPW_DEFAULT;
	SecuritySymbol = "";
}
//-----------------------------------------------------------------------------
const QVector<core::VizCTradePeriod>& VizIGraphicsItemBasedOnSecurity::periods() const
{
	return Periods;
}
//-----------------------------------------------------------------------------
const QString& VizIGraphicsItemBasedOnSecurity::securitySymbol() const
{
	return SecuritySymbol;
}
//-----------------------------------------------------------------------------
void VizIGraphicsItemBasedOnSecurity::setSecuritySymbol(const QString& symbol)
{
	SecuritySymbol = symbol;
}
//-----------------------------------------------------------------------------
VizEPenWidth VizIGraphicsItemBasedOnSecurity::penWidth() const
{
	return PenWidth;
}
//-----------------------------------------------------------------------------
void VizIGraphicsItemBasedOnSecurity::setPenWidth(VizEPenWidth penWidth)
{
	if(PenWidth != penWidth)
	{
		int old = propertyGetPenWidth();

		PenWidth = penWidth;
		
		emit createUndoCommandPropertyChanged(this, "penWidth", old, propertyGetPenWidth(), false);
		emit itemChanged();
	}
}
//-----------------------------------------------------------------------------
int VizIGraphicsItemBasedOnSecurity::propertyGetPenWidth() const
{
	return static_cast<int>(penWidth() - 1);
}
//-----------------------------------------------------------------------------
void VizIGraphicsItemBasedOnSecurity::propertySetPenWidth(int value)
{
	setPenWidth(static_cast<VizEPenWidth>(value + 1));
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
