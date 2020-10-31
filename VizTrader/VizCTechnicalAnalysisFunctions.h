//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCTECHNICALANALYSISFUNCTIONS_H__
#define __VIZCTECHNICALANALYSISFUNCTIONS_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include "VizCSecurity.h"
#include "VizCSecurityPeriod.h"
#include "VizCTradePeriod.h"
#include <QVector>
//-----------------------------------------------------------------------------
namespace viz
{
namespace core
{
//-----------------------------------------------------------------------------
class VizCTechnicalAnalysisFunctions
{
public:

	static VizCTechnicalAnalysisFunctions* instance();

	QVector<VizCTradePeriod> movingAverage(const QVector<VizCTradePeriod>& in, s32 timePeriod, viz::VizEPriceField priceField, viz::VizEMovingAverageMethod movingAverageMethod);
	QVector<VizCTradePeriod> relativeStrengthIndex(const QVector<VizCTradePeriod>& in, s32 timePeriod, viz::VizEPriceField priceField);
	QVector<VizCTradePeriod> volume(const QVector<VizCTradePeriod>& in);
	QVector<VizCTradePeriod> averageDirectionalMovementIndex(const QVector<VizCTradePeriod>& in, s32 timePeriod);
	QVector<VizCTradePeriod> minusDirectionalIndicator(const QVector<VizCTradePeriod>& in, s32 timePeriod);
	QVector<VizCTradePeriod> plusDirectionalIndicator(const QVector<VizCTradePeriod>& in, s32 timePeriod);
	//void bollingerBands(const QVector<VizCSecurityPeriod>& in, s32 timePeriod, f64 standardDeviationUp, f64 standardDeviationDown, viz::VizEPriceField priceField, viz::VizEMovingAverageMethod movingAverageMethod, QVector<gui::VzCGraphicsPeriod>& upperBand, QVector<gui::VzCGraphicsPeriod>& middleBand, QVector<gui::VzCGraphicsPeriod>& lowerBand);

	QVector<VizCTradePeriod> middleBollingerBand(const QVector<VizCTradePeriod>& in, s32 timePeriod, viz::VizEPriceField priceField, viz::VizEMovingAverageMethod movingAverageMethod);
	QVector<VizCTradePeriod> upperBollingerBand(const QVector<VizCTradePeriod>& in, s32 timePeriod, f64 standardDeviation, viz::VizEPriceField priceField, viz::VizEMovingAverageMethod movingAverageMethod);
	QVector<VizCTradePeriod> lowerBollingerBand(const QVector<VizCTradePeriod>& in, s32 timePeriod, f64 standardDeviation, viz::VizEPriceField priceField, viz::VizEMovingAverageMethod movingAverageMethod);

private:
	VizCTechnicalAnalysisFunctions();
	static VizCTechnicalAnalysisFunctions* Instance;
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
