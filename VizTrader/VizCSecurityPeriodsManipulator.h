//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCSECURITYPERIODSMANIPULATOR_H__
#define __VIZCSECURITYPERIODSMANIPULATOR_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include "VizCSecurity.h"
#include "VizCSecurityPeriod.h"
#include "VizCTradePeriod.h"
#include "VizCSplit.h"
#include <QVector>
//-----------------------------------------------------------------------------
namespace viz
{
namespace core
{
//-----------------------------------------------------------------------------
class VizCSecurityPeriodsManipulator
{
public:
	static VizCSecurityPeriodsManipulator* instance();

	QVector<VizCSecurityPeriod> adjustSecurityPeriodsSplits(QVector<VizCSplit>& splits, const QVector<VizCSecurityPeriod>& periods);
	QVector<VizCTradePeriod> convertPeriodicityInteval(const QVector<VizCSecurityPeriod>& periodsDaily, viz::VizESecurityPeriodicity securityPeriodicity = viz::VESP_DEFAULT, const QDateTime& startDate = QDateTime(), const QDateTime& endDate = QDateTime());
	QVector<VizCTradePeriod> generatePreviewPeriods(const QVector<VizCSecurityPeriod>& periodsDaily);

private:
	VizCSecurityPeriodsManipulator();
	static VizCSecurityPeriodsManipulator* Instance;
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
