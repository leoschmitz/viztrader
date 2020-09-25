//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCHorizontalLine.h"
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
VizCHorizontalLine::VizCHorizontalLine() : VizIGraphicsItemBasedOnShape(), Value(0.0)
{

	blockSignals(true);
	resetToDefaults();
	blockSignals(false);

	setProperty("name", tr("Linha Horizontal"));

}
//-----------------------------------------------------------------------------
VizCHorizontalLine::~VizCHorizontalLine()
{
}
//-----------------------------------------------------------------------------
void VizCHorizontalLine::resetToDefaults()
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
void VizCHorizontalLine::build()
{
	QRectF viewport = graphicsPanelViewPort();
	bool isSemiLogarithmicScale = VerticalScale->isSemiLogarithmicScale();

	f64 value = Value;

	if(isSemiLogarithmicScale)
	{
		value = core::clampLog10f(value);
	}

	P1 = QPointF(viewport.left(), value);
	P2 = QPointF(viewport.right(), value);
}
//-----------------------------------------------------------------------------
void VizCHorizontalLine::paint(QPainter* painter)
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
void VizCHorizontalLine::yRange(bool& isEmpty, f64& minY, f64& maxY) const
{
	maxY = qMax<f64>(maxY, Value + 1.0);
	minY = qMin<f64>(minY, Value - 1.0);
	isEmpty = false;
}
//-----------------------------------------------------------------------------
bool VizCHorizontalLine::intersects(const QRectF& rect) const
{
	return core::intersectsLineWithRect(QLineF(P1, P2), rect);
}
//-----------------------------------------------------------------------------
void VizCHorizontalLine::ajustToOverlayWithoutScale(f64 itemMinY, f64 itemMaxY)
{
	ajustPointToOverlayWithoutScale(itemMinY, itemMaxY, P1);
	ajustPointToOverlayWithoutScale(itemMinY, itemMaxY, P2);
}
//-----------------------------------------------------------------------------
QString VizCHorizontalLine::toolTip(const QPointF& p) const
{
	const QVector<QDateTime>& rv = HorizontalScale->rulerValues();
	const QDateTime& dateTime = rv[p.x()];
	QString s = QString(tr("%1\nValor: %L2\nData: %3"))
		.arg(tr("Linha Horizontal"))
		.arg(Value, 0, 'f', 2)
		.arg(dateTime.date().toString(Qt::SystemLocaleShortDate));
	return s;
}
//-----------------------------------------------------------------------------
f64 VizCHorizontalLine::value() const
{
	return Value;
}
//-----------------------------------------------------------------------------
void VizCHorizontalLine::setValue(f64 value)
{
	if(Value != value)
	{
		f64 oldValue = Value;
		Value = value;

		emit createUndoCommandPropertyChanged(this, "value", oldValue, Value, true);
		emit itemChanged(true);
	}
}
//-----------------------------------------------------------------------------
void VizCHorizontalLine::endEdit(const VizSShapeTransform& transform)
{
	VizIGraphicsItemBasedOnShape::endEdit(transform);

	/*QPointF delta = CurrentTransform.CurrentPostion - CurrentTransform.StartPostion;
	if(core::equals(delta.y(), 0.0))
	{
		return;
	}*/

	f64 value = CurrentTransform.CurrentPostion.y();
	if(VerticalScale->isSemiLogarithmicScale())
	{
		value = core::pow10f(value);
	}
	setValue(value);
	
	WorkArea->updateProperty(this, "value");

}
//-----------------------------------------------------------------------------
QList<QPointF> VizCHorizontalLine::controlPoints() const
{
	QList<QPointF > points;
	QPointF p = (P1 + P2) * 0.5;
	points.append(p);
	return points;
}
//-----------------------------------------------------------------------------
void VizCHorizontalLine::paintRubberRands(QPainter* painter)
{

	QRectF viewport = graphicsPanelViewPort();

	f64 value = CurrentTransform.CurrentPostion.y();

	QPointF p1 = QPointF(viewport.left(), value);
	QPointF p2 = QPointF(viewport.right(), value);

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
