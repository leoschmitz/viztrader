//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCSemiLogTrendLine.h"
#include "VizIGraphicsItemBasedOnSecurity.h"
#include "VizMath.h"
#include "VizGUIUtils.h"
#include "VizCWorkArea.h"
#include <QDebug>
#include <QDateTime>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
const s32 MinSamplesNumber = 10;
const s32 MaxSamplesNumber = 100;
//-----------------------------------------------------------------------------
VizCSemiLogTrendLine::VizCSemiLogTrendLine() : VizIGraphicsItemBasedOnShape(), StartValue(0.0), EndValue(0.0), LeftExtention(false), RightExtention(false), FindDateTimeInHorizontalScale(false)
{
	blockSignals(true);
	resetToDefaults();
	blockSignals(false);

	setProperty("name", tr("Semi-log Trend Line"));

}
//-----------------------------------------------------------------------------
VizCSemiLogTrendLine::~VizCSemiLogTrendLine()
{
}
//-----------------------------------------------------------------------------
void VizCSemiLogTrendLine::resetToDefaults()
{
	QStringList names;
	QVariantList oldValues;
	QVariantList newValues;

	gui::vizObjectGetAllPropertiesNamesAndValues(this, names, oldValues);

	VizIGraphicsItemBasedOnShape::resetToDefaults();
	Color = QColor(Settings->value("Appearance/Items/Color").toString());

	gui::vizObjectGetAllPropertiesNamesAndValues(this, names, newValues);

	emit createUndoCommandPropertiesChanged(this, names, oldValues, newValues, true, tr("Valores Padrão"));
	emit itemChanged(true);
}
//-----------------------------------------------------------------------------
void VizCSemiLogTrendLine::build()
{
	SemiLogLine.clear();

	bool FindDateTimeInHorizontalScale;
	FindDateTimeInHorizontalScale = HorizontalScale->findRuleValueIndex(StartDateIndex, StartDate, true);
	FindDateTimeInHorizontalScale &= HorizontalScale->findRuleValueIndex(EndDateIndex, EndDate, true);

	f64 startIndexF = static_cast<f64>(StartDateIndex);
	f64 endIndexF = static_cast<f64>(EndDateIndex);

	bool isSemiLogarithmicScale = VerticalScale->isSemiLogarithmicScale();

	f64 startValue = StartValue;
	f64 endValue = EndValue;

	if(isSemiLogarithmicScale)
	{
		startValue = core::clampLog10f(startValue);
		endValue = core::clampLog10f(endValue);
	}

	P1 = QPointF(startIndexF, startValue);
	P2 = QPointF(endIndexF, endValue);

	/*if(SnapToPrice)
	{
		VizIGraphicsItemBasedOnSecurity* parent = qobject_cast<VizIGraphicsItemBasedOnSecurity*>(parentItem());
		if(parent)
		{
			bool isValid;
			f64 y;
			parent->nearestYValue(P1, y, isValid);
			if(isValid)
				P1.setY(y);
			parent->nearestYValue(P2, y, isValid);
			if(isValid)
				P2.setY(y);

		}
	}*/

	buildSemiLogLine(P1, P2, SemiLogLine, isSemiLogarithmicScale);


	/*if(RightExtention || LeftExtention)
	{
		computeExtentionsPoints(P1, P2, ExtentionPoints, LeftExtention, RightExtention);
		buildSemiLogLine(ExtentionPoints[0], ExtentionPoints[1], SemiLogLineExtentions[0], isSemiLogarithmicScale);
		buildSemiLogLine(ExtentionPoints[2], ExtentionPoints[3], SemiLogLineExtentions[1], isSemiLogarithmicScale);
	}*/


}
//-----------------------------------------------------------------------------
void VizCSemiLogTrendLine::buildSemiLogLine(const QPointF& p1, const QPointF& p2, QPolygonF& poly, bool isSemiLogarithmicScale)
{

	f64 SamplesNumber = computeSamplesNumber(p1, p2);

	f64 x_inc = (p2.x() - p1.x()) / SamplesNumber;
	f64 delta_y_linear = p2.y() - p1.y();
	f64 delta_y_log = core::clampLog10f(p2.y()) - core::clampLog10f(p1.y());
	f64 y_inc_log = (delta_y_log) / SamplesNumber;
	f64 y_inc = (delta_y_linear) / SamplesNumber;

	s32 LeftSampleNumber = 0;
	s32 RightSampleNumber = 0;


	for(s32 i = -LeftSampleNumber; i <= static_cast<s32>(SamplesNumber) + RightSampleNumber; i++)
	{
		QPointF p;
		p.setX(p1.x() + x_inc * i);

		if(isSemiLogarithmicScale)
		{
			f64 y = core::pow10f(p1.y());
			y = core::clampLog10f(y) + y_inc * i;
			p.setY(y);
		}
		else
		{
			f64 y = core::clampLog10f(p1.y()) + y_inc_log * i;
			y = core::pow10f(y);
			p.setY(y);
		}

		poly.append(p);
	}
}
//-----------------------------------------------------------------------------
void VizCSemiLogTrendLine::computeExtentionsPoints(const QPointF& p1, const QPointF& p2, QPointF* extentions)
{
	if(p1 == p2)
	{
		extentions[0] = p1;
		extentions[1] = p1;
		extentions[2] = p1;
		extentions[3] = p1;
		return;
	}

	QRectF viewport = graphicsPanelViewPort();

	QLineF line(p1, p2);
	QLineF top_line = QLineF(viewport.topLeft(), viewport.topRight());
	QLineF bottom_line = QLineF(viewport.bottomLeft(), viewport.bottomRight());
	QLineF left_line = QLineF(viewport.topLeft(), viewport.bottomLeft());
	QLineF right_line = QLineF(viewport.topRight(), viewport.bottomRight());

	//e vertical
	if(core::equals(line.p1().x(), line.p2().x()))
	{
		QPointF topPoint = QPointF(p1.x(), top_line.p1().y());
		QPointF bottonPoint = QPointF(p1.x(), bottom_line.p1().y());

		if(p1.y() < p2.y())
		{
			extentions[0] = bottonPoint;
			extentions[1] = p1;
			extentions[2] = p2;
			extentions[3] = topPoint;
		}
		else
		{
			extentions[0] = topPoint;
			extentions[1] = p1;
			extentions[2] = p2;
			extentions[3] = bottonPoint;
		}
		return;
	}


	QList<QPointF> UnboundedIntersectionPoints;

	QPointF tmp;
	if(core::intersectsRayWithLineSegment(line, top_line, tmp))
	{
		UnboundedIntersectionPoints << tmp;
	}
	if(core::intersectsRayWithLineSegment(line, bottom_line, tmp))
	{
		UnboundedIntersectionPoints << tmp;
	}
	if(core::intersectsRayWithLineSegment(line, left_line, tmp))
	{
		UnboundedIntersectionPoints << tmp;
	}
	if(core::intersectsRayWithLineSegment(line, right_line, tmp))
	{
		UnboundedIntersectionPoints << tmp;
	}

	QPointF leftPoint = p1;
	QPointF rightPoint = p1;
	foreach(QPointF p, UnboundedIntersectionPoints)
	{
		if(p.x() < leftPoint.x())
			leftPoint = p;
		if(p.x() > rightPoint.x())
			rightPoint = p;
	}

	extentions[0] = leftPoint;
	extentions[1] = p1.x() < p2.x() ? p1 : p2;
	extentions[2] = p1.x() > p2.x() ? p1 : p2;
	extentions[3] = rightPoint;
}
//-----------------------------------------------------------------------------
void VizCSemiLogTrendLine::paint(QPainter* painter)
{

	PainterDeviceSize.setWidth(painter->device()->width());
	PainterDeviceSize.setHeight(painter->device()->height());


	qreal pw = 0.0;
	if(PenWidth != viz::VEPW_1)
	{
		pw = static_cast<qreal>(PenWidth);
	}

	Color.setAlphaF(Visibility);
	QPen pen(Color, pw, static_cast<Qt::PenStyle>(PenStyle), DefaultPenCapStyle, DefaultPenJoinStyle);
	pen.setCosmetic(true);
	
	painter->setPen(pen);
	painter->drawPolyline(SemiLogLine);
	
	VizIGraphicsItemBasedOnShape::paint(painter);

}
//-----------------------------------------------------------------------------
void VizCSemiLogTrendLine::yRange(bool& isEmpty, f64& minY, f64& maxY) const
{
	maxY = qMax<f64>(maxY, StartValue);
	minY = qMin<f64>(minY, StartValue);
	maxY = qMax<f64>(maxY, EndValue);
	minY = qMin<f64>(minY, EndValue);
	isEmpty = false;
}
//-----------------------------------------------------------------------------
bool VizCSemiLogTrendLine::intersects(const QRectF& rect) const
{

	for(s32 i = 0; i < SemiLogLine.size() - 1; i++)
	{
		const QPointF& p1 = SemiLogLine[i + 0];
		const QPointF& p2 = SemiLogLine[i + 1];
		if(core::intersectsLineWithRect(QLineF(p1, p2), rect)) return true;
	}

	return false;

}
//-----------------------------------------------------------------------------
void VizCSemiLogTrendLine::ajustToOverlayWithoutScale(f64 itemMinY, f64 itemMaxY)
{
	ajustPointToOverlayWithoutScale(itemMinY, itemMaxY, P1);
	ajustPointToOverlayWithoutScale(itemMinY, itemMaxY, P2);
	ajustLinesToOverlayWithoutScale(itemMinY, itemMaxY, SemiLogLine);
}
//-----------------------------------------------------------------------------
QString VizCSemiLogTrendLine::toolTip(const QPointF& p) const
{
	const QVector<QDateTime>& rv = HorizontalScale->rulerValues();
	const QDateTime& dateTime = rv[p.x()];
	bool isSemiLogarithmicScale = VerticalScale->isSemiLogarithmicScale();

	f64 deltax = (P2.x() - P1.x());
	QString strSlope;
	if(deltax)
	{
		f64 slope;
		if(isSemiLogarithmicScale)
		{
			slope = (core::pow10f(P2.y()) - core::pow10f(P1.y())) / deltax;
		}
		else
		{
			slope = (P2.y() - P1.y()) / deltax;
		}
		strSlope = QString("%1").arg(slope, 0, 'f', 2);
	}
	else
	{
		strSlope = tr("Indefinida");
	}

	f64 y = isSemiLogarithmicScale ? core::pow10f(p.y()) : p.y();
	
	QString s = QString(tr("%1\nInício: (%2, %L3)\nFim: (%4, %L5) \nData: %6\nValor: %L7\nInclinação: %8"))
		.arg(tr("Semi-log Trend Line"))
		.arg(StartDate.date().toString(Qt::SystemLocaleShortDate))
		.arg(StartValue, 0, 'f', 2)
		.arg(EndDate.date().toString(Qt::SystemLocaleShortDate))
		.arg(EndValue, 0, 'f', 2)
		.arg(dateTime.date().toString(Qt::SystemLocaleShortDate))
		.arg(y, 0, 'f', 2)
		.arg(strSlope)
		;
	return s;
}
//-----------------------------------------------------------------------------
f64 VizCSemiLogTrendLine::startValue() const
{
	return StartValue;
}
//-----------------------------------------------------------------------------
void VizCSemiLogTrendLine::setStartValue(f64 value)
{
	if(StartValue != value)
	{
		f64 oldValue = StartValue;

		StartValue = value;

		emit createUndoCommandPropertyChanged(this, "startValue", oldValue, StartValue, true);
		emit itemChanged(true);
	}
}
//-----------------------------------------------------------------------------
f64 VizCSemiLogTrendLine::endValue() const
{
	return EndValue;
}
//-----------------------------------------------------------------------------
void VizCSemiLogTrendLine::setEndValue(f64 value)
{
	if(EndValue != value)
	{
		f64 oldValue = EndValue;

		EndValue = value;

		emit createUndoCommandPropertyChanged(this, "endValue", oldValue, EndValue, true);
		emit itemChanged(true);
	}
}
//-----------------------------------------------------------------------------
QDateTime VizCSemiLogTrendLine::startDate() const
{
	return StartDate;
}
//-----------------------------------------------------------------------------
void VizCSemiLogTrendLine::setStartDate(const QDateTime& value)
{
	if(StartDate != value)
	{
		QDateTime oldValue = StartDate;

		StartDate = value;

		emit createUndoCommandPropertyChanged(this, "startDate", oldValue, StartDate, true);
		emit itemChanged(true);
	}
}
//-----------------------------------------------------------------------------
QDateTime VizCSemiLogTrendLine::endDate() const
{
	return EndDate;
}
//-----------------------------------------------------------------------------
void VizCSemiLogTrendLine::setEndDate(const QDateTime& value)
{
	if(EndDate != value)
	{
		QDateTime oldValue = StartDate;

		EndDate = value;

		emit createUndoCommandPropertyChanged(this, "endDate", oldValue, EndDate, true);
		emit itemChanged(true);
	}
}
//-----------------------------------------------------------------------------
bool VizCSemiLogTrendLine::leftExtention() const
{
	return LeftExtention;
}
//-----------------------------------------------------------------------------
void VizCSemiLogTrendLine::setLeftExtention(bool value)
{
	if(LeftExtention != value)
	{
		bool oldValue = LeftExtention;

		LeftExtention = value;

		emit createUndoCommandPropertyChanged(this, "leftExtention", oldValue, LeftExtention, true);
		emit itemChanged(true);
	}
}
//-----------------------------------------------------------------------------
bool VizCSemiLogTrendLine::rightExtention() const
{
	return RightExtention;
}
//-----------------------------------------------------------------------------
void VizCSemiLogTrendLine::setRightExtention(bool value)
{
	if(RightExtention != value)
	{
		bool oldValue = RightExtention;

		RightExtention = value;

		emit createUndoCommandPropertyChanged(this, "rightExtention", oldValue, RightExtention, true);
		emit itemChanged(true);
	}
}
//-----------------------------------------------------------------------------
void VizCSemiLogTrendLine::endEdit(const VizSShapeTransform& transform)
{
	VizIGraphicsItemBasedOnShape::endEdit(transform);

	bool isSemiLogarithmicScale = VerticalScale->isSemiLogarithmicScale();

	switch(CurrentTransform.Type)
	{
		case VESTT_ADDING_NEW_ITEM:
		{
			const QDateTime& date1 = HorizontalScale->rulerValues()[static_cast<s32>(CurrentTransform.StartPostion.x())];
			const QDateTime& date2 = HorizontalScale->rulerValues()[static_cast<s32>(CurrentTransform.CurrentPostion.x())];

			StartValue = CurrentTransform.StartPostion.y();
			EndValue = CurrentTransform.CurrentPostion.y();;
			if(isSemiLogarithmicScale)
			{
				StartValue = core::pow10f(StartValue);
				EndValue = core::pow10f(EndValue);
			}
			StartDate = date1;
			EndDate = date2;

			WorkArea->updateProperty(this, "startValue");
			WorkArea->updateProperty(this, "endValue");
			WorkArea->updateProperty(this, "startDate");
			WorkArea->updateProperty(this, "endDate");

			emit itemChanged(true);

			break;
		}
		case VESTT_MOVE:
		{
			s32 rulesValuesIndexBegin;
			s32 rulesValuesIndexEnd;
			HorizontalScale->currentVisibleRulerValuesIndex(rulesValuesIndexBegin, rulesValuesIndexEnd);

			QPointF delta = CurrentTransform.CurrentPostion - CurrentTransform.StartPostion;

			float dy1New;
			float dy2New;
			if(isSemiLogarithmicScale)
			{
				float dy1 = (CurrentTransform.StartPostion.y()) - core::clampLog10f(StartValue);
				float dy2 = (CurrentTransform.StartPostion.y()) - core::clampLog10f(EndValue);

				float curMouseY = (CurrentTransform.CurrentPostion.y());

				dy1New = core::pow10f((curMouseY) - (dy1));
				dy2New = core::pow10f((curMouseY) - (dy2));
			}
			else
			{
				dy1New = StartValue + delta.y();
				dy2New  = EndValue + delta.y();
			}

			//delta.setX(delta.x() + rulesValuesIndexBegin);
			//clamp no range X
			s32 rulervaluewsize = HorizontalScale->rulerValues().size() - 1;
			if(!core::isInRange(P1.x() + delta.x(), 0, rulervaluewsize))
			{
				f64 deltaExtra = (P1.x() + delta.x()) - core::clampInRange(P1.x() + delta.x(), 0, rulervaluewsize);
				delta.setX(delta.x() - deltaExtra);
			}
			if(!core::isInRange(P2.x() + delta.x(), 0, rulervaluewsize))
			{
				f64 deltaExtra = (P2.x() + delta.x()) - core::clampInRange(P2.x() + delta.x(), 0, rulervaluewsize);
				delta.setX(delta.x() - deltaExtra);
			}
			s32 index1 = static_cast<s32>(P1.x() + delta.x());
			s32 index2 = static_cast<s32>(P2.x() + delta.x());
			const QDateTime& date1 = HorizontalScale->rulerValues()[index1];
			const QDateTime& date2 = HorizontalScale->rulerValues()[index2];

			bool itemChangedEvent;
			itemChangedEvent = StartValue != StartValue + delta.y();
			itemChangedEvent |= EndValue != EndValue + delta.y();
			itemChangedEvent |= StartDate != date1;
			itemChangedEvent |= EndDate != date2;
			if(!itemChangedEvent)
			{
				break;
			}

			QVariantList oldValues;
			oldValues << StartValue << EndValue << StartDate << EndDate;

			StartValue = dy1New > 0.0 ? dy1New : 0.0;
			EndValue = dy2New > 0.0 ? dy2New : 0.0;
			StartDate = date1;
			EndDate = date2;

			QVariantList newValues;
			newValues << StartValue << EndValue << StartDate << EndDate;

			QStringList propertiesNames;
			propertiesNames << "startValue" << "endValue" << "startDate" << "endDate";
			WorkArea->updateProperties(this, propertiesNames);

			emit createUndoCommandPropertiesChanged(this, propertiesNames, oldValues, newValues, true, tr("Mover"));
			emit itemChanged(true);

			break;
		}
		case VESTT_CONTROL_POINTS:
		{

			if(CurrentTransform.ActiveControlPointIndex == 0)
			{
				const QDateTime& date = HorizontalScale->rulerValues()[static_cast<s32>(CurrentTransform.CurrentPostion.x())];

				QVariantList oldValues;
				oldValues << StartValue << StartDate;

				StartValue = CurrentTransform.CurrentPostion.y();
				if(isSemiLogarithmicScale)
				{
					StartValue = core::pow10f(StartValue);
				}
				StartDate = date;

				QVariantList newValues;
				newValues << StartValue << StartDate;

				bool itemChangedEvent = oldValues[0] != newValues[0];
				itemChangedEvent |= oldValues[1] != newValues[1];
				if(!itemChangedEvent)
				{
					break;
				}

				QStringList propertiesNames;
				propertiesNames << "startValue" << "startDate";

				WorkArea->updateProperties(this, propertiesNames);

				emit createUndoCommandPropertiesChanged(this, propertiesNames, oldValues, newValues, true, tr("Mover"));
			}
			else
			if(CurrentTransform.ActiveControlPointIndex == 1)
			{
				const QDateTime& date = HorizontalScale->rulerValues()[static_cast<s32>(CurrentTransform.CurrentPostion.x())];

				QVariantList oldValues;
				oldValues << EndValue << EndDate;

				EndValue = CurrentTransform.CurrentPostion.y();
				if(isSemiLogarithmicScale)
				{
					EndValue = core::pow10f(EndValue);
				}
				EndDate = date;

				QVariantList newValues;
				newValues << StartValue << StartDate;

				bool itemChangedEvent = oldValues[0] != newValues[0];
				itemChangedEvent |= oldValues[1] != newValues[1];
				if(!itemChangedEvent)
				{
					break;
				}

				QStringList propertiesNames;
				propertiesNames << "startValue" << "startDate";

				WorkArea->updateProperties(this, propertiesNames);

				emit createUndoCommandPropertiesChanged(this, propertiesNames, oldValues, newValues, true, tr("Mover"));
			}

			emit itemChanged(true);

			break;
		}
	}
}
//-----------------------------------------------------------------------------
QList<QPointF> VizCSemiLogTrendLine::controlPoints() const
{
	QList<QPointF> points;
	points << P1 << P2;
	return points;
}
//-----------------------------------------------------------------------------
void VizCSemiLogTrendLine::edit(const VizSShapeTransform& transform)
{
	VizSShapeTransform transformOther;
	transformOther = transform;

	if(CurrentTransform.Type == VESTT_MOVE)
	{
		s32 rulesValuesIndexBegin;
		s32 rulesValuesIndexEnd;
		HorizontalScale->currentVisibleRulerValuesIndex(rulesValuesIndexBegin, rulesValuesIndexEnd);

		f64 wiewport_width = HorizontalScale->rulerValues().size();
		f64 delta_x = transformOther.CurrentPostion.x() - transformOther.StartPostion.x();
		f64 x1 = P1.x() + delta_x;
		f64 x2 = P2.x() + delta_x;

		f64 CurrentPostionX = transformOther.CurrentPostion.x();
		if(x1 < 0)
		{
			CurrentPostionX = transformOther.CurrentPostion.x() - x1;
		}
		else
		if(x2 < 0)
		{
			CurrentPostionX = transformOther.CurrentPostion.x() - x2;
		}
		else
		if(x1 >= wiewport_width)
		{
			CurrentPostionX = transformOther.CurrentPostion.x() - (x1 - wiewport_width);
		}
		else
		if(x2 >= wiewport_width)
		{
			CurrentPostionX = transformOther.CurrentPostion.x() - (x2 - wiewport_width);
		}

		transformOther.CurrentPostion.setX(CurrentPostionX);
	}
	VizIGraphicsItemBasedOnShape::edit(transformOther);
}
//-----------------------------------------------------------------------------
void VizCSemiLogTrendLine::paintRubberRands(QPainter* painter)
{
	s32 rulesValuesIndexBegin;
	s32 rulesValuesIndexEnd;
	HorizontalScale->currentVisibleRulerValuesIndex(rulesValuesIndexBegin, rulesValuesIndexEnd);
	bool isSemiLogarithmicScale = VerticalScale->isSemiLogarithmicScale();

	QPointF p1, p2;
	switch(CurrentTransform.Type)
	{
		case VESTT_ADDING_NEW_ITEM:
		{
			p1 = CurrentTransform.StartPostion;
			p2 = CurrentTransform.CurrentPostion;
			//p1.setX(p1.x() - rulesValuesIndexBegin + 0.5);
			//p2.setX(p2.x() - rulesValuesIndexBegin + 0.5);
			break;
		}
		case VESTT_MOVE:
		{
			QPointF delta = CurrentTransform.CurrentPostion - CurrentTransform.StartPostion;

			p1 = P1 + delta;
			p2 = P2 + delta;

			break;
		}
		case VESTT_CONTROL_POINTS:
		{
			if(CurrentTransform.ActiveControlPointIndex == 0)
			{
				p1 = CurrentTransform.CurrentPostion;
				p2 = P2;
				//p1.setX(p1.x() - rulesValuesIndexBegin + 0.5);
			}
			else
			if(CurrentTransform.ActiveControlPointIndex == 1)
			{
				p1 = P1;
				p2 = CurrentTransform.CurrentPostion;
				//p2.setX(p2.x() - rulesValuesIndexBegin + 0.5);
			}
			break;
		}
	}

	if(SnapToPrice)
	{
		VizIGraphicsItemBasedOnSecurity* parent = qobject_cast<VizIGraphicsItemBasedOnSecurity*>(parentItem());
		if(parent)
		{
			bool isValid;
			f64 y;
			parent->nearestYValue(p1, y, isValid);
			if(isValid)
				p1.setY(y);
			parent->nearestYValue(p2, y, isValid);
			if(isValid)
				p2.setY(y);

		}
	}

	QPolygonF semiLogLine;
	buildSemiLogLine(p1, p2, semiLogLine, isSemiLogarithmicScale);
	

	QColor shadowColor = computeShadowColor(painter->background().color());
	QPen shadowPen(shadowColor, 1.0, Qt::DashLine);
	shadowPen.setCosmetic(true);
	painter->setPen(shadowPen);
	painter->drawPolyline(semiLogLine);

}
//-----------------------------------------------------------------------------
s32 VizCSemiLogTrendLine::computeSamplesNumber(const QPointF& p1, const QPointF& p2)
{
	if(VerticalScale->isSemiLogarithmicScale())
	{
		return 1;
	}
	f64 delta = VerticalScale->maximum() - VerticalScale->minimum();
	f64 delta_y = core::fabs(p2.y() - p1.y());
	s32 N = (delta_y * PainterDeviceSize.height()) / delta;
	
	N = core::max(N, MinSamplesNumber);
	N = core::min(N, MaxSamplesNumber);

	return N;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
