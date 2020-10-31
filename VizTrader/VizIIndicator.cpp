//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizIIndicator.h"
#include "VizIGraphicsItemBasedOnShape.h"
#include "VizGUIUtils.h"
#include "VizCWorkArea.h"
#include <QDebug>
#include <QTextStream>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
VizIIndicator::VizIIndicator() : VizIGraphicsItemBasedOnSecurity()
{
	TechnicalAnalysisFunctions = core::VizCTechnicalAnalysisFunctions::instance();
	blockSignals(true);
	resetToDefaults();
	blockSignals(false);
}
//-----------------------------------------------------------------------------
VizIIndicator::~VizIIndicator()
{
}
//-----------------------------------------------------------------------------
void VizIIndicator::resetToDefaults()
{
	QStringList names;
	QVariantList oldValues;
	QVariantList newValues;

	gui::vizObjectGetAllPropertiesNamesAndValues(this, names, oldValues);

	VizIGraphicsItemBasedOnSecurity::resetToDefaults();
	PenWidth = static_cast<viz::VizEPenWidth>(Settings->value("Appearance/Indicators/LineWidth").toInt());
	PenStyle = static_cast<viz::VizEPenStyle>(Settings->value("Appearance/Indicators/LineStyle").toInt());
	IndicatorStyle = static_cast<viz::VizEIndicatorStyle>(Settings->value("Appearance/Indicators/Style").toInt());
	Color = QColor(Settings->value("Appearance/Indicators/Color").toString());

	gui::vizObjectGetAllPropertiesNamesAndValues(this, names, newValues);

}
//-----------------------------------------------------------------------------
QString VizIIndicator::toPlainText()
{
	QString text;
	QTextStream stream(&text);
	
	stream << tr("Data") << '\t' << vizObjectName(this) << '\n';

	foreach(core::VizCTradePeriod period, Periods)
	{
		QString date = QString("%1").arg(period.TimeStamp.date().toString(Qt::SystemLocaleShortDate));
		bool ok = period.isValid();
		
		QString value = ok ? QString("%1").arg(period.IndicatorValue, 0, 'f', 2) : "";

		
		stream << date << '\t' << value << '\n';
	}

	return text;
}
//-----------------------------------------------------------------------------
void VizIIndicator::ajustPeriods()
{
	Periods = HorizontalScale->ajustPeriods(Periods);
}
//-----------------------------------------------------------------------------
QColor VizIIndicator::color() const
{
	return Color;
}
//-----------------------------------------------------------------------------
void VizIIndicator::setColor(const QColor& color)
{
	if(Color != color)
	{
		QColor oldValue = Color;

		Color = color;

		emit createUndoCommandPropertyChanged(this, "color", oldValue, Color, false);
		emit itemChanged();
	}
}
//-----------------------------------------------------------------------------
VizEIndicatorStyle VizIIndicator::indicatorStyle() const
{
	return IndicatorStyle;
}
//-----------------------------------------------------------------------------
void VizIIndicator::setIndicatorStyle(VizEIndicatorStyle style)
{
	if(IndicatorStyle != style)
	{
		VizEIndicatorStyle oldValue = IndicatorStyle;
		
		IndicatorStyle = style;

		emit createUndoCommandPropertyChanged(this, "indicatorStyle", oldValue, IndicatorStyle, false);
		emit itemChanged(false);
	}
}
//-----------------------------------------------------------------------------
void VizIIndicator::nearestYValue(const QPointF& pos, f64& value, bool& isValid)
{
	const core::VizCTradePeriod& p = Periods[pos.x()];
	isValid = p.isValid();
	if(!isValid)
		return;
	value = p.IndicatorValue;
}
//-----------------------------------------------------------------------------
void VizIIndicator::yRange(bool& isEmpty, f64& minY, f64& maxY) const
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
		maxY = qMax<f64>(maxY, Period.IndicatorValue);
		minY = qMin<f64>(minY, Period.IndicatorValue);
		isEmpty = false;
	}
}
//-----------------------------------------------------------------------------
void VizIIndicator::lastTrade(f64& value, bool& isValid) const
{
	for(s32 i = Periods.size() - 1; i >= 0; i--)
	{
		if(Periods[i].isValid())
		{
			isValid = true;
			value = Periods[i].IndicatorValue;
			break;
		}
	}
}
//-----------------------------------------------------------------------------
void VizIIndicator::build()
{
	s32 rulesValuesIndexBegin;
	s32 rulesValuesIndexEnd;
	HorizontalScale->currentVisibleRulerValuesIndex(rulesValuesIndexBegin, rulesValuesIndexEnd);

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
}
//-----------------------------------------------------------------------------
void VizIIndicator::paint(QPainter* painter)
{
	switch(IndicatorStyle)
	{
		case VEIS_POINTS:
		{
			paintPoints(painter);
			break;
		}
		case VEIS_LINES:
		{
			paintLines(painter);
			break;
		}
		case VEIS_HISTOGRAM:
		{
			paintHistogram(painter);
			break;
		}
	};
}
//-----------------------------------------------------------------------------
void VizIIndicator::paintPoints(QPainter* painter)
{

	qreal pw = 0.0;
	if(PenWidth != viz::VEPW_1)
	{
		pw = static_cast<qreal>(PenWidth);
	}
	
	Color.setAlphaF(Visibility);
	QPen Pen(Color, pw, static_cast<Qt::PenStyle>(PenStyle), DefaultPenCapStyle, DefaultPenJoinStyle);
	Pen.setCosmetic(true);

	if(IsSelected)
	{
		QColor shadowColor = gui::computeShadowColor(painter->background().color());
		shadowColor.setAlphaF(Visibility);
		QPen shadowPen(shadowColor, pw + ShadowWidth, Qt::SolidLine, DefaultPenCapStyle, DefaultPenJoinStyle);
		shadowPen.setCosmetic(true);

		painter->setPen(shadowPen);
		painter->drawPoints(Points);
	}

	painter->setPen(Pen);
	painter->drawPoints(Points);

}
//-----------------------------------------------------------------------------
void VizIIndicator::paintHistogram(QPainter* painter)
{
	qreal pw = 0.0;

	Color.setAlphaF(Visibility);
	QPen Pen(Color, pw, Qt::SolidLine, DefaultPenCapStyle, DefaultPenJoinStyle);
	Pen.setCosmetic(true);

	QColor shadowColor = gui::computeShadowColor(painter->background().color());
	shadowColor.setAlphaF(Visibility);
	QPen shadowPen(shadowColor, pw + gui::ShadowWidth, Qt::SolidLine, DefaultPenCapStyle, DefaultPenJoinStyle);
	shadowPen.setCosmetic(true);
	
	
	//LOD - to reduce the Moiré pattern
	s32 pointsCount = Points.size();
	s32 w = painter->viewport().width();
	bool lod = (w < pointsCount * 1.5) ? true : false;

	if(lod)
	{
		//qDebug() << "VizIIndicator com LOD";
		if(IsSelected)
		{
			painter->setPen(shadowPen);
			painter->drawLines(Points);
		}

		painter->setPen(Pen);
		painter->drawLines(Points);

	}
	else
	{
		//qDebug() << "VizIIndicator sem LOD";
		if(IsSelected)
		{
			QColor shadowColor = gui::computeShadowColor(painter->background().color());
			shadowColor.setAlphaF(Visibility);
			QPen shadowPen(shadowColor, pw + gui::ShadowWidth, Qt::SolidLine, DefaultPenCapStyle, DefaultPenJoinStyle);
			shadowPen.setCosmetic(true);

			painter->setPen(shadowPen);
			f64 w = gui::CandleWidth;
			f64 halfw = (w * 0.5);
			for(s32 i = 0; i < Points.size(); i += 2)
			{
				f64 h = Points[i + 0].y() - Points[i + 1].y();
				f64 x = Points[i + 0].x() - halfw;
				f64 y = Points[i + 1].y();
				QRectF r(x, y, w, h);
				painter->drawRect(r);
			}

		}
		f64 w = gui::CandleWidth;
		f64 halfw = (w * 0.5);
		for(s32 i = 0; i < Points.size(); i += 2)
		{
			f64 h = Points[i + 0].y() - Points[i + 1].y();
			f64 x = Points[i + 0].x() - halfw;
			f64 y = Points[i + 1].y();
			QRectF r(x, y, w, h);
			gui::fillRect(painter, r, Color, UseGradientToFillShapes);
		}
	}

}
//-----------------------------------------------------------------------------
void VizIIndicator::paintLines(QPainter* painter)
{
	qreal pw = 0.0;
	if(PenWidth != viz::VEPW_1)
	{
		pw = static_cast<qreal>(PenWidth);
	}

	Color.setAlphaF(Visibility);

	QPen Pen(Color, pw, static_cast<Qt::PenStyle>(PenStyle), DefaultPenCapStyle, DefaultPenJoinStyle);
	Pen.setCosmetic(true);

	if(IsSelected)
	{
		QColor shadowColor = gui::computeShadowColor(painter->background().color());
		shadowColor.setAlphaF(Visibility);
		QPen shadowPen(shadowColor, pw + gui::ShadowWidth, Qt::SolidLine, DefaultPenCapStyle, DefaultPenJoinStyle);
		shadowPen.setCosmetic(true);

		painter->setPen(shadowPen);
		painter->drawLines(Points);
	}

	painter->setPen(Pen);
	painter->drawLines(Points);
}
//-----------------------------------------------------------------------------
void VizIIndicator::buildPoints(s32 rulesValuesIndexBegin, s32 rulesValuesIndexEnd)
{
	bool isSemiLogarithmicScale = VerticalScale->isSemiLogarithmicScale();

	Points.clear();

	for(s32 i = rulesValuesIndexBegin; i <= rulesValuesIndexEnd; i++)
	{
		core::VizCTradePeriod Period = Periods[i];
		if(isSemiLogarithmicScale)
		{
			Period = Period.toLogarithmic();
		}
		if(Period.isValid())
		{
			f64 price = Period.IndicatorValue;
			Points.append(QPointF(i, price));
		}
	}

}
//-----------------------------------------------------------------------------
void VizIIndicator::buildHistogram(s32 rulesValuesIndexBegin, s32 rulesValuesIndexEnd)
{
	bool isSemiLogarithmicScale = VerticalScale->isSemiLogarithmicScale();

	f64 minY = 0.0;
	Points.clear();

	for(s32 i = rulesValuesIndexBegin; i <= rulesValuesIndexEnd; i++)
	{
		core::VizCTradePeriod Period = Periods[i];
		if(isSemiLogarithmicScale)
		{
			Period = Period.toLogarithmic();
		}
		if(Period.isValid())
		{
			f64 price = Period.IndicatorValue;
			Points.append(QPointF(i, minY));
			Points.append(QPointF(i, price));
		}
	}

}
//-----------------------------------------------------------------------------
void VizIIndicator::buildLines(s32 rulesValuesIndexBegin, s32 rulesValuesIndexEnd)
{
	bool isSemiLogarithmicScale = VerticalScale->isSemiLogarithmicScale();

	Points.clear();

	//desenhar a borda left
	if(rulesValuesIndexBegin > 0)
	{
		core::VizCTradePeriod Period1 = Periods[rulesValuesIndexBegin - 1];
		core::VizCTradePeriod Period2 = Periods[rulesValuesIndexBegin + 0];
		if(isSemiLogarithmicScale)
		{
			Period1 = Period1.toLogarithmic();
			Period2 = Period2.toLogarithmic();
		}
		if(Period1.isValid() && Period2.isValid())
		{
			f64 price1 = Period1.IndicatorValue;
			f64 price2 = Period2.IndicatorValue;
			Points.append(QPointF(rulesValuesIndexBegin - 1, price1));
			Points.append(QPointF(rulesValuesIndexBegin - 0, price2));
		}	
	}
	//desenhar a borda right
	if(rulesValuesIndexEnd < Periods.size() - 1)
	{
		s32 size = rulesValuesIndexEnd - rulesValuesIndexBegin + 1;
		core::VizCTradePeriod Period1 = Periods[rulesValuesIndexEnd + 0];
		core::VizCTradePeriod Period2 = Periods[rulesValuesIndexEnd + 1];
		if(isSemiLogarithmicScale)
		{
			Period1 = Period1.toLogarithmic();
			Period2 = Period2.toLogarithmic();
		}
		if(Period1.isValid() && Period2.isValid())
		{
			f64 price1 = Period1.IndicatorValue;
			f64 price2 = Period2.IndicatorValue;
			Points.append(QPointF(rulesValuesIndexEnd + 0, price1));
			Points.append(QPointF(rulesValuesIndexEnd + 1, price2));
		}	
	}

	for(s32 i = rulesValuesIndexBegin; i <= rulesValuesIndexEnd - 1; i++)
	{
		core::VizCTradePeriod Period1 = Periods[i + 0];
		core::VizCTradePeriod Period2 = Periods[i + 1];
		if(isSemiLogarithmicScale)
		{
			Period1 = Period1.toLogarithmic();
			Period2 = Period2.toLogarithmic();
		}
		if(Period1.isValid() && Period2.isValid())
		{
			f64 price1 = Period1.IndicatorValue;
			f64 price2 = Period2.IndicatorValue;
			Points.append(QPointF(i + 0, price1));
			Points.append(QPointF(i + 1, price2));
		}
	}
}
//-----------------------------------------------------------------------------
void VizIIndicator::ajustToOverlayWithoutScale(f64 itemMinY, f64 itemMaxY)
{
	ajustLinesToOverlayWithoutScale(itemMinY, itemMaxY, Points);
}
//-----------------------------------------------------------------------------
bool VizIIndicator::intersects(const QRectF& rect) const
{
	bool b = false;

	if(IndicatorStyle == VEIS_HISTOGRAM)
	{
		f64 w = 0.75;
		f64 halfw = (w * 0.5);
		for(s32 i = 0; i < Points.size(); i += 2)
		{
			f64 h = Points[i + 1].y() - Points[i + 0].y();
			f64 x = Points[i + 0].x() - halfw;
			f64 y = Points[i + 0].y();
			QRectF r(x, y, w, h);
			b = core::intersectsRectWithValidRect(r, rect);
			if(b) return true;
		}

	}
	else
	{
		for(s32 i = 0; i < Points.size() - 1; i +=2)
		{
			const QPointF& p1 = Points[i + 0];
			const QPointF& p2 = Points[i + 1];
			b = core::intersectsLineWithRect(QLineF(p1, p2), rect);
			if(b) return true;
		}
	}

	return b;
}
//-----------------------------------------------------------------------------
viz::VizEPenStyle VizIIndicator::penStyle() const
{
	return PenStyle;
}
//-----------------------------------------------------------------------------
void VizIIndicator::setPenStyle(VizEPenStyle penStyle)
{
	if(PenStyle != penStyle)
	{
		VizEPenStyle oldValue = PenStyle;

		PenStyle = penStyle;

		emit createUndoCommandPropertyChanged(this, "penStyle", oldValue, PenStyle, false);
		emit itemChanged(false);
	}
}
//-----------------------------------------------------------------------------
QString VizIIndicator::nameToTitleBar(bool showLastTradeValues, bool useColorOfPlot) const
{
	QString out = vizObjectName(this);

	if(showLastTradeValues)
	{
		if(useColorOfPlot)
		{
			out += QString("<font color=\"%1\">").arg(Color.name());
		}
		for(s32 i = Periods.size() - 1; i >= 0; i--)
		{
			if(Periods[i].isValid())
			{
				out += " (";
				//.arg(period.IndicatorValue, 0, 'f', 2)
				out += QString("%L1").arg(Periods[i].IndicatorValue, 0, 'f', 2);
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
void VizIIndicator::shiftRulesValuesIndex(s32 HorizontalShift, s32& rulesValuesIndexBegin, s32& rulesValuesIndexEnd) const
{
	s32 rulesValuesIndexBeginWithHorizontalShift = rulesValuesIndexBegin;
	s32 rulesValuesIndexEndWithHorizontalShift = rulesValuesIndexEnd;
	s32 firstIndex = 0;
	s32 lastIndex = HorizontalScale->rulerValues().size() - 1;

	if(HorizontalShift < 0)
	{
		rulesValuesIndexEndWithHorizontalShift = rulesValuesIndexEnd + abs(HorizontalShift);
	}
	else if(HorizontalShift > 0)
	{
		rulesValuesIndexBeginWithHorizontalShift = rulesValuesIndexBegin - abs(HorizontalShift);
	}

	if(rulesValuesIndexEndWithHorizontalShift > lastIndex)
	{
		rulesValuesIndexEndWithHorizontalShift = lastIndex;
	}
	if(rulesValuesIndexEndWithHorizontalShift < 0)
	{
		rulesValuesIndexEndWithHorizontalShift = 0;
	}

	if(rulesValuesIndexBeginWithHorizontalShift < 0)
	{
		rulesValuesIndexBeginWithHorizontalShift = 0;
	}
	if(rulesValuesIndexBeginWithHorizontalShift > lastIndex)
	{
		rulesValuesIndexBeginWithHorizontalShift = lastIndex;
	}


	rulesValuesIndexBegin = rulesValuesIndexBeginWithHorizontalShift;
	rulesValuesIndexEnd = rulesValuesIndexEndWithHorizontalShift;
}
//-----------------------------------------------------------------------------
void VizIIndicator::addRelativeItems(VizCGraphicsPanel* graphicsPanel)
{
}
//-----------------------------------------------------------------------------
int VizIIndicator::propertyGetindicatorStyle() const
{
	return static_cast<int>(indicatorStyle());
}
//-----------------------------------------------------------------------------
void VizIIndicator::propertySetIndicatorStyle(int value)
{
	setIndicatorStyle(static_cast<VizEIndicatorStyle>(value));
}
//-----------------------------------------------------------------------------
int VizIIndicator::propertyGetPenStyle() const
{
	return static_cast<viz::VizEPenStyle>(penStyle());
}
//-----------------------------------------------------------------------------
void VizIIndicator::propertySetPenStyle(int value)
{
	setPenStyle(static_cast<viz::VizEPenStyle>(value));
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
