//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCHORIZONTALSCALE_H__
#define __VIZCHORIZONTALSCALE_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include "VizCTradePeriod.h"
#include "VizCSettings.h"
#include <QLocale>
#include <QVector>
#include <QDateTime>
#include <QResizeEvent>
#include <QScrollBar>
#include <QWidget>
#include <QMenu>
#include <QGridLayout>
#include <QToolButton>
#include <QAction>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCSecurityWidget;
class VizCWorkArea;
//-----------------------------------------------------------------------------
class VizCHorizontalScale : public QWidget
{
	Q_OBJECT
	
	Q_PROPERTY(QColor color READ color WRITE setColor USER true)
	Q_PROPERTY(QColor backGroundColor READ backGroundColor WRITE setBackgroundColor USER true)
	Q_PROPERTY(QFont font READ font WRITE setFont USER true)
	Q_PROPERTY(bool gridVisible READ isGridVisible WRITE setGridVisible USER true)
	Q_PROPERTY(QColor gridColor READ gridColor WRITE setGridColor USER true)
	Q_PROPERTY(int gridPenWidth READ propertyGetGridPenWidth WRITE propertySetGridPenWidth USER true)
	Q_PROPERTY(int gridPenStyle READ propertyGetPenStyle WRITE propertySetPenStyle USER true)
// 	Q_PROPERTY(QDateTime startDate READ startDate WRITE setStartDate USER true)
// 	Q_PROPERTY(QDateTime endDate READ endDate WRITE setEndDate USER true)
	Q_PROPERTY(int rightMargin READ rightMargin WRITE setRightMargin USER true)
	Q_PROPERTY(int leftMargin READ leftMargin WRITE setLeftMargin USER true)
	Q_PROPERTY(int periodicity READ propertyGetPeriodicity WRITE propertySetPeriodicity USER true)
	Q_PROPERTY(bool selected READ isSelected WRITE setSelected)

public:
	VizCHorizontalScale(VizCSecurityWidget* parent, VizCWorkArea* workArea);
	~VizCHorizontalScale();

	//pra bloquear o sinal para as animacoes
	Q_INVOKABLE void blockCreateUndoCommandPropertyChanged(bool block);
	Q_INVOKABLE void resetToDefaults();

	bool isSelected() const;
	void setSelected(bool selected);
	bool isGridVisible() const;
	QColor color() const;
	QColor backGroundColor() const;
	s32 rightMargin() const;
	s32 leftMargin() const;
	QDateTime startData() const;
	QDateTime endData() const;
	viz::VizESecurityPeriodicity periodicity() const;
	const QVector<QDateTime>& rulerValues() const;
	bool findRuleValueIndex(s32& index, const QDateTime& dateTime, bool findNextIndex = false);
	QFont font() const;
	QColor gridColor() const;
	viz::VizEPenWidth gridPenWidth() const;
	viz::VizEPenStyle gridPenStyle() const;

	void currentVisibleRulerValuesIndex(s32& start, s32& end) const;
	s32 beginVisibleRulerValuesIndex() const;
	s32 endVisibleRulerValuesIndex() const;
	QList<s32> visibleRulerValuesIndex() const;
	void pan(s32 delta);
	void zoomIn();
	void zoomOut();
	void resetZoom();
	void zoomBox(s32 startRulerValueIndex, s32 endRulerValueIndex);
	void setGridVisible(bool b);
	void setColor(const QColor& color);
	void setBackgroundColor(const QColor& color);
	void setRightMargin(s32 rightmargin);
	void setLeftMargin(s32 leftmargin);
	void setStartData(const QDateTime& date);
	void setEndData(const QDateTime& date);
	void setPeriodicity(viz::VizESecurityPeriodicity periodicity);
	void setRulerValues(const QVector<QDateTime>& values);
	void setPeriodsToPreviewChart(const QVector<core::VizCTradePeriod> periods);
	void setFont(const QFont& font);
	void setGridColor(const QColor& color);
	void setGridPenWidth(viz::VizEPenWidth penWidth);
	void setGridPenStyle(VizEPenStyle penStyle);
	QVector<core::VizCTradePeriod> ajustPeriods(const QVector<core::VizCTradePeriod>& periods);
	
protected:
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void resizeEvent(QResizeEvent* event);
	virtual void paintEvent(QPaintEvent* event);

signals:
	void horizontalScaleChange(bool rulerValuesIndexChange, bool rulerValuesChange, bool periodicityChange, bool marginChange);
	void createUndoCommandPropertyChanged(QObject* item, const QString& propertyName, const QVariant& oldValue, const QVariant& newValue, bool needUpdateVerticalScale);
	void createUndoCommandPropertiesChanged(QObject* item, const QStringList& propertiesNames, const QVariantList& oldValues, const QVariantList& newValues, bool needUpdateVerticalScale, const QString& commandAliasName);


private:
	bool IsSelected;
	VizCSecurityWidget* Parent;
	viz::VizESecurityPeriodicity Periodicity;
	QVector<QDateTime> RulerValues;	
	QMap<s32, s32> RulValToDateMap;
	QList<s32> MinimumPxlsPerFont;
	QList<s32> VisibleRulerValuesIndices;
	QColor BackgroundColorHighlight1;
	QColor BackgroundColorHighlight2;
	QColor GridColor;
	viz::VizEPenWidth GridPenWidth;
	viz::VizEPenStyle GridPenStyle;
	QFont Font;
	s32 RightMargin;
	s32 LeftMargin;
	QDateTime StartDate;
	QDateTime EndDate;
	f64 SliderStart;
	f64 SliderEnd;
	int SliderMoveState;
	f64 SliderTranslate;
	QVector<core::VizCTradePeriod> Periods;
	f64 MaxY;
	f64 MinY;
	s32 LowerCategory;
	bool GridVisible;
	QColor Color;
	QColor BackGroundColor; 
	QWidget* Dummy;
	QMenu* PeriodicityMenu;
	QToolButton* PeriodicityButton;
	s32 PeriodicityPixmapIndex;
	QPixmap PeriodicityPixmaps[5];
	QAction* PeriodicityActions[5];
	io::VizCSettings* Settings;
	VizCWorkArea* WorkArea;

	inline bool hasRulerValues();
	void setCurrentPeriodicityPixmap();
	void createChartToolBar(QGridLayout* layout);
	void createPeriodicityMenu();

	QRect rulerValuesViewPort();
	QRect slidersViewPort();
	void computeSlidersRects(QRectF& sliderStart, QRectF& sliderEnd, QRectF& sliderTranslate);
	
	void drawHorizontalValues(QPainter* painter);
	void drawSliders(QPainter* painter);
	void drawChart(QPainter* painter);

	QString mapToString(QDateTime d, s32 hierarquy);
	void fillPxlsMinimumPerFont();
	void fillVisibleRulerValues();
	void calculeRulValToDateMap();	
	
	QDate firstDate();
	QDate lastDate();

	s32 availablePxlsHorizontal() const;	
	f64 pxlsPerRulerValue() const;
	s32 currentDate(s32 indexStart, QDateTime current);	
	s32 lowerCategory(s32 indexStart);
	s32 selectIntraPeriodCategory(s32 pxlsAvailable, s32 indexStart);
	void fillRulValToDateMapLoop();
	void fillInterval(f64 fpxlDistance, s32 minPixelsIndex, s32 drawIndex, QVector<QDateTime>::const_iterator iFirstDate, QVector<QDateTime>::const_iterator iLastDate);
	void changeIntraPeriodRulValToDateMap();

private slots:
	void buttonGroupClicked(int id);
	void slotSetPeriodicityDayly();
	void slotSetPeriodicityWeekly();
	void slotSetPeriodicityMonthy();
	void slotSetPeriodicityQuarterly();
	void slotSetPeriodicityYearly();

	int propertyGetPeriodicity() const;
	void propertySetPeriodicity(int value);
	int propertyGetGridPenWidth() const;
	void propertySetGridPenWidth(int value);
	int propertyGetPenStyle() const;
	void propertySetPenStyle(int value);
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
