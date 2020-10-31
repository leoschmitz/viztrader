//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCMovingAverageIndicator.h"
#include "VizIGraphicsItemBasedOnShape.h"
#include "VizCWorkArea.h"
#include <QDebug>
#include <QVariant>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
VizCMovingAverageIndicator::VizCMovingAverageIndicator() :  VizIIndicator()
{
	blockSignals(true);
	resetToDefaults();
	blockSignals(false);
}
//-----------------------------------------------------------------------------
void VizCMovingAverageIndicator::resetToDefaults()
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
	VerticalShift = 0.0;
	HorizontalShift = 0;
	
	if(HorizontalScale && VerticalScale && WorkArea)
	{
		QStringList propertiesNames;
		propertiesNames << 	"timePeriod" << "priceField" << "movingAverageMethod" << "verticalShift" << "horizontalShift";
		WorkArea->updateProperties(this, propertiesNames);
		updateData();
		ajustPeriods();
	}

	gui::vizObjectGetAllPropertiesNamesAndValues(this, names, newValues);

	emit createUndoCommandPropertiesChanged(this, names, oldValues, newValues, true, tr("Valores Padrão"));
	emit itemChanged(true);
}
//-----------------------------------------------------------------------------
VizCMovingAverageIndicator::~VizCMovingAverageIndicator()
{
}
//-----------------------------------------------------------------------------
s32 VizCMovingAverageIndicator::timePeriod() const
{
	return TimePeriod;
}
//-----------------------------------------------------------------------------
void VizCMovingAverageIndicator::setTimePeriod(s32 period)
{
	if(TimePeriod != period)
	{
		s32 oldValue = TimePeriod;

		TimePeriod = period;
		updateData();
		ajustPeriods();

		emit createUndoCommandPropertyChanged(this, "timePeriod", oldValue, TimePeriod, true);
		emit itemChanged(true);
	}
}
//-----------------------------------------------------------------------------
VizEPriceField VizCMovingAverageIndicator::priceField() const
{
	return PriceField;
}
//-----------------------------------------------------------------------------
void VizCMovingAverageIndicator::setPriceField(VizEPriceField field)
{
	if(PriceField != field)
	{
		VizEPriceField oldValue = PriceField;

		PriceField = field;
		updateData();
		ajustPeriods();

		emit createUndoCommandPropertyChanged(this, "priceField", oldValue, PriceField, true);
		emit itemChanged(true);
	}
}
//-----------------------------------------------------------------------------
VizEMovingAverageMethod VizCMovingAverageIndicator::movingAverageMethod() const
{
	return MovingAverageMethod;
}
//-----------------------------------------------------------------------------
void VizCMovingAverageIndicator::setMovingAverageMethod(VizEMovingAverageMethod method)
{
	if(MovingAverageMethod != method)
	{
		VizEMovingAverageMethod oldValue = MovingAverageMethod;

		MovingAverageMethod = method;
		updateData();
		ajustPeriods();

		emit createUndoCommandPropertyChanged(this, "movingAverageMethod", oldValue, MovingAverageMethod, true);
		emit itemChanged(true);
	}
}
//-----------------------------------------------------------------------------
s32 VizCMovingAverageIndicator::horizontalShift() const
{
	return HorizontalShift;
}
//-----------------------------------------------------------------------------
void VizCMovingAverageIndicator::setHorizontalShift(s32 value)
{
	if(HorizontalShift != value)
	{
		s32 oldValue = HorizontalShift;

		HorizontalShift = value;

		emit createUndoCommandPropertyChanged(this, "horizontalShift", oldValue, HorizontalShift, true);
		emit itemChanged(true);
	}
}
//-----------------------------------------------------------------------------
f64 VizCMovingAverageIndicator::verticalShift() const
{
	return VerticalShift;
}
//-----------------------------------------------------------------------------
void VizCMovingAverageIndicator::setVerticalShift(f64 value)
{
	if(VerticalShift != value)
	{
		f64 oldValue = VerticalShift;

		VerticalShift = value;

		emit createUndoCommandPropertyChanged(this, "verticalShift", oldValue, VerticalShift, true);
		emit itemChanged(true);
	}
}
//-----------------------------------------------------------------------------
void VizCMovingAverageIndicator::build()
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
void VizCMovingAverageIndicator::applyShiftProperties()
{

	for(s32 i = 0; i < Points.size(); i++)
	{
		Points[i].setX(Points[i].x() + HorizontalShift);
	}

	if(ScaleInYOptions != viz::VESIYO_OVERLAY_WITHOUT_SCALE)
	{
		for(s32 i = 0; i < Points.size(); i++)
		{
			Points[i].setY(Points[i].y() + (Points[i].y() * (VerticalShift * 0.01)));
		}
	}

}
//-----------------------------------------------------------------------------
void VizCMovingAverageIndicator::updateData()
{
	const QVector<core::VizCSecurityPeriod>& spd = DataFacade->securityPeriodsDaily(SecuritySymbol);
	//aqui tem que vir o start e o end date do HorizontalScale
	Periods = SecurityPeriodsManipulator->convertPeriodicityInteval(spd, HorizontalScale->periodicity());
	Periods = TechnicalAnalysisFunctions->movingAverage(Periods, TimePeriod, PriceField, MovingAverageMethod);

	
	QString name = tr("Média Móvel");
	QString t = QString("%1 (%2)").arg(name).arg(SecuritySymbol);
	vizObjectSetName(this, t);
	
}
//-----------------------------------------------------------------------------
void VizCMovingAverageIndicator::yRange(bool& isEmpty, f64& minY, f64& maxY) const
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


	if(ScaleInYOptions != viz::VESIYO_OVERLAY_WITHOUT_SCALE)
	{
		minY = minY + (minY * (VerticalShift * 0.01f));
		maxY = maxY + (maxY * (VerticalShift * 0.01f));
	}
}
//-----------------------------------------------------------------------------
QString VizCMovingAverageIndicator::toolTip(const QPointF& p) const
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
int VizCMovingAverageIndicator::propertyGetPriceField() const
{
	return static_cast<int>(priceField());
}
//-----------------------------------------------------------------------------
void VizCMovingAverageIndicator::propertySetPriceField(int value)
{
	setPriceField(static_cast<VizEPriceField>(value));
}
//-----------------------------------------------------------------------------
int VizCMovingAverageIndicator::propertyGetMovingAverageMethod() const
{
	return static_cast<int>(movingAverageMethod());
}
//-----------------------------------------------------------------------------
void VizCMovingAverageIndicator::propertySetMovingAverageMethod(int value)
{
	setMovingAverageMethod(static_cast<VizEMovingAverageMethod>(value));
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
