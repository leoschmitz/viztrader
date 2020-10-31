//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizGUIUtils.h"
#include "VizCSettings.h"
#include <QStyleFactory>
#include <QPainter>
#include <QDebug>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
QString styleValueText(QStyle* s)
{
	QString name = "";
	foreach(QString styleName, QStyleFactory::keys())
	{
		QStyle* s1 = QStyleFactory::create(styleName);
		if(s->metaObject()->className() == s1->metaObject()->className())
		{
			delete s1;
			name = styleName;
			break;
		}
		delete s1;
	}

	return name;
}
//-----------------------------------------------------------------------------
QString buttonStyleSheet(const QColor& c)
{
	//text-align: left;
	return QString("QPushButton { background: rgba(%1,%2,%3,%4) }").arg(c.red()).arg(c.green()).arg(c.blue()).arg(c.alpha());
}
//-----------------------------------------------------------------------------
QString colorValueText(const QColor &c)
{
	return QString("[%1, %2, %3] (%4)")
		.arg(QString::number(c.red()))
		.arg(QString::number(c.green()))
		.arg(QString::number(c.blue()))
		.arg(QString::number(c.alpha()));
}
//-----------------------------------------------------------------------------
QPixmap brushValuePixmap(const QBrush &b)
{
	QImage img(16, 16, QImage::Format_ARGB32_Premultiplied);
	img.fill(0);

	QPainter painter(&img);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(0, 0, img.width(), img.height(), b);
	QColor color = b.color();
	if (color.alpha() != 255) { // indicate alpha by an inset
		QBrush  opaqueBrush = b;
		color.setAlpha(255);
		opaqueBrush.setColor(color);
		painter.fillRect(img.width() / 4, img.height() / 4,
			img.width() / 2, img.height() / 2, opaqueBrush);
	}
	painter.end();
	return QPixmap::fromImage(img);
}
//-----------------------------------------------------------------------------
QPixmap fontValuePixmap(const QFont &font)
{
	QFont f = font;
	QImage img(16, 16, QImage::Format_ARGB32_Premultiplied);
	img.fill(0);
	QPainter p(&img);
	p.setRenderHint(QPainter::TextAntialiasing, true);
	p.setRenderHint(QPainter::Antialiasing, true);
	f.setPointSize(13);
	p.setFont(f);
	QTextOption t;
	t.setAlignment(Qt::AlignCenter);
	p.drawText(QRect(0, 0, 16, 16), QString(QLatin1Char('A')), t);
	return QPixmap::fromImage(img);
}
//-----------------------------------------------------------------------------
QString fontValueText(const QFont &f)
{
	return QString("[%1, %2]")
		.arg(f.family())
		.arg(f.pointSize());
}
//-----------------------------------------------------------------------------
QColor computeShadowColor(const QColor& backGroundColor)
{
	QColor shadowColor = backGroundColor;
	f64 GrayScaleIntensityF = 0.3 * shadowColor.redF() + 0.59 * shadowColor.greenF() + 0.11 * shadowColor.blueF();
	s32 GrayScaleIntensity = static_cast<s32>(255.0 * (1.0 - GrayScaleIntensityF));
	shadowColor.setRgb(GrayScaleIntensity, GrayScaleIntensity, GrayScaleIntensity);
	return shadowColor;
}
//-----------------------------------------------------------------------------
void fillRect(QPainter* painter, const QRectF& rect, const QColor& color, bool useGradientToFillShapes)
{
	if(useGradientToFillShapes)
	{
		f64 x1 = rect.topLeft().x();
		f64 x2 = rect.topRight().x();
		QLinearGradient gradient(x1, 0, x2 - ((x2-x1) * 0.3), 0);
		gradient.setColorAt(0.0, color);
		gradient.setColorAt(1.0, color.light(160));
		gradient.setSpread(QGradient::ReflectSpread);
		painter->fillRect(rect, gradient);
	}
	else
	{
		painter->fillRect(rect, color);
	}
}
//-----------------------------------------------------------------------------
QRectF graphicsPanelViewPort(VizCVerticalScale* verticalScale, VizCHorizontalScale* horizontalScale)
{
	f64 maxY = verticalScale->maximum();
	f64 minY = verticalScale->minimum();
	if(verticalScale->isSemiLogarithmicScale())
	{
		maxY = core::clampLog10f(maxY);
		minY = core::clampLog10f(minY);
	}

	s32 rulesValuesIndexBegin;
	s32 rulesValuesIndexEnd;
	horizontalScale->currentVisibleRulerValuesIndex(rulesValuesIndexBegin, rulesValuesIndexEnd);

	f64 x = rulesValuesIndexBegin - CellHalfWidth;
	f64 y = maxY;
	f64 w = core::fabs((rulesValuesIndexEnd - rulesValuesIndexBegin) + (2.0 * CellHalfWidth));
	f64 h = -1.0 * core::fabs(maxY-minY);

	if(verticalScale->isInvertScale())
	{
		y = minY;
		h *= -1.0;
	}

	h = core::equals(h, 0.0) ? 1.0 : h;
	w = core::equals(w, 0.0) ? 1.0 : w;
	//w = w < 0.0 ? -w : w;

	//qDebug() << QRectF(x, y, w, h);

	return QRectF(x, y, w, h);

}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
