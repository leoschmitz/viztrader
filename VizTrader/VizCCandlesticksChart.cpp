//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCCandlesticksChart.h"
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
VizCCandlesticksChart::VizCCandlesticksChart() : VizIChart()
{
}
//-----------------------------------------------------------------------------
VizCCandlesticksChart::~VizCCandlesticksChart()
{
}
//-----------------------------------------------------------------------------
void VizCCandlesticksChart::build()
{

	if(Periods.size() < 2) return;

	s32 rulesValuesIndexBegin;
	s32 rulesValuesIndexEnd;
	HorizontalScale->currentVisibleRulerValuesIndex(rulesValuesIndexBegin, rulesValuesIndexEnd);
	bool isSemiLogarithmicScale = VerticalScale->isSemiLogarithmicScale();

	LinesUp.clear();
	LinesDown.clear();
	RectsUp.clear();
	RectsDown.clear();
	FilledRectsUp.clear();
	FilledRectsDown.clear();

	LinesUpLod.clear();
	LinesDownLod.clear();

	s32 indexPrev = rulesValuesIndexBegin > 1 ? rulesValuesIndexBegin - 1 : rulesValuesIndexBegin;
	core::VizCTradePeriod Previous = Periods[indexPrev];
	for(s32 i = rulesValuesIndexBegin; i < rulesValuesIndexEnd + 1; i++)
	{
		core::VizCTradePeriod Current = Periods[i];
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

				QRectF r(x, y, w, h);
				if(Current.ClosePrice < Current.OpenPrice)
				{
					FilledRectsUp.append(r);
				}
				else
				{
					RectsUp.append(r);

				}

				QPointF p1(x + halfw, roof);
				QPointF p2(x + halfw, Current.MaxPrice);
				LinesUp.append(p1);
				LinesUp.append(p2);


				QPointF p3(x + halfw, base);
				QPointF p4(x + halfw, Current.MinPrice);
				LinesUp.append(p3);
				LinesUp.append(p4);


				LinesUpLod.append(p2);
				LinesUpLod.append(p4);


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

				QRectF r(x, y, w, h);
				if(Current.ClosePrice < Current.OpenPrice)
				{
					FilledRectsDown.append(r);
				}
				else
				{
					RectsDown.append(r);
				}

				QPointF p1(x + halfw, roof);
				QPointF p2(x + halfw, Current.MaxPrice);
				LinesDown.append(p1);
				LinesDown.append(p2);

				QPointF p3(x + halfw, base);
				QPointF p4(x + halfw, Current.MinPrice);
				LinesDown.append(p3);
				LinesDown.append(p4);


				LinesDownLod.append(p2);
				LinesDownLod.append(p4);

			}
		}

		Previous = Periods[i];


	}
}
//-----------------------------------------------------------------------------
void VizCCandlesticksChart::paint(QPainter* painter)
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
	s32 pointsCount = RectsDown.size() + RectsUp.size();
	s32 w = painter->viewport().width();
	const s32 pixels = 2;
	bool lod = (w < (pointsCount * 2.0 * pixels)) ? true : false;
	if(lod)
	{
		//qDebug() << "com LOD";
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
		//qDebug() << "sem LOD";
		if(IsSelected)
		{
			painter->setPen(shadowPen);
			painter->drawLines(LinesUp);
			painter->drawRects(RectsUp);
			painter->drawRects(FilledRectsUp);
			painter->drawLines(LinesDown);
			painter->drawRects(RectsDown);
			painter->drawRects(FilledRectsDown);
		}

		painter->setPen(downPen);
		painter->drawLines(LinesDown);
		painter->drawRects(RectsDown);

		for(s32 i = 0; i < FilledRectsDown.size(); i++)
		{
			gui::fillRect(painter, FilledRectsDown[i], DownColor, UseGradientToFillShapes);
		}

		painter->setPen(upPen);
		painter->drawLines(LinesUp);
		painter->drawRects(RectsUp);
		for(s32 i = 0; i < FilledRectsUp.size(); i++)
		{
			gui::fillRect(painter, FilledRectsUp[i], UpColor, UseGradientToFillShapes);
		}
	}


}
//-----------------------------------------------------------------------------
void VizCCandlesticksChart::ajustToOverlayWithoutScale(f64 itemMinY, f64 itemMaxY)
{
	ajustLinesToOverlayWithoutScale(itemMinY, itemMaxY, LinesUp);
	ajustLinesToOverlayWithoutScale(itemMinY, itemMaxY, LinesDown);
	ajustRectsToOverlayWithoutScale(itemMinY, itemMaxY, RectsUp);
	ajustRectsToOverlayWithoutScale(itemMinY, itemMaxY, RectsDown);
	ajustRectsToOverlayWithoutScale(itemMinY, itemMaxY, FilledRectsUp);
	ajustRectsToOverlayWithoutScale(itemMinY, itemMaxY, FilledRectsDown);
}
//-----------------------------------------------------------------------------
bool VizCCandlesticksChart::intersects(const QRectF& rect) const
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
	for(s32 i = 0; i < RectsUp.size(); i++)
	{
		const QRectF& r = RectsUp[i];
		b = core::intersectsRectWithValidRect(r, rect);
		if(b) return true;
	}
	for(s32 i = 0; i < FilledRectsUp.size(); i++)
	{
		const QRectF& r = FilledRectsUp[i];
		b = core::intersectsRectWithValidRect(r, rect);
		if(b) return true;
	}
	for(s32 i = 0; i < RectsDown.size(); i++)
	{
		const QRectF& r = RectsDown[i];
		b = core::intersectsRectWithValidRect(r, rect);
		if(b) return true;
	}

	for(s32 i = 0; i < FilledRectsDown.size(); i++)
	{
		const QRectF& r = FilledRectsDown[i];
		b = core::intersectsRectWithValidRect(r, rect);
		if(b) return true;
	}

	return b;
}
//-----------------------------------------------------------------------------
bool VizCCandlesticksChart::isLinear() const
{
	return true;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
