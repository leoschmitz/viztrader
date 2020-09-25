//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCBollingerBandsIndicator.h"
#include "VizIGraphicsItemBasedOnShape.h"
#include "VizCWorkArea.h"
#include "VizCBollingerLowerBandIndicator.h"
#include "VizCBollingerUpperBandIndicator.h"
#include "VizCGraphicsPanel.h"
#include <QDebug>
#include <QVariant>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
VizCBollingerBandsIndicator::VizCBollingerBandsIndicator() :  VizIIndicator()
{
	blockSignals(true);
	resetToDefaults();
	blockSignals(false);
}
//-----------------------------------------------------------------------------
VizCBollingerBandsIndicator::~VizCBollingerBandsIndicator()
{
}
//-----------------------------------------------------------------------------
void VizCBollingerBandsIndicator::resetToDefaults()
{
	QStringList names;
	QVariantList oldValues;
	QVariantList newValues;

	gui::vizObjectGetAllPropertiesNamesAndValues(this, names, oldValues);

	VizIIndicator::resetToDefaults();
	
	TimePeriod = 10;
	PriceField = viz::VEPF_DEFAULT;
	MovingAverageMethod = viz::VEMAM_SIMPLE;
	HorizontalShift = 0;
	StandardDeviationDown = 1.0;

	VizCBollingerLowerBandIndicator* BollingerLowerBand = bollingerLowerBand();
	if(BollingerLowerBand)
	{
		BollingerLowerBand->blockSignals(true);
		BollingerLowerBand->setTimePeriod(TimePeriod);
		BollingerLowerBand->setPriceField(PriceField);
		BollingerLowerBand->setMovingAverageMethod(MovingAverageMethod);
		BollingerLowerBand->setHorizontalShift(HorizontalShift);
		BollingerLowerBand->setStandardDeviation(StandardDeviationDown);
		BollingerLowerBand->blockSignals(false);
	}
	StandardDeviationUp = 1.0;
	VizCBollingerUpperBandIndicator* BollingerUpperBand = bollingerUpperBand();
	if(BollingerUpperBand)
	{
		BollingerUpperBand->blockSignals(true);
		BollingerUpperBand->setTimePeriod(TimePeriod);
		BollingerUpperBand->setPriceField(PriceField);
		BollingerUpperBand->setMovingAverageMethod(MovingAverageMethod);
		BollingerUpperBand->setHorizontalShift(HorizontalShift);
		BollingerUpperBand->setStandardDeviation(StandardDeviationDown);
		BollingerUpperBand->blockSignals(false);
	}


	if(HorizontalScale && VerticalScale && WorkArea)
	{
		QStringList propertiesNames;
		propertiesNames << 	"timePeriod" << "priceField" << "movingAverageMethod" << "horizontalShift" << "standardDeviationUp" << "standardDeviationDown";
		WorkArea->updateProperties(this, propertiesNames);

		updateData();
		ajustPeriods();
	}

	gui::vizObjectGetAllPropertiesNamesAndValues(this, names, newValues);

	emit createUndoCommandPropertiesChanged(this, names, oldValues, newValues, true, tr("Valores Padrão"));
	emit itemChanged(true);
}
//-----------------------------------------------------------------------------
void VizCBollingerBandsIndicator::addRelativeItems(VizCGraphicsPanel* graphicsPanel)
{


	VizCBollingerLowerBandIndicator* lower = qobject_cast<VizCBollingerLowerBandIndicator*>(graphicsPanel->newGraphicsItemBasedOnSecurity("viz::gui::VizCBollingerLowerBandIndicator", SecuritySymbol));
	VizCBollingerUpperBandIndicator* upper = qobject_cast<VizCBollingerUpperBandIndicator*>(graphicsPanel->newGraphicsItemBasedOnSecurity("viz::gui::VizCBollingerUpperBandIndicator", SecuritySymbol));

	lower->setParentItem(this);
	upper->setParentItem(this);


	/*VizCHorizontalLine* line30 = qobject_cast<VizCHorizontalLine*>(graphicsPanel->newVizIGraphicsItemBasedOnShape("viz::gui::VizCHorizontalLine"));
	Q_ASSERT(line30);
	VizCHorizontalLine* line70 = qobject_cast<VizCHorizontalLine*>(graphicsPanel->newVizIGraphicsItemBasedOnShape("viz::gui::VizCHorizontalLine"));
	Q_ASSERT(line70);

	line30->setValue(30.0);
	line70->setValue(70.0);

	line30->setParentItem(this);
	line70->setParentItem(this);*/


}
//-----------------------------------------------------------------------------
VizCBollingerUpperBandIndicator* VizCBollingerBandsIndicator::bollingerUpperBand() const
{
	foreach(VizIGraphicsItem* item, ChildItems)
	{
		VizCBollingerUpperBandIndicator* upper = qobject_cast<VizCBollingerUpperBandIndicator*>(item);
		if(upper)
		{
			return upper;
		}
	}
	return 0;
}
//-----------------------------------------------------------------------------
VizCBollingerLowerBandIndicator* VizCBollingerBandsIndicator::bollingerLowerBand() const
{
	foreach(VizIGraphicsItem* item, ChildItems)
	{
		VizCBollingerLowerBandIndicator* lower = qobject_cast<VizCBollingerLowerBandIndicator*>(item);
		if(lower)
		{
			return lower;
		}
	}
	return 0;
}
//-----------------------------------------------------------------------------
f64 VizCBollingerBandsIndicator::standardDeviationUp() const
{
	VizCBollingerUpperBandIndicator* BollingerUpperBand = bollingerUpperBand();
	if(BollingerUpperBand)
	{
		return BollingerUpperBand->standardDeviation();
	}
	return StandardDeviationUp;
}
//-----------------------------------------------------------------------------
void VizCBollingerBandsIndicator::setStandardDeviationUp(f64 value)
{
	if(StandardDeviationUp != value)
	{
		f64 oldValue = StandardDeviationUp;
		
		StandardDeviationUp = value;
		VizCBollingerUpperBandIndicator* BollingerUpperBand = bollingerUpperBand();
		if(BollingerUpperBand)
		{
			BollingerUpperBand->blockSignals(true);
			BollingerUpperBand->setStandardDeviation(StandardDeviationUp);
			BollingerUpperBand->blockSignals(false);
			WorkArea->updateProperty(BollingerUpperBand, "standardDeviation");
		}
		
		emit createUndoCommandPropertyChanged(this, "standardDeviationUp", oldValue, StandardDeviationUp, true);
		emit itemChanged(true);
	}
}
//-----------------------------------------------------------------------------
f64 VizCBollingerBandsIndicator::standardDeviationDown() const
{
	VizCBollingerLowerBandIndicator* BollingerLowerBand = bollingerLowerBand();
	if(BollingerLowerBand)
	{
		return BollingerLowerBand->standardDeviation();
	}
	return StandardDeviationDown;
}
//-----------------------------------------------------------------------------
void VizCBollingerBandsIndicator::setStandardDeviationDown(f64 value)
{
	if(StandardDeviationDown != value)
	{
		f64 oldValue = StandardDeviationDown;

		StandardDeviationDown = value;
		VizCBollingerLowerBandIndicator* BollingerLowerBand = bollingerLowerBand();
		if(BollingerLowerBand)
		{
			BollingerLowerBand->blockSignals(true);
			BollingerLowerBand->setStandardDeviation(StandardDeviationDown);
			BollingerLowerBand->blockSignals(false);
			WorkArea->updateProperty(BollingerLowerBand, "standardDeviation");
		}

		emit createUndoCommandPropertyChanged(this, "standardDeviationDown", oldValue, StandardDeviationDown, true);
		emit itemChanged(true);
	}
}
//-----------------------------------------------------------------------------
s32 VizCBollingerBandsIndicator::timePeriod() const
{
	return TimePeriod;
}
//-----------------------------------------------------------------------------
void VizCBollingerBandsIndicator::setTimePeriod(s32 period)
{
	if(TimePeriod != period)
	{
		s32 oldValue = TimePeriod;

		TimePeriod = period;
		updateData();
		ajustPeriods();
		VizCBollingerUpperBandIndicator* BollingerUpperBand = bollingerUpperBand();
		VizCBollingerLowerBandIndicator* BollingerLowerBand = bollingerLowerBand();
		if(BollingerUpperBand)
		{
			BollingerUpperBand->blockSignals(true);
			BollingerUpperBand->setTimePeriod(TimePeriod);
			BollingerUpperBand->blockSignals(false);
			WorkArea->updateProperty(BollingerUpperBand, "timePeriod");
		}
		if(BollingerLowerBand)
		{
			BollingerLowerBand->blockSignals(true);
			BollingerLowerBand->setTimePeriod(TimePeriod);
			BollingerLowerBand->blockSignals(false);
			WorkArea->updateProperty(BollingerLowerBand, "timePeriod");
		}

		emit createUndoCommandPropertyChanged(this, "timePeriod", oldValue, TimePeriod, true);
		emit itemChanged(true);
	}
}
//-----------------------------------------------------------------------------
VizEPriceField VizCBollingerBandsIndicator::priceField() const
{
	return PriceField;
}
//-----------------------------------------------------------------------------
void VizCBollingerBandsIndicator::setPriceField(VizEPriceField field)
{
	if(PriceField != field)
	{

		VizEPriceField oldValue = PriceField;

		PriceField = field;
		updateData();
		ajustPeriods();
		VizCBollingerUpperBandIndicator* BollingerUpperBand = bollingerUpperBand();
		VizCBollingerLowerBandIndicator* BollingerLowerBand = bollingerLowerBand();
		if(BollingerUpperBand)
		{
			BollingerUpperBand->blockSignals(true);
			BollingerUpperBand->setPriceField(PriceField);
			BollingerUpperBand->blockSignals(false);
			WorkArea->updateProperty(BollingerUpperBand, "priceField");
		}
		if(BollingerLowerBand)
		{
			BollingerLowerBand->blockSignals(true);
			BollingerLowerBand->setPriceField(PriceField);
			BollingerLowerBand->blockSignals(false);
			WorkArea->updateProperty(BollingerLowerBand, "priceField");
		}
		
		emit createUndoCommandPropertyChanged(this, "priceField", oldValue, PriceField, true);
		emit itemChanged(true);
	}
}
//-----------------------------------------------------------------------------
VizEMovingAverageMethod VizCBollingerBandsIndicator::movingAverageMethod() const
{
	return MovingAverageMethod;
}
//-----------------------------------------------------------------------------
void VizCBollingerBandsIndicator::setMovingAverageMethod(VizEMovingAverageMethod method)
{
	if(MovingAverageMethod != method)
	{

		VizEMovingAverageMethod oldValue = MovingAverageMethod;

		MovingAverageMethod = method;
		updateData();
		ajustPeriods();
		VizCBollingerUpperBandIndicator* BollingerUpperBand = bollingerUpperBand();
		VizCBollingerLowerBandIndicator* BollingerLowerBand = bollingerLowerBand();
		if(BollingerUpperBand)
		{
			BollingerUpperBand->blockSignals(true);
			BollingerUpperBand->setMovingAverageMethod(MovingAverageMethod);
			BollingerUpperBand->blockSignals(false);
			WorkArea->updateProperty(BollingerUpperBand, "movingAverageMethod");
		}
		if(BollingerLowerBand)
		{
			BollingerLowerBand->blockSignals(true);
			BollingerLowerBand->setMovingAverageMethod(MovingAverageMethod);
			BollingerLowerBand->blockSignals(false);
			WorkArea->updateProperty(BollingerLowerBand, "movingAverageMethod");
		}

		emit createUndoCommandPropertyChanged(this, "movingAverageMethod", oldValue, MovingAverageMethod, true);
		emit itemChanged(true);
	}
}
//-----------------------------------------------------------------------------
s32 VizCBollingerBandsIndicator::horizontalShift() const
{
	return HorizontalShift;
}
//-----------------------------------------------------------------------------
void VizCBollingerBandsIndicator::setHorizontalShift(s32 value)
{
	if(HorizontalShift != value)
	{
		s32 oldValue = HorizontalShift;

		HorizontalShift = value;
		VizCBollingerUpperBandIndicator* BollingerUpperBand = bollingerUpperBand();
		VizCBollingerLowerBandIndicator* BollingerLowerBand = bollingerLowerBand();
		if(BollingerUpperBand)
		{
			BollingerUpperBand->blockSignals(true);
			BollingerUpperBand->setHorizontalShift(HorizontalShift);
			BollingerUpperBand->blockSignals(false);
			WorkArea->updateProperty(BollingerUpperBand, "horizontalShift");
		}
		if(BollingerLowerBand)
		{
			BollingerLowerBand->blockSignals(true);
			BollingerLowerBand->setHorizontalShift(HorizontalShift);
			BollingerLowerBand->blockSignals(false);
			WorkArea->updateProperty(BollingerLowerBand, "horizontalShift");
		}

		emit createUndoCommandPropertyChanged(this, "horizontalShift", oldValue, HorizontalShift, true);
		emit itemChanged(true);
	}
}
//-----------------------------------------------------------------------------
void VizCBollingerBandsIndicator::build()
{
	
	s32 rulesValuesIndexBegin;
	s32 rulesValuesIndexEnd;
	HorizontalScale->currentVisibleRulerValuesIndex(rulesValuesIndexBegin, rulesValuesIndexEnd);
	shiftRulesValuesIndex(HorizontalShift, rulesValuesIndexBegin, rulesValuesIndexEnd);
	switch(IndicatorStyle)
	{
		case VEIS_POINTS:
		{
			buildPoints(rulesValuesIndexBegin, rulesValuesIndexEnd);
			break;
		}
		case VEIS_LINES:
		{
			buildLines(rulesValuesIndexBegin, rulesValuesIndexEnd);
			break;
		}
		case VEIS_HISTOGRAM:
		{
			buildHistogram(rulesValuesIndexBegin, rulesValuesIndexEnd);
			break;
		}
	};
	applyShiftProperties();

}
//-----------------------------------------------------------------------------
void VizCBollingerBandsIndicator::applyShiftProperties()
{

	for(s32 i = 0; i < Points.size(); i++)
	{
		Points[i].setX(Points[i].x() + HorizontalShift);
	}

}
//-----------------------------------------------------------------------------
void VizCBollingerBandsIndicator::updateData()
{
	const QVector<core::VizCSecurityPeriod>& spd = DataFacade->securityPeriodsDaily(SecuritySymbol);
	//aqui tem que vir o start e o end date do HorizontalScale
	Periods = SecurityPeriodsManipulator->convertPeriodicityInteval(spd, HorizontalScale->periodicity());
	Periods = TechnicalAnalysisFunctions->middleBollingerBand(Periods, TimePeriod, PriceField, MovingAverageMethod);

	//QString name = tr("Bandas de Bollinger (middle)");
	
	QString name = tr("Bandas de Bollinger");
	QString t = QString("%1 (%2)").arg(name).arg(SecuritySymbol);
	vizObjectSetName(this, t);
	
}
//-----------------------------------------------------------------------------
void VizCBollingerBandsIndicator::yRange(bool& isEmpty, f64& minY, f64& maxY) const
{
	s32 rulesValuesIndexBegin;
	s32 rulesValuesIndexEnd;
	HorizontalScale->currentVisibleRulerValuesIndex(rulesValuesIndexBegin, rulesValuesIndexEnd);
	shiftRulesValuesIndex(HorizontalShift, rulesValuesIndexBegin, rulesValuesIndexEnd);
	for(s32 i = rulesValuesIndexBegin; i <= rulesValuesIndexEnd; i++)
	{
		core::VizCTradePeriod Period = Periods[i];
		if(!Period.isValid())
		{
			continue;
		}
		maxY = qMax<f64>(maxY, Period.IndicatorValue);
		minY = qMin<f64>(minY, Period.IndicatorValue);
		isEmpty = false;
	}

}
//-----------------------------------------------------------------------------
QString VizCBollingerBandsIndicator::toolTip(const QPointF& p) const
{
	s32 HS = p.x() - HorizontalShift;
	const core::VizCTradePeriod& period = Periods[HS];
	const QDateTime& dateTime = Periods[p.x()].TimeStamp;

	QString method;
	QString priceField;

	switch(PriceField)
	{
		case viz::VEPF_OPEN_PRICE:
		{
			priceField = tr("Abertura");
			break;
		}
		case viz::VEPF_CLOSE_PRICE:
		{
			priceField = tr("Fechamento");
			break;
		}
		case viz::VEPF_MAX_PRICE:
		{
			priceField = tr("Máximo");
			break;
		}
		case viz::VEPF_MIN_PRICE:
		{
			priceField = ("Mínimo");
			break;
		}
	};
	switch(MovingAverageMethod)
	{
		case viz::VEMAM_SIMPLE:
		{
			method = tr("Simples");
			break;
		}
		case viz::VEMAM_EXPONENTIAL:
		{
			method = tr("Exponencial");
			break;
		}
		case viz::VEMAM_WEIGHTED:
		{
			method = tr("Com Pesos");
			break;
		}
		case viz::VEMAM_DOUBLE_EXPONENTIAL:
		{
			method = tr("Duplamente Exponencial");
			break;
		}
		case viz::VEMAM_TRIPLE_EXPONENTIAL:
		{
			method = tr("Triplamente Exponencial");
			break;
		}
		case viz::VEMAM_TRIANGULAR:
		{
			method = tr("Triangular");
			break;
		}
		case viz::VEMAM_KAYFMAN:
		{
			method = tr("Kayfman");
			break;
		}
		case viz::VEMAM_MESA:
		{
			method = tr("MESA");
			break;
		}
		case viz::VEMAM_T3:
		{
			method = "T3";
			break;
		}
	};

	QString n = vizObjectName(this);

	QString s = QString("%L1\nMethod: %L2\nPrice Field: %L3\nTime Period: %L4\nValue: %L5\nDate: %L6")
		.arg(n)
		.arg(method)
		.arg(priceField)
		.arg(TimePeriod)
		.arg(period.IndicatorValue, 0, 'f', 2)
		.arg(dateTime.date().toString(Qt::SystemLocaleShortDate));

	return s;
}
//-----------------------------------------------------------------------------
void VizCBollingerBandsIndicator::ajustToOverlayWithoutScale(f64 itemMinY, f64 itemMaxY)
{
	VizIIndicator::ajustToOverlayWithoutScale(itemMinY, itemMaxY);
}
//-----------------------------------------------------------------------------
int VizCBollingerBandsIndicator::propertyGetPriceField() const
{
	return static_cast<int>(priceField());
}
//-----------------------------------------------------------------------------
void VizCBollingerBandsIndicator::propertySetPriceField(int value)
{
	setPriceField(static_cast<VizEPriceField>(value));
}
//-----------------------------------------------------------------------------
int VizCBollingerBandsIndicator::propertyGetMovingAverageMethod() const
{
	return static_cast<int>(movingAverageMethod());
}
//-----------------------------------------------------------------------------
void VizCBollingerBandsIndicator::propertySetMovingAverageMethod(int value)
{
	setMovingAverageMethod(static_cast<VizEMovingAverageMethod>(value));
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
