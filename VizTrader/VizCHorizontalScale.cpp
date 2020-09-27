//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizMath.h"
#include "VizCHorizontalScale.h"
#include "VizCPropertyEditor.h"
#include "VizCSecurityWidget.h"
#include "VizCToolManager.h"
#include "VizCSceneInspector.h"
#include "VizGUIUtils.h"
#include <QPainter>
#include <QButtonGroup>
#include <QDebug>
#include <QApplication>
#include <QPainterPath>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
enum VizEButtonIndex
{
	VEBI_ZoomPeriodicityButton = 0,
	VEBI_ZoomResetButton = 1,
	VEBI_ZoomOutButton = 2,
	VEBI_ZoomInButton = 3,
};
//-----------------------------------------------------------------------------
const s32 DailyStart   = 0;
const s32 MonthlyStart = 3;
const s32 YearlyStart  = 6;
const s32 WithoutString = 100;
const s32 PxlThreshold = 9;
const f64 MinCells = 3.0;
const f64 ZoomPercent = 0.25f;
//-----------------------------------------------------------------------------
VizCHorizontalScale::VizCHorizontalScale(VizCSecurityWidget* parent, VizCWorkArea* workArea) :  QWidget(parent), Parent(parent), IsSelected(false), WorkArea(workArea)
{
	Q_ASSERT(Parent);
	Q_ASSERT(WorkArea);


	setAttribute(Qt::WA_DeleteOnClose);
	//setMouseTracking(true);
	setAcceptDrops(false);

	Settings = io::VizCSettings::instance();
	resetToDefaults();

	QHBoxLayout* layoutMain = new QHBoxLayout;
	QGridLayout* layoutToolBar = new QGridLayout;


	Dummy = new QWidget;
	createChartToolBar(layoutToolBar);

	createPeriodicityMenu();

	layoutMain->addWidget(Dummy);
	layoutMain->addLayout(layoutToolBar);
	
	layoutToolBar->setMargin(0);
	layoutToolBar->setSpacing(0);
	layoutMain->setMargin(0);
	layoutMain->setSpacing(0);
	
	setLayout(layoutMain);

	LowerCategory = YearlyStart;

	SliderStart = 0.0;
	SliderEnd = 1.0;
	SliderMoveState = 0;

	vizObjectSetName(this, tr("Eixo X"));

	QPalette pal = palette();
	BackgroundColorHighlight1 = pal.color(QPalette::Highlight);
	BackgroundColorHighlight2 = QColor(0,0,0,0);

}
//-----------------------------------------------------------------------------
VizCHorizontalScale::~VizCHorizontalScale()
{
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::blockCreateUndoCommandPropertyChanged(bool block)
{
	VizCGraphicsPanel* panel = WorkArea->selectedGraphicsPanel();
	Q_ASSERT(panel);
	panel->blockCreateUndoCommandPropertyChanged(block);
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::resetToDefaults()
{

	Periodicity = viz::VESP_DEFAULT;
	LeftMargin = 0;
	RightMargin = 0;
	GridVisible = false;

	QString tmp = Settings->value("Appearance/DefaultFontAndColor/Font").toString();
	Font.fromString(tmp);
	Color = QColor(Settings->value("Appearance/DefaultFontAndColor/ForegroundColor").toString());
	BackGroundColor = QColor(Settings->value("Appearance/DefaultFontAndColor/BackgroundColor").toString());
	GridColor = QColor(Settings->value("Appearance/DefaultFontAndColor/GridColor").toString());
	GridPenStyle = static_cast<viz::VizEPenStyle>(Settings->value("Appearance/DefaultFontAndColor/GridStyle").toInt());
	GridPenWidth = static_cast<viz::VizEPenWidth>(Settings->value("Appearance/DefaultFontAndColor/GridWidth").toInt());

	repaint();
	emit horizontalScaleChange(false, false, false, false);
}
//-----------------------------------------------------------------------------
bool VizCHorizontalScale::isSelected() const
{
	return IsSelected;
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::setSelected(bool selected)
{
	if(IsSelected != selected)
	{
		IsSelected = selected;
		repaint();
	}
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::setStartData(const QDateTime& date)
{
	if(StartDate != date)
	{
		StartDate = date;

		calculeRulValToDateMap();
		emit horizontalScaleChange(true, true, false, true);
		repaint();
	}
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::setEndData(const QDateTime& date)
{
	if(EndDate != date)
	{
		EndDate = date;

		calculeRulValToDateMap();
		emit horizontalScaleChange(true, true, false, true);
		repaint();
	}
}
//-----------------------------------------------------------------------------
QDateTime VizCHorizontalScale::startData() const
{
	return StartDate;
}
//-----------------------------------------------------------------------------
QDateTime VizCHorizontalScale::endData() const
{
	return EndDate;
}
//-----------------------------------------------------------------------------
bool VizCHorizontalScale::findRuleValueIndex(s32& index, const QDateTime& dateTime, bool findNextIndex)
{
	index = 0;
	bool ok = false;
	if(!dateTime.isValid())
	{
		return ok;
	}

	QVector<QDateTime>::iterator i = qBinaryFind(RulerValues.begin(), RulerValues.end(), dateTime);
	index = i - RulerValues.begin();
	ok = i != RulerValues.end();

	//index = RulerValues.indexOf(dateTime);
	//ok = index != -1;

	if(!ok && findNextIndex)
	{
		for(s32 i = 0; i < RulerValues.size(); i++)
		{
			if(RulerValues[i] > dateTime)
			{
				index = i;
				ok = true;
				break;
			}
		}
	}

	return ok;
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::createChartToolBar(QGridLayout* layout)
{
	const f64 fsize = 16;
	const QSize size(fsize, fsize);
	const QSize size1(23, 23);

	PeriodicityPixmaps[0] = QPixmap(":/VizTrader/Resources/PeriodicityDaily.png");
	PeriodicityPixmaps[1] = QPixmap(":/VizTrader/Resources/PeriodicityWeekly.png");
	PeriodicityPixmaps[2] = QPixmap(":/VizTrader/Resources/PeriodicityMonthly.png");
	PeriodicityPixmaps[3] = QPixmap(":/VizTrader/Resources/PeriodicityQuarterly.png");
	PeriodicityPixmaps[4] = QPixmap(":/VizTrader/Resources/PeriodicityYearly.png");

	switch(Periodicity)
	{
		case viz::VESP_DAILY:
		{
			PeriodicityPixmapIndex = 0;
			break;
		}
		case viz::VESP_WEEKLY:
		{
			PeriodicityPixmapIndex = 1;
			break;
		}
		case viz::VESP_MONTHLY:
		{
			PeriodicityPixmapIndex = 2;
			break;
		}
		case viz::VESP_QUARTERLY:
		{
			PeriodicityPixmapIndex = 3;
			break;
		}
		case viz::VESP_YEARLY:
		{
			PeriodicityPixmapIndex = 4;
			break;
		}
	};

	PeriodicityButton = new QToolButton;
	//ZoomInButton->setCheckable(true);
	PeriodicityButton->setText(tr("Periodicidade"));
	PeriodicityButton->setToolTip(tr("Periodicidade"));
	PeriodicityButton->setIcon(PeriodicityPixmaps[PeriodicityPixmapIndex]);
	PeriodicityButton->setIconSize(size);
	PeriodicityButton->setMaximumSize(size1);
	PeriodicityButton->setAutoRaise(true);

	QToolButton* ZoomResetButton = new QToolButton;
	ZoomResetButton->setText(tr("Reiniciar Zoom"));
	ZoomResetButton->setToolTip(tr("Reiniciar Zoom"));
	ZoomResetButton->setIcon(QPixmap(":/VizTrader/Resources/ZoomReset.png"));	
	ZoomResetButton->setIconSize(size);
	ZoomResetButton->setMaximumSize(size1);
	ZoomResetButton->setAutoRaise(true);

	QToolButton* ZoomOutButton = new QToolButton;
	ZoomOutButton->setText(tr("Menos Zoom"));
	ZoomOutButton->setToolTip(tr("Menos Zoom"));
	ZoomOutButton->setIcon(QPixmap(":/VizTrader/Resources/ZoomOut.png"));
	ZoomOutButton->setIconSize(size);
	ZoomOutButton->setMaximumSize(size1);
	ZoomOutButton->setAutoRaise(true);
	
	QToolButton* ZoomInButton = new QToolButton;
	ZoomInButton->setText(tr("Mais Zoom"));
	ZoomInButton->setToolTip(tr("Mais Zoom"));
	ZoomInButton->setIcon(QPixmap(":/VizTrader/Resources/ZoomIn.png"));
	ZoomInButton->setIconSize(size);
	ZoomInButton->setMaximumSize(size1);
	ZoomInButton->setAutoRaise(true);

	QButtonGroup* ButtonGroup = new QButtonGroup;
	ButtonGroup->setExclusive(false);

	ButtonGroup->addButton(ZoomResetButton, VEBI_ZoomResetButton);
	ButtonGroup->addButton(ZoomOutButton, VEBI_ZoomOutButton);
	ButtonGroup->addButton(ZoomInButton, VEBI_ZoomInButton);
	ButtonGroup->addButton(PeriodicityButton, VEBI_ZoomPeriodicityButton);

	connect(ButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(buttonGroupClicked(int)));

	layout->addWidget(PeriodicityButton, 0, 0);
	layout->addWidget(ZoomResetButton, 0, 1);
	layout->addWidget(ZoomOutButton, 1, 0);
	layout->addWidget(ZoomInButton, 1, 1);


}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::createPeriodicityMenu()
{
	//connect(HorizontalScale, SIGNAL(periodicityChange()), this, SLOT(slotSetPeriodicity()));

	
	PeriodicityActions[0] = new QAction(tr("Diário"), this);
	PeriodicityActions[0]->setStatusTip(tr("Diário"));
	PeriodicityActions[0]->setCheckable(true);
	connect(PeriodicityActions[0], SIGNAL(triggered()), this, SLOT(slotSetPeriodicityDayly()));

	PeriodicityActions[1] = new QAction(tr("Semanal"), this);
	PeriodicityActions[1]->setStatusTip(tr("Semanal"));
	PeriodicityActions[1]->setCheckable(true);
	connect(PeriodicityActions[1], SIGNAL(triggered()), this, SLOT(slotSetPeriodicityWeekly()));

	PeriodicityActions[2] = new QAction(tr("Mensal"), this);
	PeriodicityActions[2]->setStatusTip(tr("Mensal"));
	PeriodicityActions[2]->setCheckable(true);
	connect(PeriodicityActions[2], SIGNAL(triggered()), this, SLOT(slotSetPeriodicityMonthy()));

	PeriodicityActions[3] = new QAction(tr("Trimensal"), this);
	PeriodicityActions[3]->setStatusTip(tr("Trimensal"));
	PeriodicityActions[3]->setCheckable(true);
	connect(PeriodicityActions[3], SIGNAL(triggered()), this, SLOT(slotSetPeriodicityQuarterly()));

	PeriodicityActions[4] = new QAction(tr("Anual"), this);
	PeriodicityActions[4]->setStatusTip(tr("Anual"));
	PeriodicityActions[4]->setCheckable(true);
	connect(PeriodicityActions[4], SIGNAL(triggered()), this, SLOT(slotSetPeriodicityYearly()));

	QActionGroup* PeriodicityGroup = new QActionGroup(this);

	PeriodicityGroup->addAction(PeriodicityActions[0]);
	PeriodicityGroup->addAction(PeriodicityActions[1]);
	PeriodicityGroup->addAction(PeriodicityActions[2]);
	PeriodicityGroup->addAction(PeriodicityActions[3]);
	PeriodicityGroup->addAction(PeriodicityActions[4]);

	switch(Periodicity)
	{
		case viz::VESP_DAILY:
		{
			PeriodicityPixmapIndex = 0;
			break;
		}
		case viz::VESP_WEEKLY:
		{
			PeriodicityPixmapIndex = 1;
			break;
		}
		case viz::VESP_MONTHLY:
		{
			PeriodicityPixmapIndex = 2;
			break;
		}
		case viz::VESP_QUARTERLY:
		{
			PeriodicityPixmapIndex = 3;
			break;
		}
		case viz::VESP_YEARLY:
		{
			PeriodicityPixmapIndex = 4;
			break;
		}
	};

	PeriodicityActions[PeriodicityPixmapIndex]->setChecked(true);

	PeriodicityMenu = new QMenu;
	PeriodicityMenu->addAction(PeriodicityActions[0]);
	PeriodicityMenu->addAction(PeriodicityActions[1]);
	PeriodicityMenu->addAction(PeriodicityActions[2]);
	PeriodicityMenu->addAction(PeriodicityActions[3]);
	PeriodicityMenu->addAction(PeriodicityActions[4]);
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::setCurrentPeriodicityPixmap()
{
	switch(Periodicity)
	{
		case viz::VESP_DAILY:
		{
			PeriodicityPixmapIndex = 0;
			break;
		}
		case viz::VESP_WEEKLY:
		{
			PeriodicityPixmapIndex = 1;
			break;
		}
		case viz::VESP_MONTHLY:
		{
			PeriodicityPixmapIndex = 2;
			break;
		}
		case viz::VESP_QUARTERLY:
		{
			PeriodicityPixmapIndex = 3;
			break;
		}
		case viz::VESP_YEARLY:
		{
			PeriodicityPixmapIndex = 4;
			break;
		}
	};
	PeriodicityButton->setIcon(PeriodicityPixmaps[PeriodicityPixmapIndex]);
	PeriodicityActions[PeriodicityPixmapIndex]->setChecked(true);
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::slotSetPeriodicityDayly()
{
	setPeriodicity(viz::VESP_DAILY);
	WorkArea->updateProperty(this, "periodicity");
	setCurrentPeriodicityPixmap();
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::slotSetPeriodicityWeekly()
{
	setPeriodicity(viz::VESP_WEEKLY);
	WorkArea->updateProperty(this, "periodicity");
	setCurrentPeriodicityPixmap();
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::slotSetPeriodicityMonthy()
{
	setPeriodicity(viz::VESP_MONTHLY);
	WorkArea->updateProperty(this, "periodicity");
	setCurrentPeriodicityPixmap();
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::slotSetPeriodicityQuarterly()
{
	setPeriodicity(viz::VESP_QUARTERLY);
	WorkArea->updateProperty(this, "periodicity");
	setCurrentPeriodicityPixmap();
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::slotSetPeriodicityYearly()
{
	setPeriodicity(viz::VESP_YEARLY);
	WorkArea->updateProperty(this, "periodicity");
	setCurrentPeriodicityPixmap();
}
//-----------------------------------------------------------------------------
QList<s32> VizCHorizontalScale::visibleRulerValuesIndex() const
{
	return VisibleRulerValuesIndices;
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::currentVisibleRulerValuesIndex(s32& start, s32& end) const
{
	start = static_cast<s32>(SliderStart * RulerValues.size());
	end = static_cast<s32>(SliderEnd * RulerValues.size()) - 1;
}
//-----------------------------------------------------------------------------
s32 VizCHorizontalScale::beginVisibleRulerValuesIndex() const
{
	s32 value = static_cast<s32>(SliderStart * RulerValues.size());
	return value;
}
//-----------------------------------------------------------------------------
s32 VizCHorizontalScale::endVisibleRulerValuesIndex() const
{
	s32 value = static_cast<s32>(SliderEnd * RulerValues.size()) - 1;
	return value;
}
//-----------------------------------------------------------------------------
QVector<core::VizCTradePeriod> VizCHorizontalScale::ajustPeriods(const QVector<core::VizCTradePeriod>& periods)
{
	if(RulerValues.isEmpty())
		return periods;

	QMap<QDateTime, core::VizCTradePeriod> map;

	foreach(core::VizCTradePeriod sp, periods)
	{
		map[sp.TimeStamp] = sp;
	}
	QVector<core::VizCTradePeriod> out(RulerValues.size());
	for(s32 i = 0; i < out.size(); i++)
	{
		out[i].TimeStamp = RulerValues[i];
	}
	for(s32 i = 0; i < out.size(); i++)
	{
		QMap<QDateTime, core::VizCTradePeriod>::const_iterator it = map.find(out[i].TimeStamp);
		if(it != map.end())
		{
			out[i] = it.value();
		}
	}

	return out;

}
//-----------------------------------------------------------------------------
QColor VizCHorizontalScale::gridColor() const
{
	return GridColor;
}
//-----------------------------------------------------------------------------
viz::VizEPenWidth VizCHorizontalScale::gridPenWidth() const
{
	return GridPenWidth;
}
//-----------------------------------------------------------------------------
viz::VizEPenStyle VizCHorizontalScale::gridPenStyle() const
{
	return GridPenStyle;
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::setGridColor(const QColor& color)
{
	if(GridColor != color)
	{
		GridColor = color;
		emit horizontalScaleChange(false, false, false, false);
	}
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::setGridPenWidth(viz::VizEPenWidth penWidth)
{
	if(GridPenWidth != penWidth)
	{
		GridPenWidth = penWidth;
		emit horizontalScaleChange(false, false, false, false);
	}
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::setGridPenStyle(VizEPenStyle penStyle)
{
	if(GridPenStyle != penStyle)
	{
		GridPenStyle = penStyle;
		emit horizontalScaleChange(false, false, false, false);
	}
}
//-----------------------------------------------------------------------------
const QVector<QDateTime>& VizCHorizontalScale::rulerValues() const
{
	return RulerValues;
}
//-----------------------------------------------------------------------------
viz::VizESecurityPeriodicity VizCHorizontalScale::periodicity() const
{
	return Periodicity;
}
//-----------------------------------------------------------------------------
bool VizCHorizontalScale::isGridVisible() const
{
	return GridVisible;
}
//-----------------------------------------------------------------------------
QColor VizCHorizontalScale::color() const
{
	return Color;
}
//-----------------------------------------------------------------------------
QColor VizCHorizontalScale::backGroundColor() const
{
	return BackGroundColor;
}
//-----------------------------------------------------------------------------
s32 VizCHorizontalScale::rightMargin() const
{
	return RightMargin;
}
//-----------------------------------------------------------------------------
s32 VizCHorizontalScale::leftMargin() const
{
	return LeftMargin;
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::setRightMargin(s32 rightmargin)
{
	if(rightmargin != RightMargin)
	{
		RightMargin = rightmargin;
		emit horizontalScaleChange(true, true, false, true);
	}
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::setLeftMargin(s32 leftmargin)
{
	if(leftmargin != LeftMargin)
	{
		LeftMargin = leftmargin;
		emit horizontalScaleChange(true, true, false, true);
	}
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::pan(s32 delta)
{

	f64 deltaF = delta / static_cast<f64>(RulerValues.size());

	f64 x0 = SliderStart + deltaF;
	f64 x1 = SliderEnd + deltaF;

	if(core::isInRange(x0, 0, 1.0) && core::isInRange(x1, 0.0, 1.0))
	{
		SliderStart = x0;
		SliderEnd = x1;

		repaint();
		calculeRulValToDateMap();
		emit horizontalScaleChange(true, false, false, false);

	}
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::resetZoom()
{
	if(SliderStart == 0.0 && SliderEnd == 1.0)
	{
		QApplication::beep();
		return;
	}

	SliderStart = 0.0;
	SliderEnd = 1.0;

	emit horizontalScaleChange(true, false, false, false);
	calculeRulValToDateMap();
	repaint();

}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::zoomIn()
{
	f64 range = SliderEnd - SliderStart;
	f64 newSliderStar = SliderStart + (range * ZoomPercent);

	QRect viewport = slidersViewPort();
	f64 w = static_cast<f64>(viewport.width());
	f64 w_ = ((gui::MinimalVisibleCell * w) / static_cast<f64>(RulerValues.size() - 1)) / w;

	if(core::isInRange(newSliderStar, 0, SliderEnd) && ((SliderEnd - newSliderStar) >= w_))
	{
		SliderStart = newSliderStar;
		emit horizontalScaleChange(true, false, false, false);
		calculeRulValToDateMap();
		repaint();
	}
	else
	{
		QApplication::beep();
	}
	
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::zoomOut()
{
	
	QRect viewport = slidersViewPort();
	f64 w = static_cast<f64>(viewport.width());
	f64 w_ = ((gui::MinimalVisibleCell * w) / static_cast<f64>(RulerValues.size() - 1)) / w;

	f64 range = SliderEnd - SliderStart;
	bool beep = false;
	bool repaintX = false;

	if(core::equals(SliderStart, 0.0))
	{
		f64 newSliderEnd = SliderEnd + (range * ZoomPercent);
		if(newSliderEnd > 1.0)
		{
			newSliderEnd = 1.0;
			beep = true;
		}
		if(core::isInRange(newSliderEnd, SliderStart, 1.0) && ((newSliderEnd - SliderStart) >= w_))
		{
			SliderEnd = newSliderEnd;
			repaintX = true;
		}
		else
		{
			beep = true;
		}
	}
	else
	{
		f64 newSliderStar = SliderStart - (range * ZoomPercent);
		if(newSliderStar < 0.0)
		{
			newSliderStar = 0.0;
			beep = true;
		}
		if(core::isInRange(newSliderStar, 0, SliderEnd) && ((SliderEnd - newSliderStar) >= w_))
		{
			SliderStart = newSliderStar;
			repaintX = true;
		}
		else
		{
			beep = true;
		}
	}

	if(beep)
	{
		QApplication::beep();
	}
	if(repaintX)
	{
		emit horizontalScaleChange(true, false, false, false);
		calculeRulValToDateMap();
		repaint();
	}



}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::zoomBox(s32 startRulerValueIndex, s32 endRulerValueIndex)
{
	f64 size = static_cast<f64>(RulerValues.size());

	Q_ASSERT(core::isInRange(startRulerValueIndex, 0, size));
	Q_ASSERT(core::isInRange(endRulerValueIndex, 0, size));

	SliderStart = startRulerValueIndex / size;
	SliderEnd = endRulerValueIndex / size;

	emit horizontalScaleChange(true, false, false, false);
	calculeRulValToDateMap();
	repaint();
	

}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::setPeriodicity(const viz::VizESecurityPeriodicity periodicity)
{
	if(Periodicity != periodicity)
	{
		Periodicity = periodicity;
		emit horizontalScaleChange(true, true, true, false);
	}
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::setRulerValues(const QVector<QDateTime>& values)
{
	RulerValues = values;
	calculeRulValToDateMap();
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::setPeriodsToPreviewChart(const QVector<core::VizCTradePeriod> periods)
{
	Periods = periods;

	MaxY = std::numeric_limits<f64>::min();
	MinY = std::numeric_limits<f64>::max();
	foreach(const core::VizCTradePeriod& Period, periods)
	{
		if(Period.ClosePrice > MaxY)
			MaxY = Period.ClosePrice;
		if(Period.ClosePrice < MinY)
			MinY = Period.ClosePrice;
	}

	f64 border = (MaxY-MinY) * 0.25;
	MaxY = MaxY + border;


}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::setGridVisible(bool b)
{
	if(GridVisible != b)
	{
		GridVisible = b;
		emit horizontalScaleChange(false, false, false, false);
	}
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::setColor(const QColor& color)
{
	if(Color != color)
	{
		Color = color;
		repaint();
	}
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::setBackgroundColor(const QColor& color)
{
	if(BackGroundColor != color)
	{
		BackGroundColor = color;
		repaint();
	}
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::setFont(const QFont& f)
{
	if(Font != f)
	{
		Font = f;
		calculeRulValToDateMap();
	}
}
//-----------------------------------------------------------------------------
QFont VizCHorizontalScale::font() const
{
	return Font;
}
//-----------------------------------------------------------------------------
int VizCHorizontalScale::propertyGetPeriodicity() const
{
	return static_cast<viz::VizESecurityPeriodicity>(periodicity());
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::propertySetPeriodicity(int value)
{
	setPeriodicity(static_cast<viz::VizESecurityPeriodicity>(value));
	setCurrentPeriodicityPixmap();
}
//-----------------------------------------------------------------------------
int VizCHorizontalScale::propertyGetGridPenWidth() const
{
	return static_cast<int>(gridPenWidth() - 1);
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::propertySetGridPenWidth(int value)
{
	setGridPenWidth(static_cast<viz::VizEPenWidth>(value + 1));
}
//-----------------------------------------------------------------------------
int VizCHorizontalScale::propertyGetPenStyle() const
{
	return static_cast<viz::VizEPenStyle>(gridPenStyle());
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::propertySetPenStyle(int value)
{
	setGridPenStyle(static_cast<viz::VizEPenStyle>(value));
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::resizeEvent(QResizeEvent* event)
{
	if(event->oldSize().width() != event->size().width())
	{
		calculeRulValToDateMap();
	}
	QWidget::resizeEvent(event);
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::mousePressEvent(QMouseEvent* event)
{
	if(VizCToolManager::instance()->isToolSelected(VETS_SELECTION))
	{
		WorkArea->setSelectedVizOBject(this);
		WorkArea->updateScene();
	}

	QRectF sliderStart, sliderEnd, sliderTranslate;
	computeSlidersRects(sliderStart, sliderEnd, sliderTranslate);

	QSizeF sizePlusOne1 = QSizeF(sliderStart.width() + 3.0, sliderStart.height() + 3.0);
	QSizeF sizePlusOne2 = QSizeF(sliderEnd.width() + 3.0, sliderEnd.height() + 3.0);
	QPointF p1 = sliderStart.center();
	QPointF p2 = sliderEnd.center();
	QPointF point1(p1.x() - (sizePlusOne1.width() * 0.5), p1.y() - (sizePlusOne1.height() * 0.5));
	QPointF point2(p2.x() - (sizePlusOne2.width() * 0.5), p2.y() - (sizePlusOne2.height() * 0.5));
	QRectF rect1(point1, sizePlusOne1);
	QRectF rect2(point2, sizePlusOne2);


	QPointF pos = event->localPos();

	if(rect1.contains(pos))
	{
		SliderMoveState = 1;
	}
	else
	if(rect2.contains(pos))
	{
		SliderMoveState = 2;
	}
	else
	if(sliderTranslate.contains(pos))
	{
		SliderMoveState = 3;
	}
	else
	{
		SliderMoveState = 0;
	}
	
	QWidget::mousePressEvent(event);
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::mouseMoveEvent(QMouseEvent* event)
{
	QPointF pos = event->localPos();
	QRect viewport = slidersViewPort();

	bool ok = false;

	f64 w = static_cast<f64>(viewport.width());
	f64 w_ = ((gui::MinimalVisibleCell * w) / static_cast<f64>(RulerValues.size() - 1)) / w;

	if(SliderMoveState == 1)
	{
		f64 x = pos.x() / w;

		if(core::isInRange(x, 0, SliderEnd) && ((SliderEnd - x) >= w_))
		{
			SliderStart = x;
			ok = true;
		}
	}
	else
	if(SliderMoveState == 2)
	{
		f64 x = pos.x() / w;

		if(core::isInRange(x, SliderStart, 1.0) && ((x - SliderStart) >= w_))
		{
			SliderEnd = x;
			ok = true;
		}
	}
	else
	if(SliderMoveState == 3)
	{
		f64 delta = SliderTranslate != 0.0 ? pos.x() - SliderTranslate : 0.0;
		
		delta = delta / w;

		f64 x0 = SliderStart + delta;
		f64 x1 = SliderEnd + delta;

		if(core::isInRange(x0, 0, 1.0) && core::isInRange(x1, 0.0, 1.0))
		{
			SliderStart = x0;
			SliderEnd = x1;
			ok = true;
		}

		SliderTranslate = pos.x();

	}

	if(ok)
	{
		emit horizontalScaleChange(true, false, false, false);
		calculeRulValToDateMap();
		repaint();
	}

	QWidget::mouseMoveEvent(event);
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::mouseReleaseEvent(QMouseEvent* event)
{
	SliderTranslate = 0.0;
	SliderMoveState = 0;
	QWidget::mouseReleaseEvent(event);
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::buttonGroupClicked(s32 id)
{
	if(id == VEBI_ZoomPeriodicityButton)
	{
		PeriodicityMenu->exec(QCursor::pos());
	}
	if(id == VEBI_ZoomResetButton)
	{
		resetZoom();
	}
	if(id == VEBI_ZoomOutButton)
	{
		zoomOut();
	}
	if(id == VEBI_ZoomInButton)
	{
		zoomIn();
	}
	
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::calculeRulValToDateMap()
{
	LowerCategory = YearlyStart;

	RulValToDateMap.clear();

	//TODO: MAKE IT WORK WITH CHANGING FONTS
	if (MinimumPxlsPerFont.size() == 0)
		fillPxlsMinimumPerFont();

	fillRulValToDateMapLoop();

	changeIntraPeriodRulValToDateMap();

	fillVisibleRulerValues();
}

//-----------------------------------------------------------------------------
void VizCHorizontalScale::fillVisibleRulerValues()
{
	VisibleRulerValuesIndices.clear();

	s32 start, end;
	currentVisibleRulerValuesIndex(start, end);

	//QList<s32>::const_iterator iKeys = RulValToDateMap.keys().constBegin();
	QMap<s32, s32>::const_iterator iKeys = RulValToDateMap.constBegin();

	//while (iKeys != RulValToDateMap.keys().constEnd())
	while (iKeys != RulValToDateMap.constEnd())
	{
		//if (*iKeys > end)
		if (iKeys.key() > end)
			break;
		if (iKeys.key() >= start)
			VisibleRulerValuesIndices.push_back(iKeys.key());

		iKeys++;
	}
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::fillPxlsMinimumPerFont()
{
	QFontMetrics fm(Font);
	s32 stringWidth = 0;
	s32 widestDigit = 0;
	
	for (s32 i = 0; i < 10; i++)
	{
		stringWidth = fm.width(QString::number(i));
		widestDigit = (widestDigit < stringWidth) ? 
			stringWidth : widestDigit;
	}

	const s32 spaceWidth = fm.width(" ");
	const s32 widestDigitYear = widestDigit * 4;	
	const s32 widestShortDigitYear  = widestDigit * 2;

	const s32 dummyYear  = 1900;
	const s32 dummyMonth = 1;
	const s32 dummyDay = 1;

	QDate widestDay(dummyYear, dummyMonth, dummyDay);	
	s32 widestShortMonth = 0;	
	for (s32 i = 1; i < 13; i++)
	{
		widestDay.setDate(dummyYear, i, dummyDay);
		stringWidth = fm.width(locale().toString(widestDay, "MMM"));
		widestShortMonth = (widestShortMonth < stringWidth) ? 
			stringWidth : widestShortMonth;
	}

	s32 widestShortDayText = 0;
	for (s32 i = 1; i < 8; i++)
	{
		widestDay.setDate(dummyYear, dummyMonth, i);
		stringWidth = fm.width(locale().toString(widestDay, "ddd"));
		widestShortDayText = (widestShortDayText < stringWidth) ? 
			stringWidth : widestShortDayText;
	}

	//notice that this leads to impossible dates, as in february, but that its not important
	s32 widestDigitDay = 0;
	for (s32 i = 1; i < 32; i++)
	{
		stringWidth = fm.width(QString::number(i));
		widestDigitDay = (widestDigitDay < stringWidth) ? 
			 stringWidth : widestDigitDay ;
	}	

	//-----hierarquy------	
	//INTRADAY
	//

	//DAILY
	//short day ... 12 oct; 13; 14; 15; 16 ...
	//short day + short month ... 12 oct; 13 oct; 14 oct ...
	//day + short day + short month ... tue 12 aug; wed 13 aug; sat 14 aug... 	
	MinimumPxlsPerFont.push_back(widestDigitDay);
	MinimumPxlsPerFont.push_back(widestDigitDay + spaceWidth + widestShortMonth);
	MinimumPxlsPerFont.push_back(widestShortDayText + spaceWidth + widestDigitDay + 
		spaceWidth + widestShortMonth);
	//MONTHLY
	//short month  ... aug 99; sep; oct; nov; dec; jan 00; feb ...
	//short month + short year ... aug 99; sep 99; oct 99 ... 
	//short month + long year ... aug 1999; sep 1999; oct 1999 ...
	MinimumPxlsPerFont.push_back(widestShortMonth);
	MinimumPxlsPerFont.push_back(widestShortMonth + spaceWidth + widestShortDigitYear);
	MinimumPxlsPerFont.push_back(widestShortMonth + spaceWidth + widestDigitYear);
	//YEARLY
	//short year  ... 99 ; 00 ; 01 ...
	//long  year  ... 1999 ; 2000 ; 2001 ...	
	MinimumPxlsPerFont.push_back(widestShortDigitYear);
	MinimumPxlsPerFont.push_back(widestDigitYear);

	

}
//-----------------------------------------------------------------------------
s32 VizCHorizontalScale::selectIntraPeriodCategory(s32 pxlsAvailable, s32 indexStart)
{
	switch (indexStart)
	{
	case DailyStart:
		for (s32 i = MonthlyStart - 1; i >= static_cast<int>(DailyStart); i--)
			if (pxlsAvailable >= MinimumPxlsPerFont[i])
				return i;			
		break;
	case MonthlyStart:
		for (s32 i = YearlyStart - 1; i >= MonthlyStart; i--)
			if (pxlsAvailable >= MinimumPxlsPerFont[i])
				return i;					
		break;
	case YearlyStart:
		for (s32 i = MinimumPxlsPerFont.size() - 1; i >= YearlyStart; i--)
			if (pxlsAvailable >= MinimumPxlsPerFont[i])
				return i;		
		break;
	case WithoutString:
		return WithoutString;
		break;		
	}

	return -1;
	
}
//-----------------------------------------------------------------------------
s32 VizCHorizontalScale::currentDate(s32 indexStart, QDateTime current)
{
	switch (indexStart)
	{
	case DailyStart:
		return current.date().day();
		break;
	case MonthlyStart:
		return current.date().month();
		break;
	case YearlyStart:
		return current.date().year();
		break;
	}

	return -1;
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::changeIntraPeriodRulValToDateMap()
{
	QVector<QDateTime>::const_iterator iRuler = rulerValues().constBegin();
	QMap<s32, s32>::iterator iDraw  = RulValToDateMap.begin();
	
	//TODO:colocar valor anterior
	s32 count = 0;
	while (iDraw != RulValToDateMap.end())  
	{
		for (s32 i = iDraw.key(); i > count; i--)
			iRuler++;		

		f64 availablePxls = ((iDraw.key() - count) * pxlsPerRulerValue()) - PxlThreshold;
		count = iDraw.key();

		if (iDraw != RulValToDateMap.begin())
		{
			iDraw--;
			s32 finalIndex = selectIntraPeriodCategory(availablePxls, iDraw.value());
			if (finalIndex != -1)
				iDraw.value() = finalIndex;
			iDraw++;
		}

		iDraw++;
	}	

	if (iDraw != RulValToDateMap.begin())
	{
		iDraw--;
		s32 start, end;
		currentVisibleRulerValuesIndex(start, end);
		//f32 availablePxls = (Dummy->width() - ((iDraw.key() - start) * pxlsPerRulerValue())) - PxlThreshold;
		f64 availablePxls = (Dummy->geometry().width() - ((iDraw.key() - start) * pxlsPerRulerValue())) - PxlThreshold;
		
		s32 finalIndex = selectIntraPeriodCategory(availablePxls, iDraw.value());
		
		if (finalIndex != -1)
			iDraw.value() = finalIndex;		
	}
}
//-----------------------------------------------------------------------------
s32 VizCHorizontalScale::lowerCategory(s32 indexStart)
{
	
	switch (indexStart)
	{
		case DailyStart:
			return -1;
			break;
		case MonthlyStart:
			if (Periodicity < viz::VESP_MONTHLY)
				return DailyStart;
			break;
		case YearlyStart:
			if (Periodicity < viz::VESP_YEARLY)
				return MonthlyStart;
			break;			
	}

	return -1;
}
//-----------------------------------------------------------------------------
s32 VizCHorizontalScale::availablePxlsHorizontal() const
{
	return Dummy->geometry().width();
}
//-----------------------------------------------------------------------------
f64 VizCHorizontalScale::pxlsPerRulerValue() const
{	
	//return (static_cast<f32>(availablePxlsHorizontal()) / static_cast<f32>(RulerValues.size()));
	//return (static_cast<f32>(availablePxlsHorizontal()) / static_cast<f32>(RulerValues.size()));

	s32 start, end;
	currentVisibleRulerValuesIndex(start, end);
	return (static_cast<f64>(Dummy->geometry().width()) / static_cast<f64>(end - start + 1));
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::fillInterval(f64 fpxlDistance, s32 minPixelsIndex, s32 drawIndex,
		QVector<QDateTime>::const_iterator iFirstDate, QVector<QDateTime>::const_iterator iLastDate)
{
	if (fpxlDistance < MinimumPxlsPerFont[minPixelsIndex])
		return;

	QVector<QDateTime>::const_iterator intervalIteratorEnd   = iFirstDate;
	QVector<QDateTime>::const_iterator intervalIteratorStart = iFirstDate;

	s32 intervalStartDate = currentDate(minPixelsIndex, *iFirstDate);

	s32 startIndex = drawIndex;
	s32 endIndex   = drawIndex;

	s32 periodicityCount = 0;
	f64 viewportPxlDifference = 0.0;

	if (fpxlDistance > 0.0)
		while (intervalIteratorEnd != iLastDate) 
		{
			QDateTime intervalEndDate = *intervalIteratorEnd;
			if (intervalStartDate != currentDate(minPixelsIndex, intervalEndDate))
			{
				f64	availablePxls = ((endIndex - startIndex) * pxlsPerRulerValue()) - PxlThreshold;

				if (availablePxls  > MinimumPxlsPerFont[minPixelsIndex])
				{
					if (lowerCategory(minPixelsIndex) != -1)
						fillInterval(availablePxls, lowerCategory(minPixelsIndex), startIndex, 
							intervalIteratorStart, intervalIteratorEnd);

					intervalIteratorStart = intervalIteratorEnd;

					//TODO: this works only forwarding...
					if (minPixelsIndex < LowerCategory)
						LowerCategory = minPixelsIndex;					
					
					RulValToDateMap.insert(startIndex, minPixelsIndex);

					startIndex = endIndex;
					periodicityCount = 0;
				} else 
				{
					periodicityCount++;
				}

				bool periodicityJumpReached = (LowerCategory == minPixelsIndex) ? false : 
					(periodicityCount > 2);

				if ((minPixelsIndex != YearlyStart) && periodicityJumpReached)				
					return;
				
				intervalStartDate = currentDate(minPixelsIndex, intervalEndDate);
			}

			++intervalIteratorEnd;
			++endIndex;
		}

	if (intervalIteratorStart != iFirstDate )
	{
		--intervalIteratorStart;
		--intervalIteratorEnd;
	}

	f64	availablePxls = ((endIndex - startIndex) * pxlsPerRulerValue()) - PxlThreshold;

	if (availablePxls  > MinimumPxlsPerFont[minPixelsIndex])
	{
		if (lowerCategory(minPixelsIndex) != -1)
				fillInterval(availablePxls, lowerCategory(minPixelsIndex), startIndex, 
							intervalIteratorStart, intervalIteratorEnd);

		RulValToDateMap.insert(startIndex, minPixelsIndex);	
	}
}


//-----------------------------------------------------------------------------
void VizCHorizontalScale::fillRulValToDateMapLoop()
{
	if (hasRulerValues())
		fillInterval(availablePxlsHorizontal(), YearlyStart, 0, rulerValues().constBegin(), 
			rulerValues().constEnd());
		
}
//-----------------------------------------------------------------------------
QString VizCHorizontalScale::mapToString(QDateTime d, s32 hierarquy)
{
	switch (hierarquy)
	{
	case 0:
		return locale().toString(d, "dd");
		break;
	case 1:
		return locale().toString(d, "dd MMM");
		break;
	case 2:
		return locale().toString(d, "ddd dd MMM");
		break;
	case 3:
		return locale().toString(d, "MMM");
		break;
	case 4:
		return locale().toString(d, "MMM yy");
		break;
	case 5:
		return locale().toString(d, "MMM yyyy");
		break;
	case 6:
		return locale().toString(d, "yy");
		break;
	case 7:
		return locale().toString(d, "yyyy");
		break;
	}
	return "";	
}
//-----------------------------------------------------------------------------
inline bool VizCHorizontalScale::hasRulerValues()
{
	return (RulerValues.size() > 0);
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::drawHorizontalValues(QPainter* painter)
{

	QRect viewport = rulerValuesViewPort();
	
	//if (hasRulerValues() && (RulValToDateMap.size() > 0))
	{

		QMap<s32, s32>::const_iterator iDraw = RulValToDateMap.constBegin();	
		QVector<QDateTime>::const_iterator iRuler = RulerValues.constBegin();	
		
		s32 count = 0;	

		s32 start, end;
		currentVisibleRulerValuesIndex(start, end);		

		while (iDraw.key() < start)
			iDraw++;

		f64 xPosition = 0.0;				
		QFontMetrics fm(Font);
		while ((iDraw != RulValToDateMap.constEnd()) && (iDraw.key() <= end))  
		{
			for (s32 i = iDraw.key(); i > count; i--)			
				iRuler++;
			
	 
			count = iDraw.key();
			f64 pprv = pxlsPerRulerValue();
			xPosition = ((count - start) * pprv) + (pprv / 2.0);		
			
			painter->drawLine(QPointF(xPosition, 4.0), QPointF(xPosition, 0.0));						
			
			if (iDraw.value() != WithoutString)
			{
				QString strValue = mapToString(*iRuler, iDraw.value()); 
							
				//painter.drawText(QPointF(xPosition, 15.0),  strValue);
				s32 w = fm.width(strValue);
				//QRect viewport(xPosition - static_cast<f32>(w / 2), 0, w, Dummy->geometry().height());
				f64 xBegin = xPosition - static_cast<f64>(w / 2);
				xBegin = (xBegin < 0) ? -2 : xBegin;
				QRect v(xBegin + 2, 0, w, viewport.height());
				painter->drawText(v, Qt::AlignCenter, strValue);
			}			
			iDraw++;		
		}


	}

}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::drawSliders(QPainter* painter)
{
	
	QRect viewport = slidersViewPort();

	QRectF sliderStart, sliderEnd, sliderTranslate;
	computeSlidersRects(sliderStart, sliderEnd, sliderTranslate);

	QPalette pal = palette();
	QColor slideTranslateColor = pal.color(QPalette::Window);
	slideTranslateColor.setAlphaF(0.5);

	QColor c = gui::computeShadowColor(BackGroundColor);
	
	c.setAlphaF(0.8);
	painter->setPen(c);

	painter->drawRect(sliderTranslate);
	painter->fillRect(sliderTranslate, slideTranslateColor);

	c.setAlphaF(1.0);
	painter->fillRect(sliderStart, c);
	painter->fillRect(sliderEnd, c);

	painter->save();
	
	QPen constrolPointsPen(Qt::white, 1.0, Qt::SolidLine);
	constrolPointsPen.setCosmetic(true);
	painter->setPen(constrolPointsPen);
	painter->setCompositionMode(QPainter::CompositionMode_Difference);
	
	QSizeF sizePlusOne1 = QSizeF(sliderStart.width() + 3.0, sliderStart.height() + 3.0);
	QSizeF sizePlusOne2 = QSizeF(sliderEnd.width() + 3.0, sliderEnd.height() + 3.0);
	QPointF p1 = sliderStart.center();
	QPointF p2 = sliderEnd.center();
	QPointF point1(p1.x() - (sizePlusOne1.width() * 0.5), p1.y() - (sizePlusOne1.height() * 0.5));
	QPointF point2(p2.x() - (sizePlusOne2.width() * 0.5), p2.y() - (sizePlusOne2.height() * 0.5));
	QRectF rect1(point1, sizePlusOne1);
	QRectF rect2(point2, sizePlusOne2);
	painter->drawRect(rect1);
	painter->drawRect(rect2);

	painter->restore();

}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::drawChart(QPainter* painter)
{
	if(Periods.isEmpty())
		return;

	QRect viewport = slidersViewPort();

	QPolygonF polyline;
	f64 x = 0.0;
	polyline << QPointF(viewport.left(), viewport.bottom());

	s32 size = Periods.size();
	
	f64 scaleX = viewport.width() / static_cast<f64>(size);
	f64 scaleY = -1.0 * viewport.height() / (MaxY - MinY);

	for(s32 i = 0; i < size; i++)
	{
		x = static_cast<f64>(i) * scaleX;
		polyline << QPointF(x, (Periods[i].ClosePrice - MinY) * scaleY + viewport.bottom());
	}
	polyline << QPointF(viewport.right(), viewport.bottom());
	polyline << QPointF(viewport.left(), viewport.bottom());

	QPainterPath path;
	path.addPolygon(polyline);

	QColor c = gui::computeShadowColor(BackGroundColor);

	QColor ForegroundColor1 = BackGroundColor;
	QColor ForegroundColor2 = c;


	/*QLinearGradient gradientFpreground(0, viewport.top(), 0, viewport.bottom());
	gradientFpreground.setColorAt(0.0, ForegroundColor1);
	gradientFpreground.setColorAt(1.0, ForegroundColor2);
	//painter->fillPath(path, gradientFpreground);
	*/

	painter->setRenderHint(QPainter::Antialiasing, true);

	c.setAlphaF(0.3);
	painter->fillPath(path, c);
	c.setAlphaF(0.5);
	painter->setPen(QPen(c, 2.0));
	painter->drawPath(path);

	painter->setRenderHint(QPainter::Antialiasing, false);


}
//-----------------------------------------------------------------------------
QRect VizCHorizontalScale::rulerValuesViewPort()
{
	QRect viewport = Dummy->geometry();
	//viewport.setWidth(viewport.width()+1);
	viewport.setHeight(viewport.height()*0.5);
	return viewport;
}
//-----------------------------------------------------------------------------
QRect VizCHorizontalScale::slidersViewPort()
{
	QRect viewport = Dummy->geometry();
	//viewport.setWidth(viewport.width()+1);
	viewport.translate(QPoint(0, viewport.height()*0.5));
	viewport.setHeight(viewport.height()*0.5);
	return viewport;
}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::computeSlidersRects(QRectF& sliderStart, QRectF& sliderEnd, QRectF& sliderTranslate)
{
	QRect viewport = slidersViewPort();
	f64 xPositionSliderStart = SliderStart * viewport.width();	
	f64 xPositionSliderEnd = SliderEnd * viewport.width();

	sliderTranslate = QRectF(QPointF(xPositionSliderStart, viewport.top()), QPointF(xPositionSliderEnd, viewport.bottom()));

	f64 w = 8.0;
	f64 h = viewport.height() * 0.5;

	QPointF p1(xPositionSliderStart - (w * 0.5), (viewport.top() + viewport.height() * 0.5) - (h * 0.5));
	QPointF p2(xPositionSliderEnd - (w * 0.5), (viewport.top() + viewport.height() * 0.5) - (h * 0.5));
	QSizeF s(w, h);

	sliderStart = QRectF(p1, s);
	sliderEnd = QRectF(p2, s);

}
//-----------------------------------------------------------------------------
void VizCHorizontalScale::paintEvent(QPaintEvent* event)
{
	const QRect& dummyGeometry = Dummy->geometry();

	QPainter painter(this);
	painter.setFont(Font);
	painter.setPen(Color);
	painter.fillRect(QRect(0, 0, dummyGeometry.width(), dummyGeometry.height()), BackGroundColor);
	painter.drawLine(QPoint(dummyGeometry.left(), dummyGeometry.top()), QPoint(dummyGeometry.right(), dummyGeometry.top()));

	drawHorizontalValues(&painter);
	drawChart(&painter);
	drawSliders(&painter);

	QPalette pal = palette();
	QColor windowColor = pal.color(QPalette::Window);
	painter.fillRect(QRect(dummyGeometry.right(), 0, 46, dummyGeometry.height()), windowColor);
	//painter.setPen(Color);
	//painter.drawLine(QPoint(dummyGeometry.right() + 1, dummyGeometry.top()), QPoint(dummyGeometry.right() + 1, dummyGeometry.bottom()));

	if(IsSelected)
	{
		QPalette pal = this->palette();
		QColor titleBackgroundColor1 = BackgroundColorHighlight1;
		QColor titleBackgroundColor2 = BackgroundColorHighlight2;
		titleBackgroundColor1.setAlphaF(0.5);

		QLinearGradient gradient(0, 0, 0, Dummy->geometry().height());
		gradient.setColorAt(0.0, titleBackgroundColor1);
		gradient.setColorAt(1.0, titleBackgroundColor2);
		painter.setBrush(gradient);
		painter.setPen(Qt::NoPen);
		painter.fillRect(QRect(0, 0, Dummy->geometry().width(), Dummy->geometry().height()), gradient);
	}


}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------

