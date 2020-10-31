//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCVerticalScale.h"
#include "VizCGraphicsPanel.h"
#include "VizCToolManager.h"
#include "VizCWorkArea.h"
#include "VizMath.h"
#include <QDebug>
#include <QPolygonF>
#include <QPolygon>
#include <QPainter>
#include <QHBoxLayout>
#include <QTextDocument>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
const int Width = 46;
const s32 MaximumFractionNumbers = 5;
const f64 MinimumInFraction = 100000.0;
const s32 RulerMarkPxlSize = 3;
const s32 PxlThreshold = 15;
//-----------------------------------------------------------------------------
VizCVerticalScale::VizCVerticalScale(VizCGraphicsPanel* parent, VizCWorkArea* workArea) : QWidget(parent), ClampedDigits(0), Precision(1, 0), IsSelected(false), WorkArea(workArea)
{
	Q_ASSERT(parent);
	Q_ASSERT(WorkArea);

	setAttribute(Qt::WA_DeleteOnClose);

	Settings = io::VizCSettings::instance();
	resetToDefaults();

	setAttribute(Qt::WA_DeleteOnClose);
	//setAutoFillBackground(false);
	//setBackgroundColor(BackGroundColor);
	setMinimumSize(QSize(0,0));
	setMaximumWidth(Width);
	setMinimumWidth(Width);
	setMouseTracking(true);
	
	QHBoxLayout* layoutH = new QHBoxLayout; 

	Dummy = new QWidget;
	Dummy->setAutoFillBackground(false);

	layoutH->addWidget(Dummy);
	layoutH->setMargin(0);
	layoutH->setSpacing(0);

	setLayout(layoutH);
	
	setProperty("name", tr("Eixo Y"));
	//setProperty("selected", false);

	QPalette pal = palette();
	BackgroundColorHighlight1 = pal.color(QPalette::Highlight);
	BackgroundColorHighlight2 = QColor(0,0,0,0);
}
//-----------------------------------------------------------------------------
VizCVerticalScale::~VizCVerticalScale()
{
}
//-----------------------------------------------------------------------------
void VizCVerticalScale::blockCreateUndoCommandPropertyChanged(bool block)
{
	VizCGraphicsPanel* panel = WorkArea->selectedGraphicsPanel();
	Q_ASSERT(panel);
	panel->blockCreateUndoCommandPropertyChanged(block);
}
//-----------------------------------------------------------------------------
void VizCVerticalScale::resetToDefaults()
{

	//GridVisible(false), GridColor(QColor(210,210,210)), GridPenWidth(viz::VEPW_DEFAULT), GridPenStyle(viz::VEPP_DEFAULT)
	//LastTradeVisible(false), Border(1.0), SemiLogarithmicScale(false), InvertScale(false), AutoScale(true), Color(Qt::black), BackGroundColor(Qt::white), 


	GridVisible = false;
	LastTradeVisible = false;
	MaxValue = 1.0;
	MinValue = 0.0;
	Border = 10;
	SemiLogarithmicScale = false;
	InvertScale = false;
	AutoScale = true;


	QString tmp = Settings->value("Appearance/DefaultFontAndColor/Font").toString();
	Font.fromString(tmp);
	Color = QColor(Settings->value("Appearance/DefaultFontAndColor/ForegroundColor").toString());
	BackGroundColor = QColor(Settings->value("Appearance/DefaultFontAndColor/BackgroundColor").toString());
	GridColor = QColor(Settings->value("Appearance/DefaultFontAndColor/GridColor").toString());
	GridPenStyle = static_cast<viz::VizEPenStyle>(Settings->value("Appearance/DefaultFontAndColor/GridStyle").toInt());
	GridPenWidth = static_cast<viz::VizEPenWidth>(Settings->value("Appearance/DefaultFontAndColor/GridWidth").toInt());


	repaint();
	emit scaleChange(true);
}
//-----------------------------------------------------------------------------
bool VizCVerticalScale::isSelected() const
{
	return IsSelected;
}
//-----------------------------------------------------------------------------
void VizCVerticalScale::setSelected(bool selected)
{
	if(IsSelected != selected)
	{
		IsSelected = selected;
		repaint();
	}
}
//-----------------------------------------------------------------------------
QSize VizCVerticalScale::minimumSizeHint() const
{
	return QSize(Width, 0);
}
//-----------------------------------------------------------------------------
QVector<f64> VizCVerticalScale::rulerValues()
{
	return RulerValues;
}
//-----------------------------------------------------------------------------
void VizCVerticalScale::setLastTradeVisible(bool b)
{
	if(LastTradeVisible != b)
	{
		LastTradeVisible = b;
		repaint();
	}
}
//-----------------------------------------------------------------------------
bool VizCVerticalScale::isLastTradeVisible() const
{
	return LastTradeVisible;
}
//-----------------------------------------------------------------------------
void VizCVerticalScale::setLastTrade(f64 value, bool isValid)
{
	LastTradePeriod = value;
	LastTradePeriodIsValid = isValid;
}
//-----------------------------------------------------------------------------
QColor VizCVerticalScale::gridColor() const
{
	return GridColor;
}
//-----------------------------------------------------------------------------
viz::VizEPenWidth VizCVerticalScale::gridPenWidth() const
{
	return GridPenWidth;
}
//-----------------------------------------------------------------------------
viz::VizEPenStyle VizCVerticalScale::gridPenStyle() const
{
	return GridPenStyle;
}
//-----------------------------------------------------------------------------
void VizCVerticalScale::setGridColor(const QColor& color)
{
	if(GridColor != color)
	{
		GridColor = color;
		emit scaleChange(false);
	}
}
//-----------------------------------------------------------------------------
void VizCVerticalScale::setGridPenWidth(viz::VizEPenWidth penWidth)
{
	if(GridPenWidth != penWidth)
	{
		GridPenWidth = penWidth;
		emit scaleChange(false);
	}
}
//-----------------------------------------------------------------------------
void VizCVerticalScale::setGridPenStyle(VizEPenStyle penStyle)
{
	if(GridPenStyle != penStyle)
	{
		GridPenStyle = penStyle;
		scaleChange(false);
	}
}
//-----------------------------------------------------------------------------
void VizCVerticalScale::setRange(f64 min, f64 max)
{
	if(MaxValue != max || MinValue != min)
	{
		MinValue = min;
		MaxValue = max;
		fillRulerValues();
		emit scaleChange(false);
		repaint();

/*		QStringList propertiesNames;
		propertiesNames << "minimum" << "maximum";
		VizCPropertyEditor::instance()->propertiesChanged(this, propertiesNames);
*/
	}
}
//-----------------------------------------------------------------------------
void VizCVerticalScale::setMaximum(f64 value)
{
	if(MaxValue != value)
	{
		MaxValue = value;
		fillRulerValues();
		emit scaleChange(false);
		repaint();

		//VizCPropertyEditor::instance()->propertyChanged(this, "maximum");

	}
}
//-----------------------------------------------------------------------------
void VizCVerticalScale::setMinimum(f64 value)
{
	if(MinValue != value)
	{
		MinValue = value;
		fillRulerValues();
		emit scaleChange(false);
		repaint();

		//VizCPropertyEditor::instance()->propertyChanged(this, "minimum");
	}
}
//-----------------------------------------------------------------------------
f64 VizCVerticalScale::border() const
{
	return Border;
}
//-----------------------------------------------------------------------------
void VizCVerticalScale::setBorder(f64 value)
{
	if(Border != value)
	{
		Border = value;
		fillRulerValues();
		emit scaleChange(false);
		repaint();

		QStringList propertiesNames;
		propertiesNames << "minimum" << "maximum";

		WorkArea->updateProperties(this, propertiesNames);
	}
}
//-----------------------------------------------------------------------------
f64 VizCVerticalScale::maximum() const
{
	//return MaxValue + (Border * 0.01 * (MaxValue - MinValue));
	return MaxValue;
}
//-----------------------------------------------------------------------------
f64 VizCVerticalScale::minimum() const
{
	//return MinValue - (Border * 0.01 * (MaxValue - MinValue));
	return MinValue;
}
//-----------------------------------------------------------------------------
bool VizCVerticalScale::isGridVisible() const
{
	return GridVisible;
}
//-----------------------------------------------------------------------------
void VizCVerticalScale::setGridVisible(bool b)
{
	if(GridVisible != b)
	{
		GridVisible = b;
		emit scaleChange(false);
	}
}
//-----------------------------------------------------------------------------
void VizCVerticalScale::setSemiLogarithmicScale(bool b)
{
	if(SemiLogarithmicScale != b)
	{
		SemiLogarithmicScale = b;
		fillRulerValues();
		emit scaleChange(true);
		repaint();
	}
}
//-----------------------------------------------------------------------------
bool VizCVerticalScale::isSemiLogarithmicScale() const
{
	return SemiLogarithmicScale;
}
//-----------------------------------------------------------------------------
void VizCVerticalScale::setInvertScale(bool b)
{
	if(InvertScale != b)
	{
		InvertScale = b;		
		emit scaleChange(false);
		repaint();
	}
}
//-----------------------------------------------------------------------------
bool VizCVerticalScale::isInvertScale() const
{
	return InvertScale;
}
//-----------------------------------------------------------------------------
bool VizCVerticalScale::isAutoScale() const
{
	return AutoScale;
}
//-----------------------------------------------------------------------------
void VizCVerticalScale::setAutoScale(bool b)
{
	if(AutoScale != b)
	{
		AutoScale = b;
		fillRulerValues();
		emit scaleChange(true);
		repaint();
	}
}
//-----------------------------------------------------------------------------
QColor VizCVerticalScale::color() const
{
	return Color;
}
//-----------------------------------------------------------------------------
QColor VizCVerticalScale::backGroundColor() const
{
	return BackGroundColor;
}
//-----------------------------------------------------------------------------
void VizCVerticalScale::setColor(const QColor& color)
{
	if(Color != color)
	{
		Color = color;
		repaint();
	}
}
//-----------------------------------------------------------------------------
void VizCVerticalScale::setBackgroundColor(const QColor& color)
{
	if(BackGroundColor != color)
	{
		BackGroundColor = color;
		repaint();
	}
}
//-----------------------------------------------------------------------------
void VizCVerticalScale::setFont(const QFont& f)
{
	if(Font != f)
	{
		Font = f;
		fillRulerValues();
	}
}
//-----------------------------------------------------------------------------
QFont VizCVerticalScale::font() const
{
	return Font;
}
//-----------------------------------------------------------------------------
void VizCVerticalScale::mousePressEvent(QMouseEvent* event)
{
	if(VizCToolManager::instance()->isToolSelected(VETS_SELECTION))
	{
		WorkArea->setSelectedVizOBject(this);

		VizCGraphicsPanel* ip = qobject_cast<VizCGraphicsPanel*>(parentWidget());
		if(ip)
		{
			WorkArea->setSelectedGraphicsPanel(ip);
			WorkArea->updateScene();
		}
	}
}
//-----------------------------------------------------------------------------
int VizCVerticalScale::propertyGetGridPenWidth() const
{
	return static_cast<int>(gridPenWidth() - 1);
}
//-----------------------------------------------------------------------------
void VizCVerticalScale::propertySetGridPenWidth(int value)
{
	setGridPenWidth(static_cast<viz::VizEPenWidth>(value + 1));
}
//-----------------------------------------------------------------------------
int VizCVerticalScale::propertyGetGridPenStyle() const
{
	return static_cast<viz::VizEPenStyle>(gridPenStyle());
}
//-----------------------------------------------------------------------------
void VizCVerticalScale::propertySetGridPenStyle(int value)
{
	setGridPenStyle(static_cast<viz::VizEPenStyle>(value));
}
//-----------------------------------------------------------------------------
s32 VizCVerticalScale::pixelsWide() const
{
	return Dummy->geometry().width();	
}
//-----------------------------------------------------------------------------
s32 VizCVerticalScale::pixelsHigh() const
{
	return Dummy->geometry().height();	
}
//-----------------------------------------------------------------------------
s32 VizCVerticalScale::ceilDigit(const s32 n) const
{
	if (n == 0)
		return 0;

	if (n > 5)
		return 10;

	if (n <= 2)
		return n;

	if (n <= 5)
		return 5;	

	return 0; //error (n > 10)
}
//-----------------------------------------------------------------------------
f64 VizCVerticalScale::ceilValue(const f64 n) const
{
	bool isNegative = (n < 0.0) ? true : false;
	f64 absN = (isNegative) ? -n : n; 

	bool bigLattice  = (absN > 1.0) ? true : false;

	QString strLatticeDelta  = QString::number((bigLattice) ? absN 
		: absN * MinimumInFraction , 'f', 0);		

	s32 roundHundreth(0);
	if (strLatticeDelta.size() > 1)
		roundHundreth = (strLatticeDelta.at(1) > 5 ? 1 : 0);

	s32 intervalDecider = ceilDigit(static_cast<s32>
		(strLatticeDelta.at(0).digitValue() + roundHundreth));

	if (intervalDecider == 0)
		return 0.0; //error
	else 
		if (intervalDecider < 10)
		{		
			strLatticeDelta.replace(0, 1, QString::number(intervalDecider));
		}
		else 
			if (intervalDecider == 10)
			{
				strLatticeDelta.replace(0, 1, QChar('0'));
				strLatticeDelta.push_front(QChar('1'));				
			} 	

			for (s32 i = 1; i < strLatticeDelta.size(); i++)
				strLatticeDelta.replace(i, 1, QChar('0'));

			f64 result = (isNegative)
				? ((bigLattice) ? (strLatticeDelta.toDouble() * -1.0)
				: (strLatticeDelta.toDouble() * -1.0) / MinimumInFraction) :
			((bigLattice) ? strLatticeDelta.toDouble()
				: strLatticeDelta.toDouble() / MinimumInFraction);

	 
	 return result;
}
//-----------------------------------------------------------------------------
f64 VizCVerticalScale::latticeFirstValue(const f64 n, const f64 lattice) const
{
	f64 remainder = (n / lattice) - floor(n / lattice);
	f64 toTheInterval = 1 - remainder;
	return n + (toTheInterval * lattice);
}
//-----------------------------------------------------------------------------
s32 VizCVerticalScale::decimalPlaces(f64 x1, f64 x2)
{
	s32 precision(0);
	
	if (RulerValues.size() > 1)
	{	
		f64 lattice = abs(x1 - x2);

		lattice = (lattice > std::numeric_limits<float>::epsilon()) ? lattice : 2.0;

		precision = (lattice  > (1.0 - std::numeric_limits<float>::epsilon())) ? 0 : MaximumFractionNumbers;		

		if (precision == MaximumFractionNumbers)
		{
			QString sLattice = QString::number(floor(lattice * MinimumInFraction + 0.5) );

			int CountZeroes(MaximumFractionNumbers);
			
			for (int i = (sLattice.size() - 1); i  >= 0; i--)
			{
				if (sLattice.at(i) == '0')
					CountZeroes--;
				else
					break;
			}

			precision = CountZeroes;
		}
	}
	else
	{
		if (RulerValues.size() > 0)
			precision = (x2 > (1.0 - std::numeric_limits<float>::epsilon())) ? 0 : MaximumFractionNumbers;		
	}

	return precision;
}

//-----------------------------------------------------------------------------
// Essa rotina desenha todos elementos da regua
// 1) valores
// 2) last trade
// 3) caixa de "vezes quanto" que indica digitos que nao couberam na barra
void VizCVerticalScale::drawRuler()
{
	QPainter painter(this);
	painter.setFont(Font);
	painter.setPen(Color);
	
	s32 sizeX = pixelsWide();
	s32 sizeY = pixelsHigh();

	f64 delta = maximum() - minimum();
	f64 lDelta = logDelta();

	QRect viewport(0, 0, sizeX, sizeY);
	QRect window(0, 0, sizeX, sizeY);

	painter.setViewport(viewport);
	painter.setWindow(window);
	
	painter.fillRect(QRect(0, 0, sizeX, sizeY), BackGroundColor);

	painter.setPen(Color);
	painter.drawLine(QPointF(0, 0), QPointF(0, sizeY));	

	QFontMetrics fm(font());
	int pxFontHeight = fm.height();

	f64 fpxlDistance = static_cast<f64>(sizeY); 
	
	s32 sDrawPosition;
	QString strValue;

	s32 semiLogJump(-1);	
	s32 precisionIndex(0);
	if (isSemiLogarithmicScale())
		semiLogJump = (RulerValues.size() > 0) ? static_cast<s32>(floor(logMax())) : 0;	

	// desenha regua
	for (s32 i = 0; i < RulerValues.size(); i++)
	{
		f64 drawPosition =  (isSemiLogarithmicScale()) ? ((fpxlDistance * (core::clampLog10f(RulerValues.at(i)) - (logMin()))) / lDelta) 
			: ((fpxlDistance * (RulerValues.at(i) - (minimum()))) / delta);

		if (isSemiLogarithmicScale())
		{
			s32 newPrecisionIndex = (RulerValues.size() > 0) ? static_cast<s32>(floor(core::clampLog10f(RulerValues[i]))) : 0;	

			if (semiLogJump > newPrecisionIndex)
			{
				if (precisionIndex < Precision.size() - 1)
					precisionIndex++;
				semiLogJump = newPrecisionIndex;
			}

			if (ClampedDigits > 0)
			{
				s32 integerNumbers = QString::number(RulerValues.at(i), 'f', 0).size();

				f64 divVal = RulerValues.at(i) / core::pow10f(ClampedDigits);

				s32 divisionProblemFix = (ClampedDigits - integerNumbers + 1) > 0 ? (ClampedDigits - integerNumbers) + 1 : 0 ;
				strValue = QString::number(divVal, 'f', Precision.at(precisionIndex) + divisionProblemFix);

			}
			else
				strValue = QString::number(RulerValues.at(i), 'f', Precision.at(precisionIndex));
		}
		else
		{

			if (ClampedDigits > 0)
				strValue = QString::number(RulerValues.at(i) / core::pow10f(ClampedDigits), 'f', Precision.at(0));
			else
				strValue = QString::number(RulerValues.at(i), 'f', Precision.at(0));
		}

		if(!InvertScale)
			sDrawPosition = static_cast<s32>(floor(fpxlDistance - drawPosition +.5));			
		else
			sDrawPosition = static_cast<s32>(floor(drawPosition +.5));			
	
		painter.drawLine(QPoint(RulerMarkPxlSize, sDrawPosition), QPoint(0, sDrawPosition));			
		sDrawPosition += (pxFontHeight / 4);	
		painter.drawText(QPoint(RulerMarkPxlSize + 2, sDrawPosition), strValue);	
	}
	
	// desenha last trade
	if (LastTradeVisible && LastTradePeriodIsValid)
	{
		f64 drawPosition =  (isSemiLogarithmicScale()) ? ((fpxlDistance * (core::clampLog10f(LastTradePeriod) - (logMin()))) / lDelta) 
			: ((fpxlDistance * (LastTradePeriod - (minimum()))) / delta);
		
		strValue = QString::number(LastTradePeriod, 'f');

		if(!InvertScale)
			sDrawPosition = static_cast<s32>(floor(fpxlDistance - drawPosition +.5));			
		else
			sDrawPosition = static_cast<s32>(floor(drawPosition +.5));

 		sDrawPosition += (pxFontHeight / 4);
		QRect bound(fm.boundingRect(strValue));
		bound.translate(QPoint(0, sDrawPosition));
		QBrush br(Color);
		bound.setWidth(Dummy->width());
		
		painter.fillRect(bound, br);
		
		QPen p(backGroundColor());
		
		painter.setPen(p);

		painter.drawText(QPoint(RulerMarkPxlSize, sDrawPosition), strValue);		
	}
	
	// desenha x 100 x 1000  etc ou 10^blabla
	if (ClampedDigits > 0)
	{
		QPoint drawClampedPos = QPoint(2, pixelsHigh() - (RulerMarkPxlSize + 1));
		
		QFont defaultFont;
		defaultFont.fromString(Settings->value("DefaultFont", QApplication::font()).toString());
				
		QFontMetrics dfm(defaultFont);

		painter.setFont(defaultFont);		

		QString zeroesString = QString("%1x").arg(QString::number(static_cast<long>(core::pow10f( static_cast<f64>(ClampedDigits)))));

		QString clampedDigitText;
		if (dfm.boundingRect(zeroesString).width() > (Dummy->width() - 4))
			clampedDigitText = QString(" 10<b><sup>%1</sup></b>").arg(ClampedDigits);
		else 
			clampedDigitText = zeroesString;

		clampedDigitText = "<span>" + clampedDigitText +"</span>";

		
		QRect bound(dfm.boundingRect(clampedDigitText));
		QTextDocument td;
		td.setDefaultStyleSheet(QString("span {color: %1;}").arg(Color.name()));
		td.setHtml(clampedDigitText);
		
		bound.translate(drawClampedPos);

		bound.setWidth(Dummy->width() - 4);
		painter.fillRect(bound, BackGroundColor);
		
		painter.setPen(Color);
		painter.drawRect(bound);
		
		painter.save();	
		// Esse 2 é do bounding box
		painter.translate(0, bound.y() - 2);		
		td.drawContents(&painter);
		painter.restore();	
		
		
	}

	if(IsSelected)
	{
		QPalette pal = parentWidget()->palette();
		QColor titleBackgroundColor1 = BackgroundColorHighlight1;
		QColor titleBackgroundColor2 = BackgroundColorHighlight2;
		titleBackgroundColor1.setAlphaF(0.5);
		QLinearGradient gradient(0, 0, Dummy->geometry().width(), 0);
		gradient.setColorAt(0.0, titleBackgroundColor1);
		gradient.setColorAt(1.0, titleBackgroundColor2);
		painter.setBrush(gradient);
		painter.setPen(Qt::NoPen);
		painter.fillRect(QRect(0, 0, Dummy->geometry().width(), Dummy->geometry().height()), gradient);
	}
}
//-----------------------------------------------------------------------------
f64 VizCVerticalScale::logMax() const
{
	return core::clampLog10f(maximum());
}
//-----------------------------------------------------------------------------
f64 VizCVerticalScale::logMin() const
{
	return core::clampLog10f(minimum());
}
//-----------------------------------------------------------------------------
f64 VizCVerticalScale::logDelta() const
{
	return logMax() - logMin();
}
//-----------------------------------------------------------------------------
s32 VizCVerticalScale::verticalTiles(const f64 height) const
{
	QFontMetrics fm(font());
	s32 pxFontHeight = fm.height();			
	return (height / (pxFontHeight + PxlThreshold));
}
//-----------------------------------------------------------------------------
f64 VizCVerticalScale::semiLogLattice(const f64 logTop, const f64 logBottom) const
{
//	if ((logBottom < logMin()) || (logTop > logMax()))
//		return std::numeric_limits<f64>::max();

	QFontMetrics fm(font());
	f64 pxFontHeight = static_cast<f64>(fm.height());

	f64 pxlsSubHeight = static_cast<f64>(((logTop - logBottom) * pixelsHigh()) / logDelta());
	//f64 pxlsSubHeight = static_cast<f64>(((core::pow10f(logTop) - core::pow10f(logBottom)) * pixelsHigh()) / core::pow10f(logDelta()));

	int numbOfTiles = verticalTiles(pxlsSubHeight);

	if (numbOfTiles > 2)
	{
		//f64 logBelow = ((pxlsSubHeight - (5.0 * pxFontHeight )) * (logTop - logBottom)) / pxlsSubHeight;
		//f64 logBelow = ((pxlsSubHeight - (2.6 * pxFontHeight )) * (logTop - logBottom)) / pxlsSubHeight;
		f64 logBelow = ((pxlsSubHeight - (pxFontHeight + (PxlThreshold + 5))) * (logTop - logBottom)) / pxlsSubHeight;

		logBelow += logBottom;
		
		return core::pow10f(logTop) - core::pow10f(((logTop + logBelow) / 2.0));
		//return core::pow10f(logTop);
	} 
	else if (numbOfTiles > 0)
	{
		return core::pow10f(logTop) - core::pow10f(((logTop + logBottom) / 2.0));
	}

	return std::numeric_limits<f64>::max();

}
//-----------------------------------------------------------------------------
void VizCVerticalScale::fillLinearInterval(const s32 pxLatticeTiles, const f64 top, const f64 bottom)
{
	if (pxLatticeTiles > 1)
	{
		f64 minValueRound;
		f64 delta = top - bottom;
		f64 pxLT  = static_cast<f64>(pxLatticeTiles);
		f64 latticeDelta = static_cast<f64>(delta / ((pxLT > 0.0) ? pxLT : 1.0));
		f64 latticeDeltaRound = ceilValue(latticeDelta);	

		minValueRound = latticeFirstValue(bottom, latticeDeltaRound);		
				
		s32 countLimit(2000);
		while ((minValueRound < top) && (countLimit-- > 0))
		{
			RulerValues << minValueRound;

			minValueRound += latticeDeltaRound;			
		}
	}
	else if (pxLatticeTiles >= 0)
	{
		f64 delta = top - bottom;		
		
		f64 centerValueRound = ceilValue(delta / 2.0) + ceilValue(bottom);
		
		RulerValues << centerValueRound;							
	}
}

//-----------------------------------------------------------------------------
void VizCVerticalScale::fillSemiLogInterval(const f64 lattice, const f64 top, const f64 bottom)
{
	f64 latticeDeltaRound = ceilValue(lattice);	

	f64 minValueRound =  latticeFirstValue(bottom, latticeDeltaRound);
			
	s32 countLimit(2000);
	while ((minValueRound < top) && (countLimit-- > 0))
	{
		RulerValues << minValueRound;

		minValueRound += latticeDeltaRound;					
	}
}
//-----------------------------------------------------------------------------
void VizCVerticalScale::clampLargeNumbers()
{
	QFontMetrics fm(font());
	s32 widest = 0;
	s32 maxWidth   = 0;
	s32 digitsSize = 0;

	for (int i = 0; i < 10; i++)
	{
		QString strValue = QString::number(i);
		if (widest < fm.width(strValue))
			widest = fm.width(strValue);
	}

	//error situation
	if (widest < 0)
		return;

	//if ((RulerValues.size() > 0) || (Precision.at(0) < 1))
	if (RulerValues.size() > 0)
	{
		if (Precision.at(0) < 1)
		{
			for (int i = 0; i < RulerValues.size(); i++)
			{
				//QString strValue = QString::number(RulerValues.at(i), 'f', Precision);
				QString strValue = QString::number(RulerValues.at(i), 'f', 0);

				digitsSize = fm.width(strValue);
				
				if (maxWidth < digitsSize)
					maxWidth = digitsSize;
				
			}
			if (pixelsWide() != RulerMarkPxlSize)
			{	
				ClampedDigits = 0;
				f64 fmaxWidth = static_cast<f64>(maxWidth);
				const f64 fPxlWidth = static_cast<f64>(pixelsWide() - RulerMarkPxlSize);
				s32 doesNotFit = 0;
				while (floor(fmaxWidth / fPxlWidth) > 0.0001f)
				{
					ClampedDigits++; 				
					fmaxWidth -= static_cast<f64>(widest);
				}
			}
		}
		else
		{
			ClampedDigits = 0;
		}		
	}
}
//-----------------------------------------------------------------------------
// A divisao da regua eh constante, ie, elementos possuem distancias regulares.
void VizCVerticalScale::fillLinearValues()
{
	// lattice representa a subdivisao fixa entre os elementos na regua	
	int pxLatticeTiles = verticalTiles(pixelsHigh());

	fillLinearInterval(pxLatticeTiles, maximum(), minimum());		

	f64 basePrecisionX1(0.0), basePrecisionX2(0.0);
	if (RulerValues.size() > 1)
	{
		basePrecisionX1 = RulerValues[1];
		basePrecisionX2 = RulerValues[0];			
	}
	else if (RulerValues.size() == 1)
	{
		basePrecisionX1 = RulerValues[0];
		basePrecisionX2 = RulerValues[0];
	}

	Precision.append(decimalPlaces(basePrecisionX1, basePrecisionX2));	
}

//-----------------------------------------------------------------------------
// A divisao eh feita em duas partes, 
//	(1) a divisao dos elementos multiplos de 10, e
//	(2) seus intermediarios
void VizCVerticalScale::fillSemiLogValues()
{
	// lattice representa a subdivisao fixa entre os elementos na regua	
	int pxLatticeTiles = verticalTiles(pixelsHigh());

	s32 logarithmicDistance = static_cast<s32>(floor(logMax()) - ceil(logMin()));

	// isso aqui eh necessario mostrar uma distancia logaritmica grande
	// em pouco espaco, por isso tem de ser feitos 'pulos' atraves de logs
	if (logarithmicDistance >= pxLatticeTiles)
	{
		if (pxLatticeTiles > 0)
		{
			s32 jumpThroughLog = floor(static_cast<f64>(logarithmicDistance / pxLatticeTiles));
			for (int i = 0; i < logarithmicDistance; i+=jumpThroughLog)
			{
				f64 nextValue = core::pow10f((floor(logMax()) - i));   
				RulerValues << nextValue;           
			}
		} 
		else
		{
			RulerValues << core::pow10f(floor(logMax()));			
		}

		Precision.append(0);
	}
	else
	{
		// vai de distancia em distancia logaritmica 
		// preenchedo a parte inicial
 		f64 logLattice = semiLogLattice(logMax(), floor(logMax()));	

		fillSemiLogInterval(logLattice, maximum(), core::pow10f( floor(logMax())));

		if (RulerValues.size() > 1)
			Precision.append(decimalPlaces(RulerValues[RulerValues.size() - 1], RulerValues[RulerValues.size() - 2]));
		else
			Precision.append(0);

		// aqui preenche a parte intermediaria
		if ((floor(logMax()) - ceil(logMin())) > 0.0)
		{
			for (int i = 0; i < logarithmicDistance ; i++)
			{
				f64 nextValue = core::pow10f( floor(logMax()) - i);   
				RulerValues << nextValue;

				logLattice = semiLogLattice(floor(logMax()) - i, 
					floor(logMax()) - i - 1);

				fillSemiLogInterval(logLattice, core::pow10f( floor(logMax()) - i), 
					core::pow10f( floor(logMax()) - i - 1));

				if (RulerValues.size() > 1)
					Precision.append(decimalPlaces(RulerValues[RulerValues.size() - 1], RulerValues[RulerValues.size() - 2]));
				else
					Precision.append(0);

			}			
		}	

		f64 nextValue = core::pow10f(ceil(logMin()));   
		RulerValues << nextValue;
		//Precision.append(0);
// 		else 
// 		{
// 			f64 nextValue = core::pow10f( floor(logMax()));   
// 			RulerValues << nextValue;
// 			Precision.append(0);
// 		}
// 		

		// aqui preenche a parte final 
		if (floor(logMax()) == floor(logMin()))
			return;

		logLattice = semiLogLattice(ceil(logMin()), logMin());
		fillSemiLogInterval(logLattice, core::pow10f( ceil(logMin())), minimum());

		if (RulerValues.size() == 0)
			RulerValues << core::pow10f( floor(logMax()));

		if (RulerValues.size() > 1)
			Precision.append(decimalPlaces(RulerValues[RulerValues.size() - 1], RulerValues[RulerValues.size() - 2]));
		else
			Precision.append(0);
	}       		

}
//-----------------------------------------------------------------------------
// Essa rotina calcula os valores que encontrados na regua vertical.
// Os valores são incluidos na lista RulerValues
void VizCVerticalScale::fillRulerValues()
{
	Precision.clear();
	RulerValues.clear();		
	
	if (!isSemiLogarithmicScale())
	{
		fillLinearValues();		
	}	
	else
	{
		fillSemiLogValues();
	}	
	
	// ve se a fonte cabe na horizontal e rearranja os valores se necessario
	clampLargeNumbers();
}
//-----------------------------------------------------------------------------
void VizCVerticalScale::resizeEvent(QResizeEvent* event)
{
	if(event->oldSize().height() != event->size().height())
	{
		fillRulerValues();
	}
}
//-----------------------------------------------------------------------------
void VizCVerticalScale::paintEvent(QPaintEvent* event)
{	
	drawRuler();
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------


