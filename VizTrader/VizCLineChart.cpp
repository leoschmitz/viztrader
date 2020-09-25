//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCLineChart.h"
#include "VizIGraphicsItemBasedOnShape.h"
#include "VizMath.h"
#include "VizGUIUtils.h"
#include <QDebug>
#include <QPen>
#include <QVariant>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
VizCLineChart::VizCLineChart() : VizIChart()
{
}
//-----------------------------------------------------------------------------
VizCLineChart::~VizCLineChart()
{
}
//-----------------------------------------------------------------------------
void VizCLineChart::build()
{
	if(Periods.size() < 2) return;

	s32 rulesValuesIndexBegin;
	s32 rulesValuesIndexEnd;
	HorizontalScale->currentVisibleRulerValuesIndex(rulesValuesIndexBegin, rulesValuesIndexEnd);
	bool isSemiLogarithmicScale = VerticalScale->isSemiLogarithmicScale();

	LinesUp.clear();
	LinesDown.clear();


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
			f64 price1 = Period1.ClosePrice;
			f64 price2 = Period2.ClosePrice;
			if(Period2.ClosePrice >= Period1.ClosePrice)
			{
				LinesUp.append(QPointF(rulesValuesIndexBegin-1, price1));
				LinesUp.append(QPointF(rulesValuesIndexBegin-0, price2));
			}
			else
			{
				LinesDown.append(QPointF(rulesValuesIndexBegin-1, price1));
				LinesDown.append(QPointF(rulesValuesIndexBegin-0, price2));
			}
		}	
	}

	//desenhar a borda right
	if(rulesValuesIndexEnd < Periods.size() - 1)
	{
		//s32 size = rulesValuesIndexEnd - rulesValuesIndexBegin + 1;
		core::VizCTradePeriod Period1 = Periods[rulesValuesIndexEnd + 0];
		core::VizCTradePeriod Period2 = Periods[rulesValuesIndexEnd + 1];
		if(isSemiLogarithmicScale)
		{
			Period1 = Period1.toLogarithmic();
			Period2 = Period2.toLogarithmic();
		}
		if(Period1.isValid() && Period2.isValid())
		{
			f64 price1 = Period1.ClosePrice;
			f64 price2 = Period2.ClosePrice;
			if(Period2.ClosePrice >= Period1.ClosePrice)
			{
				LinesUp.append(QPointF(rulesValuesIndexEnd + 0 , price1));
				LinesUp.append(QPointF(rulesValuesIndexEnd + 1, price2));
			}
			else
			{
				LinesDown.append(QPointF(rulesValuesIndexEnd + 0, price1));
				LinesDown.append(QPointF(rulesValuesIndexEnd + 1, price2));
			}
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
			f64 price1 = Period1.ClosePrice;
			f64 price2 = Period2.ClosePrice;

			if(Period2.ClosePrice >= Period1.ClosePrice)
			{
				LinesUp.append(QPointF(i + 0, price1));
				LinesUp.append(QPointF(i + 1, price2));
			}
			else
			{
				LinesDown.append(QPointF(i + 0, price1));
				LinesDown.append(QPointF(i + 1, price2));
			}
		}
	}

}
//-----------------------------------------------------------------------------
void VizCLineChart::paint(QPainter* painter)
{
	qreal pw = 0.0;
	if(PenWidth != viz::VEPW_1)
	{
		pw = static_cast<qreal>(PenWidth);
	}

	UpColor.setAlphaF(Visibility);
	DownColor.setAlphaF(Visibility);

	QPen upPen(UpColor, pw, Qt::SolidLine, DefaultPenCapStyle, DefaultPenJoinStyle);
	QPen downPen(DownColor, pw, Qt::SolidLine, DefaultPenCapStyle, DefaultPenJoinStyle);
	upPen.setCosmetic(true);
	downPen.setCosmetic(true);

	if(IsSelected)
	{
		QColor shadowColor = gui::computeShadowColor(painter->background().color());
		shadowColor.setAlphaF(Visibility);
		QPen shadowPen(shadowColor, pw + gui::ShadowWidth, Qt::SolidLine, DefaultPenCapStyle, DefaultPenJoinStyle);
		shadowPen.setCosmetic(true);
		painter->setPen(shadowPen);
		painter->drawLines(LinesUp);
		painter->drawLines(LinesDown);
	}

	painter->setPen(downPen);
	painter->drawLines(LinesDown);
	painter->setPen(upPen);
	painter->drawLines(LinesUp);
}
//-----------------------------------------------------------------------------
void VizCLineChart::ajustToOverlayWithoutScale(f64 itemMinY, f64 itemMaxY)
{
	ajustLinesToOverlayWithoutScale(itemMinY, itemMaxY, LinesUp);
	ajustLinesToOverlayWithoutScale(itemMinY, itemMaxY, LinesDown);
}
//-----------------------------------------------------------------------------
bool VizCLineChart::intersects(const QRectF& rect) const
{
	bool b = false;

	for(s32 i = 0; i < LinesUp.size() - 1; i +=2)
	{
		const QPointF& p1 = LinesUp[i + 0];
		const QPointF& p2 = LinesUp[i + 1];
		b = core::intersectsLineWithRect(QLineF(p1, p2), rect);
		if(b)return true;

	}
	for(s32 i = 0; i < LinesDown.size() - 1; i += 2)
	{
		const QPointF& p1 = LinesDown[i + 0];
		const QPointF& p2 = LinesDown[i + 1];
		b = core::intersectsLineWithRect(QLineF(p1, p2), rect);
		if(b) return true;

	}
	return b;
}
//-----------------------------------------------------------------------------
bool VizCLineChart::isLinear() const
{
	return true;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
