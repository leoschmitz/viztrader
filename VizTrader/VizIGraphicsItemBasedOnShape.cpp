//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizIGraphicsItemBasedOnShape.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
VizIGraphicsItemBasedOnShape::VizIGraphicsItemBasedOnShape() : VizIGraphicsItem(), Editing(false), ShowRubberBands(false)
{
	resetToDefaults();
}
//-----------------------------------------------------------------------------
VizIGraphicsItemBasedOnShape::~VizIGraphicsItemBasedOnShape()
{
}
//-----------------------------------------------------------------------------
void VizIGraphicsItemBasedOnShape::resetToDefaults()
{
	VizIGraphicsItem::resetToDefaults();

	PenWidth = static_cast<viz::VizEPenWidth>(Settings->value("Appearance/Items/LineWidth").toInt());
	PenStyle = static_cast<viz::VizEPenStyle>(Settings->value("Appearance/Items/LineStyle").toInt());
	Color  = Qt::blue;
	SnapToPrice = false;

}
//-----------------------------------------------------------------------------
bool VizIGraphicsItemBasedOnShape::snapToPrice() const
{
	return SnapToPrice;
}
//-----------------------------------------------------------------------------
void VizIGraphicsItemBasedOnShape::setSnapToPrice(bool value)
{
	if(SnapToPrice != value)
	{
		bool old = SnapToPrice;
		SnapToPrice = value;
		build();

		emit createUndoCommandPropertyChanged(this, "snapToPrice", old, SnapToPrice, true);
		emit itemChanged(true);
	}
}
//-----------------------------------------------------------------------------
VizEPenWidth VizIGraphicsItemBasedOnShape::penWidth() const
{
	return PenWidth;
}
//-----------------------------------------------------------------------------
void VizIGraphicsItemBasedOnShape::setPenWidth(VizEPenWidth penWidth)
{
	if(PenWidth != penWidth)
	{
		int oldValue = propertyGetPenWidth();

		PenWidth = penWidth;

		emit createUndoCommandPropertyChanged(this, "penWidth", oldValue, propertyGetPenWidth(), false);
		emit itemChanged();
	}
}
//-----------------------------------------------------------------------------
int VizIGraphicsItemBasedOnShape::propertyGetPenWidth() const
{
	return static_cast<int>(penWidth() - 1);
}
//-----------------------------------------------------------------------------
void VizIGraphicsItemBasedOnShape::propertySetPenWidth(int value)
{
	setPenWidth(static_cast<VizEPenWidth>(value + 1));
}
//-----------------------------------------------------------------------------
viz::VizEPenStyle VizIGraphicsItemBasedOnShape::penStyle() const
{
	return PenStyle;
}
//-----------------------------------------------------------------------------
void VizIGraphicsItemBasedOnShape::setPenStyle(VizEPenStyle penStyle)
{
	if(PenStyle != penStyle)
	{
		int oldValue = propertyGetPenStyle();

		PenStyle = penStyle;

		emit createUndoCommandPropertyChanged(this, "penStyle", oldValue, propertyGetPenStyle(), false);
		emit itemChanged();
	}
}
//-----------------------------------------------------------------------------
int VizIGraphicsItemBasedOnShape::propertyGetPenStyle() const
{
	return static_cast<viz::VizEPenStyle>(penStyle());
}
//-----------------------------------------------------------------------------
void VizIGraphicsItemBasedOnShape::propertySetPenStyle(int value)
{
	setPenStyle(static_cast<viz::VizEPenStyle>(value));
}
//-----------------------------------------------------------------------------
bool VizIGraphicsItemBasedOnShape::isEditing()
{
	return Editing;
}
//-----------------------------------------------------------------------------
void VizIGraphicsItemBasedOnShape::edit(const VizSShapeTransform& transform)
{
	Editing = true;
	ShowRubberBands = true;
	CurrentTransform = transform;
}
//-----------------------------------------------------------------------------
void VizIGraphicsItemBasedOnShape::beginEdit(const VizSShapeTransform& transform)
{
	ShowRubberBands = false;
	Editing = false;
	CurrentTransform = transform;
}
//-----------------------------------------------------------------------------
void VizIGraphicsItemBasedOnShape::endEdit(const VizSShapeTransform& transform)
{
	ShowRubberBands = false;
	Editing = false;
	CurrentTransform = transform;
}
//-----------------------------------------------------------------------------
void VizIGraphicsItemBasedOnShape::paint(QPainter* painter)
{
	if(ShowRubberBands)
	{
		paintRubberRands(painter);
	}
}
//-----------------------------------------------------------------------------
QColor VizIGraphicsItemBasedOnShape::color() const
{
	return Color;
}
//-----------------------------------------------------------------------------
void VizIGraphicsItemBasedOnShape::setColor(const QColor& color)
{
	if(Color != color)
	{
		QColor old = Color;
		Color = color;
		
		emit createUndoCommandPropertyChanged(this, "color", old, Color, false);
		emit itemChanged();
	}
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
