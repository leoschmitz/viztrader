//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCVerticalLine.h"
#include "VizIGraphicsItemBasedOnSecurity.h"
#include "VizGUIUtils.h"
#include "VizMath.h"
#include <QDebug>
#include <QDateTime>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
VizCVerticalLine::VizCVerticalLine() : VizIGraphicsItemBasedOnShape()
{
	blockSignals(true);
	resetToDefaults();
	blockSignals(false);

	setProperty("name", tr("Linha Vertical"));
}
//-----------------------------------------------------------------------------
VizCVerticalLine::~VizCVerticalLine()
{
}
//-----------------------------------------------------------------------------
void VizCVerticalLine::resetToDefaults()
{
	QStringList names;
	QVariantList oldValues;
	QVariantList newValues;

	gui::vizObjectGetAllPropertiesNamesAndValues(this, names, oldValues);

	VizIGraphicsItemBasedOnShape::resetToDefaults();
	PenWidth = static_cast<viz::VizEPenWidth>(Settings->value("Appearance/Items/LineWidth").toInt());
	PenStyle = static_cast<viz::VizEPenStyle>(Settings->value("Appearance/Items/LineStyle").toInt());
	Color = QColor(Settings->value("Appearance/Items/Color").toString());

	gui::vizObjectGetAllPropertiesNamesAndValues(this, names, newValues);

	emit createUndoCommandPropertiesChanged(this, names, oldValues, newValues, true, tr("Valores Padrão"));
	emit itemChanged(true);
}
//-----------------------------------------------------------------------------
void VizCVerticalLine::build()
{
	bool isSemiLogarithmicScale = VerticalScale->isSemiLogarithmicScale();

	FindDateTimeInHorizontalScale = HorizontalScale->findRuleValueIndex(DateIndex, Value, true);
	f64 x = DateIndex;

	QRectF viewport = graphicsPanelViewPort();

	/*BeginY = VerticalScale->minimum();
	EndY = VerticalScale->maximum();

	if(isSemiLogarithmicScale)
	{
		BeginY = core::clampLog10f(BeginY);
		EndY = core::clampLog10f(EndY);
	}

	P1 = QPointF(x, BeginY);
	P2 = QPointF(x, EndY);
	*/
	P1 = QPointF(x, viewport.top());
	P2 = QPointF(x, viewport.bottom());

}
//-----------------------------------------------------------------------------
void VizCVerticalLine::paint(QPainter* painter)
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

	VizIGraphicsItemBasedOnShape::paint(painter);

}
//-----------------------------------------------------------------------------
void VizCVerticalLine::yRange(bool& isEmpty, f64& minY, f64& maxY) const
{
	QRectF viewport = graphicsPanelViewPort();

	maxY = qMax<f64>(maxY, viewport.top());
	minY = qMin<f64>(minY, viewport.bottom());
	isEmpty = false;
}
//-----------------------------------------------------------------------------
bool VizCVerticalLine::intersects(const QRectF& rect) const
{
	return core::intersectsLineWithRect(QLineF(P1, P2), rect);
}
//-----------------------------------------------------------------------------
void VizCVerticalLine::ajustToOverlayWithoutScale(f64 itemMinY, f64 itemMaxY)
{
	ajustPointToOverlayWithoutScale(itemMinY, itemMaxY, P1);
	ajustPointToOverlayWithoutScale(itemMinY, itemMaxY, P2);
}
//-----------------------------------------------------------------------------
QString VizCVerticalLine::toolTip(const QPointF& p) const
{
	const QVector<QDateTime>& rv = HorizontalScale->rulerValues();
	const QDateTime& dateTime = rv[p.x()];
	QString s = QString(tr("%1\nData: %3"))
		.arg(tr("Linha Vertical"))
		.arg(dateTime.date().toString(Qt::SystemLocaleShortDate));
	return s;
}
//-----------------------------------------------------------------------------
QDateTime VizCVerticalLine::value() const
{
	return Value;
}
//-----------------------------------------------------------------------------
void VizCVerticalLine::setValue(const QDateTime& value)
{
	if(Value != value)
	{
		QDateTime oldValue = Value;
		Value = value;

		emit createUndoCommandPropertyChanged(this, "value", oldValue, Value, true);
		emit itemChanged(true);
	}
}
//-----------------------------------------------------------------------------
void VizCVerticalLine::endEdit(const VizSShapeTransform& transform)
{
	VizIGraphicsItemBasedOnShape::endEdit(transform);
	const QDateTime& value = HorizontalScale->rulerValues()[static_cast<s32>(CurrentTransform.CurrentPostion.x())];
	setValue(value);
}
//-----------------------------------------------------------------------------
QList<QPointF> VizCVerticalLine::controlPoints() const
{
	QList<QPointF> points;
	QPointF p = (P1 + P2) * 0.5;
	points << p;
	return points;
}
//-----------------------------------------------------------------------------
void VizCVerticalLine::paintRubberRands(QPainter* painter)
{

	QRectF viewport = graphicsPanelViewPort();

	f64 value = CurrentTransform.CurrentPostion.x();

	QPointF p1 = QPointF(value, viewport.top());
	QPointF p2 = QPointF(value, viewport.bottom());

	QColor shadowColor = computeShadowColor(painter->background().color());
	QPen shadowPen(shadowColor, 1.0, Qt::DashLine);
	shadowPen.setCosmetic(true);
	painter->setPen(shadowPen);
	painter->drawLine(p1, p2);

}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
