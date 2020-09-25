//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZIGRAPHICSITEMBASEDONSECURITY_H__
#define __VIZIGRAPHICSITEMBASEDONSECURITY_H__
//-----------------------------------------------------------------------------
#include "VizIGraphicsItem.h"
#include "VizCTradePeriod.h"
#include "VizCDataFacade.h"
#include "VizCSecurityPeriodsManipulator.h"
#include "VizCSecurityPeriod.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizIGraphicsItemBasedOnShape;
//-----------------------------------------------------------------------------
class VizIGraphicsItemBasedOnSecurity : public VizIGraphicsItem
{
	Q_OBJECT
	Q_PROPERTY(int penWidth READ propertyGetPenWidth WRITE propertySetPenWidth USER true)

public:
	VizIGraphicsItemBasedOnSecurity();
	~VizIGraphicsItemBasedOnSecurity();

	const QString& securitySymbol() const;
	void setSecuritySymbol(const QString& symbol);
	const QVector<core::VizCTradePeriod>& periods() const;
	VizEPenWidth penWidth() const;
	void setPenWidth(VizEPenWidth penWidth);
	
	virtual void resetToDefaults();
	virtual QString toPlainText() = 0;
	virtual void ajustPeriods() = 0;
	virtual void nearestYValue(const QPointF& pos, f64& value, bool& isValid) = 0;
	virtual void lastTrade(f64& value, bool& isValid) const = 0;
	virtual void updateData() = 0;
	virtual QString nameToTitleBar(bool showLastTradeValues, bool useColorOfPlot) const = 0;

protected:
	QString SecuritySymbol;
	QVector<core::VizCTradePeriod> Periods;
	VizEPenWidth PenWidth;
	io::VizCDataFacade* DataFacade;
	core::VizCSecurityPeriodsManipulator* SecurityPeriodsManipulator;
	int propertyGetPenWidth() const;
	void propertySetPenWidth(int value);
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
