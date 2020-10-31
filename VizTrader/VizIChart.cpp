//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizIChart.h"
#include "VizMath.h"
#include <QDebug>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
VizIChart::VizIChart() : VizIGraphicsItemBasedOnSecurity()
{
	blockSignals(true);
	resetToDefaults();
	blockSignals(false);
}
//-----------------------------------------------------------------------------
VizIChart::~VizIChart()
{
}
//-----------------------------------------------------------------------------
void VizIChart::resetToDefaults()
{
	QStringList names;
	QVariantList oldValues;
	QVariantList newValues;

	gui::vizObjectGetAllPropertiesNamesAndValues(this, names, oldValues);

	VizIGraphicsItemBasedOnSecurity::resetToDefaults();
	PenWidth = static_cast<viz::VizEPenWidth>(Settings->value("Appearance/Charts/LineWidth").toInt());
	UpColor = QColor(Settings->value("Appearance/Charts/UpColor").toString());
	DownColor = QColor(Settings->value("Appearance/Charts/DownColor").toString());

	gui::vizObjectGetAllPropertiesNamesAndValues(this, names, newValues);

	emit createUndoCommandPropertiesChanged(this, names, oldValues, newValues, true, tr("Valores Padrão"));
	emit itemChanged(true);
}
//-----------------------------------------------------------------------------
void VizIChart::updateData()
{

	const QVector<core::VizCSecurityPeriod>& spd = DataFacade->securityPeriodsDaily(SecuritySymbol);
	//aqui tem que vir o start e o end date do HorizontalScale
	Periods = SecurityPeriodsManipulator->convertPeriodicityInteval(spd, HorizontalScale->periodicity());

	QString t = QString("%1").arg(SecuritySymbol);
	vizObjectSetName(this, t);
}
//-----------------------------------------------------------------------------
QString VizIChart::toPlainText()
{
	QString text;
	QTextStream stream(&text);

	stream
		<< tr("Data") << '\t'
		<< tr("Abertura") << '\t'
		<< tr("Maior") << '\t'
		<< tr("Menor") << '\t'
		<< tr("Fechamento") << '\t'
		<< tr("Volume")
		<< '\n';

	foreach(core::VizCTradePeriod period, Periods)
	{
		QString date = QString("%1").arg(period.TimeStamp.date().toString(Qt::SystemLocaleShortDate));
		stream << date << '\t';

		bool ok = period.isValid();
		QString OpenPrice = ok ? QString("%1").arg( period.OpenPrice, 0, 'f', 2) : "";
		QString MaxPrice = ok ? QString("%1").arg( period.MaxPrice, 0, 'f', 2) : "";
		QString MinPrice = ok ? QString("%1").arg( period.MinPrice, 0, 'f', 2) : "";
		QString ClosePrice = ok ? QString("%1").arg( period.ClosePrice, 0, 'f', 2) : "";
		QString TotalVolume = ok ? QString("%1").arg( period.TotalVolume, 0, 'f', 2) : "";

		stream << OpenPrice << '\t' << MaxPrice << '\t' << MinPrice << '\t' << ClosePrice << '\t' << TotalVolume << '\n';
	}

	return text;
}
//-----------------------------------------------------------------------------
QColor VizIChart::upColor() const
{
	return UpColor;
}
//-----------------------------------------------------------------------------
QColor VizIChart::downColor() const
{
	return DownColor;
}
//-----------------------------------------------------------------------------
void VizIChart::setUpColor(const QColor& color)
{
	if(color != UpColor)
	{
		QColor oldValue = UpColor;

		UpColor = color;

		emit createUndoCommandPropertyChanged(this, "upColor", oldValue, UpColor, true);
		emit itemChanged();
	}

}
//-----------------------------------------------------------------------------
void VizIChart::setDownColor(const QColor& color)
{
	if(color != DownColor)
	{
		QColor oldValue = DownColor;
		
		DownColor = color;

		emit createUndoCommandPropertyChanged(this, "downColor", oldValue, DownColor, true);
		emit itemChanged();
	}
}
//-----------------------------------------------------------------------------
void VizIChart::ajustPeriods()
{
	Periods = HorizontalScale->ajustPeriods(Periods);
}
//-----------------------------------------------------------------------------
void VizIChart::nearestYValue(const QPointF& pos, f64& value, bool& isValid)
{
	f64 dist = std::numeric_limits<f64>::max();
	const core::VizCTradePeriod& p = Periods[pos.x()];
	isValid = p.isValid();

	if(!isValid)
		return;

	f64 y = 0.0;
	f64 _dist = 0.0;
	_dist = core::fabs(pos.y() - p.ClosePrice);
	if(_dist < dist)
	{
		y = p.ClosePrice;
		dist = _dist;
	}
	_dist = core::fabs(pos.y() - p.OpenPrice);
	if(_dist < dist)
	{
		y = p.OpenPrice;
		dist = _dist;
	}
	_dist = core::fabs(pos.y() - p.MaxPrice);
	if(_dist < dist)
	{
		y = p.MaxPrice;
		dist = _dist;
	}
	_dist = core::fabs(pos.y() - p.MinPrice);
	if(_dist < dist)
	{
		y = p.MinPrice;
		dist = _dist;
	}
	
	value = y;
}
//-----------------------------------------------------------------------------
void VizIChart::yRange(bool& isEmpty, f64& minY, f64& maxY) const
{
	s32 rulesValuesIndexBegin;
	s32 rulesValuesIndexEnd;
	HorizontalScale->currentVisibleRulerValuesIndex(rulesValuesIndexBegin, rulesValuesIndexEnd);
	for(s32 i = rulesValuesIndexBegin; i <= rulesValuesIndexEnd; i++)
	{
		core::VizCTradePeriod Period = Periods[i];
		if(!Period.isValid())
		{
			continue;
		}
		maxY = qMax<f64>(maxY, Period.MaxPrice);
		minY = qMin<f64>(minY, Period.MinPrice);
		isEmpty = false;
	}
}
//-----------------------------------------------------------------------------
void VizIChart::lastTrade(f64& value, bool& isValid) const
{
	for(s32 i = Periods.size() - 1; i >= 0; i--)
	{
		if(Periods[i].isValid())
		{
			isValid = true;
			value = Periods[i].ClosePrice;
			break;
		}
	}
}
//-----------------------------------------------------------------------------
QString VizIChart::toolTip(const QPointF& p) const
{
	const core::VizCTradePeriod& period = Periods[p.x()];
	const QDateTime& dateTime = period.TimeStamp;
	QString s = QString("%L1\nDate: %L2\nOpen: %L3\nHigh: %L4\nLow: %L5\nClose: %L6\nVolume: %L7")
		.arg(SecuritySymbol)
		.arg(period.TimeStamp.date().toString(Qt::SystemLocaleShortDate))
		.arg(period.OpenPrice, 0, 'f', 2)
		.arg(period.MaxPrice, 0, 'f', 2)
		.arg(period.MinPrice, 0, 'f', 2)
		.arg(period.ClosePrice, 0, 'f', 2)
		.arg(period.TotalVolume, 0, 'f', 2);

	return s;
}
//-----------------------------------------------------------------------------
QString VizIChart::nameToTitleBar(bool showLastTradeValues, bool useColorOfPlot) const
{
	QString out = vizObjectName(this);

	if(showLastTradeValues)
	{
		f64 diff = lastTradeDifference();
		if(useColorOfPlot)
		{
			QColor c = DownColor;
			if(diff >= 0)
			{
				c = UpColor;
			}
			out += QString("<font color=\"%1\">").arg(c.name());
		}
		for(s32 i = Periods.size() - 1; i >= 0; i--)
		{
			if(Periods[i].isValid())
			{
				out += " (";
				//out += QString::number(Periods[i].OpenPrice);
				out += QString("%L1").arg(Periods[i].OpenPrice, 0, 'f', 2);
				out += ", ";
				//out += QString::number(Periods[i].MaxPrice);
				out += QString("%L1").arg(Periods[i].MaxPrice, 0, 'f', 2);
				out += ", ";
				//out += QString::number(Periods[i].MinPrice);
				out += QString("%L1").arg(Periods[i].MinPrice, 0, 'f', 2);
				out += ", ";
				//out += QString::number(Periods[i].ClosePrice);
				out += QString("%L1").arg(Periods[i].ClosePrice, 0, 'f', 2);
				out += ", ";
				out += diff >= 0 ? "+" : "";
				//out += QString::number(diff);
				out += QString("%L1").arg(diff, 0, 'f', 2);
				out += ")";
				break;
			}
		}
		if(useColorOfPlot)
		{
			out += "</font>";
		}
	}

	return out;

}
//-----------------------------------------------------------------------------
f64 VizIChart::lastTradeDifference() const
{
	core::VizCTradePeriod p1, p2;
	bool find1 = false;
	bool find2 = false;

	for(s32 i = Periods.size() - 1; i >= 0; i--)
	{
		if(!find1)
		{
			if(Periods[i].isValid())
			{
				p1 = Periods[i];
				find1 = true;
				continue;
			}
		}
		if(!find2)
		{
			if(Periods[i].isValid())
			{
				p2 = Periods[i];
				find2 = true;
				break;
			}
		}
	}

	if(find1 && find2)
	{
		return p1.ClosePrice - p2.ClosePrice;
	}

	return 0;

}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
