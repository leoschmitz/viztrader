//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCVERTICALSCALE_H__
#define __VIZCVERTICALSCALE_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include "VizCSettings.h"
#include <QResizeEvent>
#include <QColor>
#include <QFont>
#include <QWidget>
#include <QVector>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCGraphicsPanel;
class VizCWorkArea;
//-----------------------------------------------------------------------------
class VizCVerticalScale : public QWidget
{
	Q_OBJECT

	Q_PROPERTY(bool semiLogarithmicScale READ isSemiLogarithmicScale WRITE setSemiLogarithmicScale USER true)
	Q_PROPERTY(bool invertScale READ isInvertScale WRITE setInvertScale USER true)
	Q_PROPERTY(bool autoScale READ isAutoScale WRITE setAutoScale USER true)
	Q_PROPERTY(bool lastTradeVisible READ isLastTradeVisible WRITE setLastTradeVisible USER true)
	Q_PROPERTY(double maximum READ maximum WRITE setMaximum USER true)
	Q_PROPERTY(double minimum READ minimum WRITE setMinimum USER true)
	Q_PROPERTY(double border READ border WRITE setBorder USER true)
	Q_PROPERTY(QColor color READ color WRITE setColor USER true)
	Q_PROPERTY(QColor backGroundColor READ backGroundColor WRITE setBackgroundColor USER true)
	Q_PROPERTY(QFont font READ font WRITE setFont USER true)
	Q_PROPERTY(bool gridVisible READ isGridVisible WRITE setGridVisible USER true)
	Q_PROPERTY(QColor gridColor READ gridColor WRITE setGridColor USER true)
	Q_PROPERTY(int gridPenWidth READ propertyGetGridPenWidth WRITE propertySetGridPenWidth USER true)
	Q_PROPERTY(int gridPenStyle READ propertyGetGridPenStyle WRITE propertySetGridPenStyle USER true)
	Q_PROPERTY(bool selected READ isSelected WRITE setSelected)

public:
	VizCVerticalScale(VizCGraphicsPanel* parent, VizCWorkArea* workArea);
	~VizCVerticalScale();

	//pra bloquear o sinal para as animacoes
	Q_INVOKABLE void blockCreateUndoCommandPropertyChanged(bool block);

	Q_INVOKABLE void resetToDefaults();

	bool isSelected() const;
	void setSelected(bool selected);
	QSize minimumSizeHint() const;
	QVector<f64> rulerValues();
	f64 maximum() const;
	f64 minimum() const;
	f64 border() const;
	bool isGridVisible() const;
	bool isSemiLogarithmicScale() const;
	bool isInvertScale() const;
	bool isAutoScale() const;
	bool isLastTradeVisible() const;
	QColor color() const;
	QColor backGroundColor() const;
	QFont font() const;
	QColor gridColor() const;
	viz::VizEPenWidth gridPenWidth() const;
	viz::VizEPenStyle gridPenStyle() const;
	void setRange(f64 min, f64 max);
	void setMaximum(f64 value);
	void setMinimum(f64 value);
	void setBorder(f64 value);
	void setGridVisible(bool b);
	void setSemiLogarithmicScale(bool b);
	void setInvertScale(bool b);
	void setAutoScale(bool b);
	void setColor(const QColor& color);
	void setBackgroundColor(const QColor& color);
	void setFont(const QFont& font);
	void setGridColor(const QColor& color);
	void setGridPenWidth(viz::VizEPenWidth penWidth);
	void setGridPenStyle(VizEPenStyle penStyle);
	void setLastTradeVisible(bool b);
	void setLastTrade(f64 value, bool isValid);

protected:
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void resizeEvent(QResizeEvent* event);
	virtual void paintEvent(QPaintEvent* event);

signals:
	void scaleChange(bool verticalScaleChange);
	void createUndoCommandPropertyChanged(QObject* obj, const QString& propertyName, const QVariant& oldValue, const QVariant& newValue, bool needUpdateVerticalScale);
	void createUndoCommandPropertiesChanged(QObject* obj, const QStringList& propertiesNames, const QVariantList& oldValues, const QVariantList& newValues, bool needUpdateVerticalScale, const QString& commandAliasName);

private:
	int propertyGetGridPenWidth() const;
	void propertySetGridPenWidth(int value);
	int propertyGetGridPenStyle() const;
	void propertySetGridPenStyle(int value);

	void fillRulerValues();
	void fillLinearValues();
	void fillSemiLogValues();

	void fillLinearInterval(const s32 pxLatticeTiles, const f64 top, const f64 bottom);	
	void fillSemiLogInterval(const f64 lattice, const f64 top, const f64 bottom);	

	void drawRuler();

	s32 pixelsWide() const;		
	s32 pixelsHigh() const;	
	s32 ceilDigit(const s32 n) const;
	f64 ceilValue(const f64 n) const;	
	void clampLargeNumbers();
	s32 verticalTiles(const f64 height) const;
	f64 semiLogLattice(const f64 logTop, const f64 logBottom) const;
	f64 latticeFirstValue(const f64 n, const f64 lattice) const;	
	s32 decimalPlaces(f64 x1, f64 x2);
	
	f64 logDelta() const;
	f64 logMax() const;
	f64 logMin() const;
	
	bool IsSelected;
	QVector<f64> RulerValues;
	QFont Font;
	f64 MinValue;
	f64 MaxValue;
	f64 Border;
	QVector<s32> Precision;
	s32 ClampedDigits;
	bool GridVisible;
	bool SemiLogarithmicScale;
	bool InvertScale;
	bool AutoScale;
	QColor Color;
	QColor BackGroundColor;
	QColor BackgroundColorHighlight1;
	QColor BackgroundColorHighlight2;
	QWidget* Dummy;
	f64 LastTradePeriod;
	bool LastTradePeriodIsValid;
	bool LastTradeVisible;
	QColor GridColor;
	viz::VizEPenWidth GridPenWidth;
	viz::VizEPenStyle GridPenStyle;
	io::VizCSettings* Settings;
	VizCWorkArea* WorkArea;

};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
