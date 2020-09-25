//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCVolumeIndicator.h"
#include "VizCTechnicalAnalysisFunctions.h"
#include "VizCWorkArea.h"
#include <QDebug>
#include <QGraphicsSceneHoverEvent>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
VizCVolumeIndicator::VizCVolumeIndicator() :  VizIIndicator()
{
	blockSignals(true);
	resetToDefaults();
	blockSignals(false);
}
//-----------------------------------------------------------------------------
VizCVolumeIndicator::~VizCVolumeIndicator()
{
}
//-----------------------------------------------------------------------------
void VizCVolumeIndicator::resetToDefaults()
{
	QStringList names;
	QVariantList oldValues;
	QVariantList newValues;

	gui::vizObjectGetAllPropertiesNamesAndValues(this, names, oldValues);

	VizIIndicator::resetToDefaults();

	if(HorizontalScale && VerticalScale)
	{
		updateData();
		ajustPeriods();
	}

	gui::vizObjectGetAllPropertiesNamesAndValues(this, names, newValues);

	emit createUndoCommandPropertiesChanged(this, names, oldValues, newValues, true, tr("Valores Padrão"));
	emit itemChanged(true);
}
//-----------------------------------------------------------------------------
void VizCVolumeIndicator::updateData()
{

	const QVector<core::VizCSecurityPeriod>& spd = DataFacade->securityPeriodsDaily(SecuritySymbol);
	//aqui tem que vir o start e o end date do HorizontalScale
	Periods = SecurityPeriodsManipulator->convertPeriodicityInteval(spd, HorizontalScale->periodicity());
	Periods = TechnicalAnalysisFunctions->volume(Periods);

	QString name = tr("Volume");
	QString t = QString("%1 (%2)").arg(name).arg(SecuritySymbol);
	vizObjectSetName(this, t);
}
//-----------------------------------------------------------------------------
QString VizCVolumeIndicator::toolTip(const QPointF& p) const
{
	const core::VizCTradePeriod& period = Periods[p.x()];
	const QDateTime& dateTime = period.TimeStamp;
	QString method;

	QString n = vizObjectName(this);

	QString s = QString("%L1\nValue: %L3\nDate: %L4")
		.arg(n)
		.arg(period.IndicatorValue, 0, 'f', 2)
		.arg(dateTime.date().toString(Qt::SystemLocaleShortDate));


	return s;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
