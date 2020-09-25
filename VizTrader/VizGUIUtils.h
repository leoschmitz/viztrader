//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZGUIUTILS_H__
#define __VIZGUIUTILS_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include "VizCVerticalScale.h"
#include "VizCHorizontalScale.h"
#include <QStyle>
#include <QColor>
#include <QBrush>
#include <QFont>
#include <QRectF>
#include <QPointF>
#include <QColor>
#include <QPainter>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
static const Qt::PenCapStyle DefaultPenCapStyle = Qt::RoundCap;		//Qt::SquareCap;
static const Qt::PenJoinStyle DefaultPenJoinStyle = Qt::RoundJoin;	//Qt::BevelJoin;
static const f64 ShadowWidth = 3.0;
static const f64 MinimalVisibleCell = 3.0;
static const f64 CellWidth = 1.0;
static const f64 CellHalfWidth = 0.5;
static const f64 CandleWidth = 0.75;
//-----------------------------------------------------------------------------
static const QColor VizGreen(50, 205, 50);
static const QColor VizRed(255, 0, 0);
static const QColor VizGray(192, 192, 192);
static const QColor VizBlue(0, 0, 255);
//-----------------------------------------------------------------------------
QString styleValueText(QStyle* s);
QString buttonStyleSheet(const QColor& c);
QString colorValueText(const QColor &c);
QPixmap brushValuePixmap(const QBrush &b);
QPixmap fontValuePixmap(const QFont &font);
QString fontValueText(const QFont &f);
QColor computeShadowColor(const QColor& backGroundColor);
void fillRect(QPainter* painter, const QRectF& rect, const QColor& color, bool useGradientToFillShapes);
QRectF graphicsPanelViewPort(VizCVerticalScale* verticalScale, VizCHorizontalScale* horizontalScale);
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
