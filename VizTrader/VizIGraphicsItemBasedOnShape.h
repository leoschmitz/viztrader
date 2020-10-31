//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZIGRAPHICSITEMBASEDONSHAPE_H__
#define __VIZIGRAPHICSITEMBASEDONSHAPE_H__
//-----------------------------------------------------------------------------
#include "VizIGraphicsItem.h"
#include "VizCPropertyEditor.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
enum VizEShapeTransformType
{
	VESTT_NONE = 0,
	VESTT_ADDING_NEW_ITEM,
	VESTT_CONTROL_POINTS,
	VESTT_MOVE
};
//-----------------------------------------------------------------------------
struct VizSShapeTransform
{
	VizEShapeTransformType Type;
	QPointF StartPostion;
	QPointF CurrentPostion;
	s32 ActiveControlPointIndex;
};
//-----------------------------------------------------------------------------
class VizIGraphicsItemBasedOnShape : public VizIGraphicsItem
{
	Q_OBJECT
	Q_PROPERTY(QColor color READ color WRITE setColor USER true)
	Q_PROPERTY(bool snapToPrice READ snapToPrice WRITE setSnapToPrice USER true)
	Q_PROPERTY(int penStyle READ propertyGetPenStyle WRITE propertySetPenStyle USER true)
	Q_PROPERTY(int penWidth READ propertyGetPenWidth WRITE propertySetPenWidth USER true)

public:
	VizIGraphicsItemBasedOnShape();
	~VizIGraphicsItemBasedOnShape();

	QColor color() const;
	void setColor(const QColor& color);
	bool snapToPrice() const;
	void setSnapToPrice(bool value);
	viz::VizEPenWidth penWidth() const;
	void setPenWidth(viz::VizEPenWidth penWidth);
	viz::VizEPenStyle penStyle() const;
	void setPenStyle(VizEPenStyle penStyle);

	virtual void resetToDefaults();
	virtual void paint(QPainter* painter);
	virtual bool isEditing();
	
	virtual void beginEdit(const VizSShapeTransform& transform);
	virtual void edit(const VizSShapeTransform& transform);
	virtual void endEdit(const VizSShapeTransform& transform);
	virtual QList<QPointF> controlPoints() const = 0;

protected:
	virtual void paintRubberRands(QPainter* painter) = 0;
	int propertyGetPenStyle() const;
	void propertySetPenStyle(int value);
	int propertyGetPenWidth() const;
	void propertySetPenWidth(int value);

	VizSShapeTransform CurrentTransform;
	QColor Color;
	bool Editing;
	bool ShowRubberBands;
	bool SnapToPrice;
	viz::VizEPenStyle PenStyle;
	viz::VizEPenWidth PenWidth;

};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
