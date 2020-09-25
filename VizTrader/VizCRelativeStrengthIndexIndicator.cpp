//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCRelativeStrengthIndexIndicator.h"
#include "VizCWorkArea.h"
#include "VizCHorizontalLine.h"
#include "VizCGraphicsPanel.h"
#include <QDebug>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
VizCRelativeStrengthIndexIndicator::VizCRelativeStrengthIndexIndicator() :  VizIIndicator()
{
	blockSignals(true);
	resetToDefaults();
	blockSignals(false);
}
//-----------------------------------------------------------------------------
void VizCRelativeStrengthIndexIndicator::resetToDefaults()
{
	QStringList names;
	QVariantList oldValues;
	QVariantList newValues;

	gui::vizObjectGetAllPropertiesNamesAndValues(this, names, oldValues);

	VizIIndicator::resetToDefaults();

	TimePeriod = 10;
	PriceField = viz::VEPF_DEFAULT;


	if(HorizontalScale && VerticalScale && WorkArea)
	{
		QStringList propertiesNames;
		propertiesNames << 	"timePeriod" << "priceField";
		WorkArea->updateProperties(this, propertiesNames);

		updateData();
		ajustPeriods();
	}

	gui::vizObjectGetAllPropertiesNamesAndValues(this, names, newValues);

	emit createUndoCommandPropertiesChanged(this, names, oldValues, newValues, true, tr("Valores Padrão"));
	emit itemChanged(true);
}
//-----------------------------------------------------------------------------
VizCRelativeStrengthIndexIndicator::~VizCRelativeStrengthIndexIndicator()
{
}
//-----------------------------------------------------------------------------
void VizCRelativeStrengthIndexIndicator::addRelativeItems(VizCGraphicsPanel* graphicsPanel)
{

	VizCHorizontalLine* line30 = qobject_cast<VizCHorizontalLine*>(graphicsPanel->newVizIGraphicsItemBasedOnShape("viz::gui::VizCHorizontalLine"));
	Q_ASSERT(line30);
	VizCHorizontalLine* line70 = qobject_cast<VizCHorizontalLine*>(graphicsPanel->newVizIGraphicsItemBasedOnShape("viz::gui::VizCHorizontalLine"));
	Q_ASSERT(line70);
	
	line30->setValue(30.0);
	line70->setValue(70.0);

	line30->setParentItem(this);
	line70->setParentItem(this);

}
//-----------------------------------------------------------------------------
s32 VizCRelativeStrengthIndexIndicator::timePeriod() const
{
	return TimePeriod;
}
//-----------------------------------------------------------------------------
void VizCRelativeStrengthIndexIndicator::setTimePeriod(s32 period)
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
VizEPriceField VizCRelativeStrengthIndexIndicator::priceField() const
{
	return PriceField;
}
//-----------------------------------------------------------------------------
void VizCRelativeStrengthIndexIndicator::setPriceField(VizEPriceField field)
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
void VizCRelativeStrengthIndexIndicator::updateData()
{
	const QVector<core::VizCSecurityPeriod>& spd = DataFacade->securityPeriodsDaily(SecuritySymbol);
	//aqui tem que vir o start e o end date do HorizontalScale
	Periods = SecurityPeriodsManipulator->convertPeriodicityInteval(spd, HorizontalScale->periodicity());
	Periods = TechnicalAnalysisFunctions->relativeStrengthIndex(Periods, TimePeriod, PriceField);

	
	QString name = tr("Relative Strength Index");
	QString t = QString("%1 (%2)").arg(name).arg(SecuritySymbol);
	vizObjectSetName(this, t);
	
}
//-----------------------------------------------------------------------------
QString VizCRelativeStrengthIndexIndicator::toolTip(const QPointF& p) const
{
	const core::VizCTradePeriod& period = Periods[p.x()];
	const QDateTime& dateTime = period.TimeStamp;
	QString priceField;

	switch(PriceField)
	{
		case viz::VEPF_OPEN_PRICE:
		{
			priceField = "Open";
			break;
		}
		case viz::VEPF_CLOSE_PRICE:
		{
			priceField = "Close";
			break;
		}
		case viz::VEPF_MAX_PRICE:
		{
			priceField = "Max";
			break;
		}
		case viz::VEPF_MIN_PRICE:
		{
			priceField = "Min";
			break;
		}
	};

	QString n = vizObjectName(this);

	QString s = QString("%L1\nPrice Field: %L2\nTime Period: %L3\nValue: %L4\nDate: %L5")
		.arg(n)
		.arg(priceField)
		.arg(TimePeriod)
		.arg(period.IndicatorValue, 0, 'f', 2)
		.arg(dateTime.date().toString(Qt::SystemLocaleShortDate));

	return s;
}
//-----------------------------------------------------------------------------
int VizCRelativeStrengthIndexIndicator::propertyGetPriceField() const
{
	return static_cast<int>(priceField());
}
//-----------------------------------------------------------------------------
void VizCRelativeStrengthIndexIndicator::propertySetPriceField(int value)
{
	setPriceField(static_cast<VizEPriceField>(value));
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
