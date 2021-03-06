//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCHeikinAshiBarsChart.h"
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
VizCHeikinAshiBarsChart::VizCHeikinAshiBarsChart() : VizIChart()
{
}
//-----------------------------------------------------------------------------
VizCHeikinAshiBarsChart::~VizCHeikinAshiBarsChart()
{
}
//-----------------------------------------------------------------------------
void VizCHeikinAshiBarsChart::build()
{

	if(Periods.size() < 2) return;

	s32 rulesValuesIndexBegin;
	s32 rulesValuesIndexEnd;
	HorizontalScale->currentVisibleRulerValuesIndex(rulesValuesIndexBegin, rulesValuesIndexEnd);
	bool isSemiLogarithmicScale = VerticalScale->isSemiLogarithmicScale();

	LinesUp.clear();
	LinesDown.clear();

	LinesUpLod.clear();
	LinesDownLod.clear();

	s32 indexPrev = rulesValuesIndexBegin > 1 ? rulesValuesIndexBegin - 1 : rulesValuesIndexBegin;
	core::VizCTradePeriod Previous = Periods[indexPrev];
	for(s32 i = rulesValuesIndexBegin; i < rulesValuesIndexEnd + 1; i++)
	{
		core::VizCTradePeriod Current  = Periods[i - 0];

		/*
		Heikin-Ashi Technique
		xClose = (Open+High+Low+Close)/4						//Average price of the current bar
		xOpen = [xOpen(Previous Bar) + xClose(Previous Bar)]/2	//Midpoint of the previous bar
		xHigh = Max(High, xOpen, xClose)						//Highest value in the set
		xLow = Min(Low, xOpen, xClose)							//Lowest value in the set
		*/

		f64 xClose = (Current.OpenPrice + Current.MaxPrice + Current.MinPrice + Current.ClosePrice) / 4.0;
		f64 xClose_PreviousBar = (Previous.OpenPrice + Previous.MaxPrice + Previous.MinPrice + Previous.ClosePrice) / 4.0;
		f64 xOpen_PreviousBar;

		if(i == rulesValuesIndexBegin)
		{
			xOpen_PreviousBar = Previous.OpenPrice;
		}
		else
		{
			xOpen_PreviousBar = (Previous.OpenPrice + xClose_PreviousBar) / 2.0;
		}
		f64 xOpen = (xOpen_PreviousBar + xClose_PreviousBar) / 2.0;
		f64 xHigh = viz::core::max(Current.MaxPrice, xOpen, xClose);
		f64 xLow = viz::core::min(Current.MinPrice, xOpen, xClose);

		Current.OpenPrice = xOpen;
		Current.ClosePrice = xClose;
		Current.MaxPrice = xHigh;
		Current.MinPrice = xLow;


		if(isSemiLogarithmicScale)
		{
			Current = Current.toLogarithmic();
			Previous = Previous.toLogarithmic();
		}

		f64 base, roof;
		f64 w = gui::CandleWidth;
		f64 halfw = (w * 0.5);

		if(Current.isValid() && Previous.isValid())
		{
			if(Current.ClosePrice > Previous.ClosePrice || i == 0)
			{
				if(Current.ClosePrice > Current.OpenPrice)
				{
					base = Current.OpenPrice;
					roof = Current.ClosePrice;
				}
				else
				{
					roof = Current.OpenPrice;
					base = Current.ClosePrice;
				}

				f64 h = roof - base;
				f64 x = i - halfw;
				f64 y = base;

				QPointF p1(x + halfw, Current.MaxPrice);
				QPointF p2(x + halfw, Current.MinPrice);
				QPointF p3(x + halfw, base);
				QPointF p4(x, base);
				QPointF p5(x + halfw, roof);
				QPointF p6(x + 2 * halfw, roof);

				LinesUp.append(p1);
				LinesUp.append(p2);
				LinesUp.append(p3);
				LinesUp.append(p4);
				LinesUp.append(p5);
				LinesUp.append(p6);

				LinesUpLod.append(p1);
				LinesUpLod.append(p2);
			}
			else
			{
				if(Current.ClosePrice > Current.OpenPrice)
				{
					base = Current.OpenPrice;
					roof = Current.ClosePrice;
				}
				else
				{
					roof = Current.OpenPrice;
					base = Current.ClosePrice;
				}

				f64 h = roof - base;
				f64 x = i - halfw;
				f64 y = base;

				QPointF p1(x + halfw, Current.MaxPrice);
				QPointF p2(x + halfw, Current.MinPrice);
				QPointF p3(x + halfw, roof);
				QPointF p4(x, roof);
				QPointF p5(x + halfw, base);
				QPointF p6(x + 2 * halfw, base);

				LinesDown.append(p1);
				LinesDown.append(p2);
				LinesDown.append(p3);
				LinesDown.append(p4);
				LinesDown.append(p5);
				LinesDown.append(p6);

				LinesDownLod.append(p1);
				LinesDownLod.append(p2);
			}
		}

		Previous = Periods[i];
	}

}
//-----------------------------------------------------------------------------
void VizCHeikinAshiBarsChart::paint(QPainter* painter)
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

	QColor shadowColor = gui::computeShadowColor(painter->background().color());
	shadowColor.setAlphaF(Visibility);
	QPen shadowPen(shadowColor, pw + gui::ShadowWidth, Qt::SolidLine, DefaultPenCapStyle, DefaultPenJoinStyle);
	shadowPen.setCosmetic(true);


	//LOD - to reduce the Moiré pattern
	s32 pointsCount = LinesDownLod.size() + LinesUpLod.size();
	s32 w = painter->viewport().width();
	bool lod = (w < pointsCount * 0.8) ? true : false;

	if(lod)
	{
		//qDebug() << "com LOD1";

		if(IsSelected)
		{
			painter->setPen(shadowPen);

			painter->drawLines(LinesUpLod);
			painter->drawLines(LinesDownLod);
		}

		painter->setPen(downPen);
		painter->drawLines(LinesDownLod);
		painter->setPen(upPen);
		painter->drawLines(LinesUpLod);

	}
	else
	{
		//qDebug() << "sem com LOD";

		if(IsSelected)
		{
			painter->setPen(shadowPen);

			painter->drawLines(LinesUp);
			painter->drawLines(LinesDown);
		}

		painter->setPen(downPen);
		painter->drawLines(LinesDown);
		painter->setPen(upPen);
		painter->drawLines(LinesUp);

	}

}
//-----------------------------------------------------------------------------
void VizCHeikinAshiBarsChart::ajustToOverlayWithoutScale(f64 itemMinY, f64 itemMaxY)
{
	ajustLinesToOverlayWithoutScale(itemMinY, itemMaxY, LinesUp);
	ajustLinesToOverlayWithoutScale(itemMinY, itemMaxY, LinesDown);
}
//-----------------------------------------------------------------------------
bool VizCHeikinAshiBarsChart::intersects(const QRectF& rect) const
{
	bool b = false;

	for(s32 i = 0; i < LinesUp.size() - 1; i += 2)
	{
		const QPointF& p1 = LinesUp[i + 0];
		const QPointF& p2 = LinesUp[i + 1];
		b = core::intersectsLineWithRect(QLineF(p1, p2), rect);
		if(b) return true;
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
bool VizCHeikinAshiBarsChart::isLinear() const
{
	return true;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
