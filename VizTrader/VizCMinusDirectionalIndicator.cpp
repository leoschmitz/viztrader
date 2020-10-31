//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCMinusDirectionalIndicator.h"
#include "VizCWorkArea.h"
#include <QDebug>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
VizCMinusDirectionalIndicator::VizCMinusDirectionalIndicator() :  VizIIndicator()
{
	blockSignals(true);
	resetToDefaults();
	blockSignals(false);
}
//-----------------------------------------------------------------------------
VizCMinusDirectionalIndicator::~VizCMinusDirectionalIndicator()
{
}
//-----------------------------------------------------------------------------
void VizCMinusDirectionalIndicator::resetToDefaults()
{
	QStringList names;
	QVariantList oldValues;
	QVariantList newValues;

	gui::vizObjectGetAllPropertiesNamesAndValues(this, names, oldValues);

	VizIIndicator::resetToDefaults();

	TimePeriod = 10;

	if(HorizontalScale && VerticalScale && WorkArea)
	{
		WorkArea->updateProperty(this, "timePeriod");
		updateData();
		ajustPeriods();
	}

	gui::vizObjectGetAllPropertiesNamesAndValues(this, names, newValues);

	emit createUndoCommandPropertiesChanged(this, names, oldValues, newValues, true, tr("Valores Padrão"));
	emit itemChanged(true);
}
//-----------------------------------------------------------------------------
s32 VizCMinusDirectionalIndicator::timePeriod() const
{
	return TimePeriod;
}
//-----------------------------------------------------------------------------
void VizCMinusDirectionalIndicator::setTimePeriod(s32 period)
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
void VizCMinusDirectionalIndicator::updateData()
{
	const QVector<core::VizCSecurityPeriod>& spd = DataFacade->securityPeriodsDaily(SecuritySymbol);
	//aqui tem que vir o start e o end date do HorizontalScale
	Periods = SecurityPeriodsManipulator->convertPeriodicityInteval(spd, HorizontalScale->periodicity());
	Periods = TechnicalAnalysisFunctions->minusDirectionalIndicator(Periods, TimePeriod);
	
	//QString name = tr("Minus Directional Indicator -DI");
	QString name = tr("-DI");
	QString t = QString("%1 (%2)").arg(name).arg(SecuritySymbol);
	vizObjectSetName(this, t);
	
}
//-----------------------------------------------------------------------------
QString VizCMinusDirectionalIndicator::toolTip(const QPointF& p) const
{
	const core::VizCTradePeriod& period = Periods[p.x()];
	const QDateTime& dateTime = period.TimeStamp;

	QString n = vizObjectName(this);

	QString s = QString("%L1\nTime Period: %L2\nValue: %L3\nDate: %L4")
		.arg(n)
		.arg(TimePeriod)
		.arg(period.IndicatorValue, 0, 'f', 2)
		.arg(dateTime.date().toString(Qt::SystemLocaleShortDate));


	return s;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
