//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCTrendLine.h"
#include "VizIGraphicsItemBasedOnSecurity.h"
#include "VizMath.h"
#include "VizGUIUtils.h"
#include "VizCWorkArea.h"
#include "VizMath.h"
#include <QDebug>
#include <QDateTime>
#include <limits>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
VizCTrendLine::VizCTrendLine() : VizIGraphicsItemBasedOnShape(), StartValue(0.0), EndValue(0.0), LeftExtention(false), RightExtention(false)
{
	blockSignals(true);
	resetToDefaults();
	blockSignals(false);
	
	setProperty("name", tr("Trend Line"));
}
//-----------------------------------------------------------------------------
VizCTrendLine::~VizCTrendLine()
{
}
//-----------------------------------------------------------------------------
void VizCTrendLine::resetToDefaults()
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
void VizCTrendLine::build()
{
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
	}
	*/
	

	if(RightExtention || LeftExtention)
	{
		computeExtentionsPoints(P1, P2, ExtentionPoints);
	}

}
//-----------------------------------------------------------------------------
void VizCTrendLine::computeExtentionsPoints(const QPointF& p1, const QPointF& p2, QPointF* extentions)
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
void VizCTrendLine::paint(QPainter* painter)
{

	qreal pw = 0.0;
	if(PenWidth != viz::VEPW_1)
	{
		pw = static_cast<qreal>(PenWidth);
	}

	Color.setAlphaF(Visibility);
	QPen pen(Color, pw, static_cast<Qt::PenStyle>(PenStyle), DefaultPenCapStyle, DefaultPenJoinStyle);
	pen.setCosmetic(true);
	painter->setPen(pen);
	painter->drawLine(P1, P2);

	if(LeftExtention)
	{
		painter->drawLine(ExtentionPoints[0], ExtentionPoints[1]);
	}
	if(RightExtention)
	{
		painter->drawLine(ExtentionPoints[2], ExtentionPoints[3]);
	}

	VizIGraphicsItemBasedOnShape::paint(painter);
}
//-----------------------------------------------------------------------------
void VizCTrendLine::yRange(bool& isEmpty, f64& minY, f64& maxY) const
{
	maxY = qMax<f64>(maxY, StartValue);
	minY = qMin<f64>(minY, StartValue);
	maxY = qMax<f64>(maxY, EndValue);
	minY = qMin<f64>(minY, EndValue);
	isEmpty = false;
}
//-----------------------------------------------------------------------------
bool VizCTrendLine::intersects(const QRectF& rect) const
{
	if(core::intersectsLineWithRect(QLineF(P1, P2), rect))
	{
		return true;
	}
	
	if(LeftExtention && core::intersectsLineWithRect(QLineF(ExtentionPoints[0], ExtentionPoints[1]), rect))
	{
		return true;
	}

	if(RightExtention && core::intersectsLineWithRect(QLineF(ExtentionPoints[2], ExtentionPoints[3]), rect))
	{
		return true;
	}

	return false;
}
//-----------------------------------------------------------------------------
void VizCTrendLine::ajustToOverlayWithoutScale(f64 itemMinY, f64 itemMaxY)
{
	ajustPointToOverlayWithoutScale(itemMinY, itemMaxY, P1);
	ajustPointToOverlayWithoutScale(itemMinY, itemMaxY, P2);
}
//-----------------------------------------------------------------------------
QString VizCTrendLine::toolTip(const QPointF& p) const
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
		.arg(tr("Trend Line"))
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
f64 VizCTrendLine::startValue() const
{
	return StartValue;
}
//-----------------------------------------------------------------------------
void VizCTrendLine::setStartValue(f64 value)
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
f64 VizCTrendLine::endValue() const
{
	return EndValue;
}
//-----------------------------------------------------------------------------
void VizCTrendLine::setEndValue(f64 value)
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
QDateTime VizCTrendLine::startDate() const
{
	return StartDate;
}
//-----------------------------------------------------------------------------
void VizCTrendLine::setStartDate(const QDateTime& value)
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
QDateTime VizCTrendLine::endDate() const
{
	return EndDate;
}
//-----------------------------------------------------------------------------
void VizCTrendLine::setEndDate(const QDateTime& value)
{
	if(EndDate != value)
	{
		QDateTime oldValue = EndDate;

		EndDate = value;

		emit createUndoCommandPropertyChanged(this, "endDate", oldValue, EndDate, true);
		emit itemChanged(true);
	}
}
//-----------------------------------------------------------------------------
bool VizCTrendLine::leftExtention() const
{
	return LeftExtention;
}
//-----------------------------------------------------------------------------
void VizCTrendLine::setLeftExtention(bool value)
{
	if(LeftExtention != value)
	{
		LeftExtention = value;
		build();
		emit itemChanged(true);
	}
}
//-----------------------------------------------------------------------------
bool VizCTrendLine::rightExtention() const
{
	return RightExtention;
}
//-----------------------------------------------------------------------------
void VizCTrendLine::setRightExtention(bool value)
{
	if(RightExtention != value)
	{
		RightExtention = value;
		build();
		emit itemChanged(true);
	}
}
//-----------------------------------------------------------------------------
void VizCTrendLine::endEdit(const VizSShapeTransform& transform)
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
			EndValue = CurrentTransform.CurrentPostion.y();
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
			if(isSemiLogarithmicScale)
			{
				delta.setY(core::pow10f(CurrentTransform.CurrentPostion.y()) - core::pow10f(CurrentTransform.StartPostion.y()));
			}

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

			StartValue = StartValue + delta.y();
			EndValue = EndValue + delta.y();
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
				newValues << EndValue << EndDate;

				bool itemChangedEvent = oldValues[0] != newValues[0];
				itemChangedEvent |= oldValues[1] != newValues[1];
				if(!itemChangedEvent)
				{
					break;
				}

				QStringList propertiesNames;
				propertiesNames << "endValue" << "endDate";

				WorkArea->updateProperties(this, propertiesNames);

				emit createUndoCommandPropertiesChanged(this, propertiesNames, oldValues, newValues, true, tr("Mover"));
			}

			emit itemChanged(true);

			break;
		}
	}
}
//-----------------------------------------------------------------------------
void VizCTrendLine::edit(const VizSShapeTransform& transform)
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
QList<QPointF> VizCTrendLine::controlPoints() const
{
	QList<QPointF> points;
	points << P1 << P2;
	return points;
}
//-----------------------------------------------------------------------------
void VizCTrendLine::paintRubberRands(QPainter* painter)
{
	bool isSemiLogarithmicScale = VerticalScale->isSemiLogarithmicScale();

	QPointF p1, p2;
	switch(CurrentTransform.Type)
	{
		case VESTT_ADDING_NEW_ITEM:
		{
			p1 = CurrentTransform.StartPostion;
			p2 = CurrentTransform.CurrentPostion;
			break;
		}
		case VESTT_MOVE:
		{
			QPointF delta = CurrentTransform.CurrentPostion - CurrentTransform.StartPostion;

			//qDebug() << delta;

			if(isSemiLogarithmicScale)
			{
				float dy1 = core::pow10f(CurrentTransform.StartPostion.y()) - core::pow10f(P1.y());
				float dy2 = core::pow10f(CurrentTransform.StartPostion.y()) - core::pow10f(P2.y());

				float curMouseY = core::pow10f(CurrentTransform.CurrentPostion.y());

				float dy1New = core::clampLog10f(curMouseY - dy1);
				float dy2New = core::clampLog10f(curMouseY - dy2);

				p1.setY(dy1New);
				p2.setY(dy2New);			
			}
			else
			{
				p1.setY(P1.y() + (delta.y()));
				p2.setY(P2.y() + (delta.y()));
			}
			p1.setX(P1.x() + (delta.x()));
			p2.setX(P2.x() + (delta.x()));

			break;
		}
		case VESTT_CONTROL_POINTS:
		{
			if(CurrentTransform.ActiveControlPointIndex == 0)
			{
				p1 = CurrentTransform.CurrentPostion;
				p2 = P2;
			}
			else
			if(CurrentTransform.ActiveControlPointIndex == 1)
			{
				p1 = P1;
				p2 = CurrentTransform.CurrentPostion;
			}
			break;
		}
	}

	/*
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
	}*/


	QColor shadowColor = computeShadowColor(painter->background().color());
	QPen shadowPen(shadowColor, 1.0, Qt::DashLine);
	shadowPen.setCosmetic(true);
	painter->setPen(shadowPen);
	painter->drawLine(p1, p2);


	if(RightExtention || LeftExtention)
	{
		QPointF extentions[4];
		computeExtentionsPoints(p1, p2, extentions);
		if(LeftExtention)
		{
			painter->drawLine(extentions[0], extentions[1]);
		}
		if(RightExtention)
		{
			painter->drawLine(extentions[2], extentions[3]);
		}
	}
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------

