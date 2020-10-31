//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCMDISUBWINDOW_H__
#define __VIZCMDISUBWINDOW_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include "VizCDataFacade.h"
#include <QMdiSubWindow>
#include <QTabWidget>
#include <QMdiArea>
#include <QFrame>
#include <QMenu>
//-----------------------------------------------------------------------------
namespace viz
{
//-----------------------------------------------------------------------------
namespace gui
{
//-----------------------------------------------------------------------------
class VizCWorkArea;
class VizCGraphicsPanel;
class VizCOpenSecurityWidget;
class VizCOpenIndicatorWidget;
class VizIChart;
//-----------------------------------------------------------------------------
class VizCMdiSubWindow : public QMdiSubWindow
{
	Q_OBJECT

public:
	VizCMdiSubWindow(VizCWorkArea* workArea);
	~VizCMdiSubWindow();

	void loadProject(const QString& securitySymbol);
	void saveProject();

private slots:
	void closeCurrentTab();
	void currentChanged(int index);
	void menuOpen();

	void addPrimaryChart();
	void addSecundaryChartInSelectedPanel();
	void addSecundaryChartInPrimaryPanel();
	void addSecundaryChartInNewPanel();
	
	void addIndicatorBasedOnPrimaryChartInSelectedPanel();
	void addIndicatorBasedOnPrimaryChartInPrimaryPanel();
	void addIndicatorBasedOnPrimaryChartInNewPanel();

	void addIndicatorBasedOnSelectedChartInSelectedPanel();
	void addIndicatorBasedOnSelectedChartInPrimaryPanel();
	void addIndicatorBasedOnSelectedChartInNewPanel();
	
	void addSimplifiedOfferBook();
	void addFullOfferBook();

protected:
	virtual void closeEvent(QCloseEvent *event);

private:
	QString SecuritySymbolBase;
	QToolButton* NewTabButton;
	QToolButton* CloseTabButton;
	QTabWidget* TabWidget;
	VizCWorkArea* WorkArea;
	VizCOpenSecurityWidget* OpenSecurityWidget;
	VizCOpenIndicatorWidget* OpenIndicatorWidget;
	io::VizCDataFacade* DataFacade;

	VizIChart* SelectedChart;
	VizIChart* PrimaryChart;
	VizCGraphicsPanel* SelectedPanel;
	VizCGraphicsPanel* PrimaryPanel;

	QMenu* AddMenu;

	QMenu* MenuAddChart;
	QMenu* MenuAddSecondaryChart;

	QMenu* MenuAddIndicator;
	QMenu* MenuAddIndicatorPrimaryChart;
	QMenu* MenuAddIndicatorSelectedChart;

	QMenu* MenuAddOfferBook;

	QAction* AddPrimaryChart;
	QAction* AddSecundaryChartInSelectedPanel;
	QAction* AddSecundaryChartInPrimaryPanel;
	QAction* AddSecundaryChartInNewPanel;
	QAction* AddIndicatorBasedOnPrimaryChartInSelectedPanel;
	QAction* AddIndicatorBasedOnPrimaryChartInPrimaryPanel;
	QAction* AddIndicatorBasedOnPrimaryChartInNewPanel;
	QAction* AddIndicatorBasedOnSelectedChartInSelectedPanel;
	QAction* AddIndicatorBasedOnSelectedChartInPrimaryPanel;
	QAction* AddIndicatorBasedOnSelectedChartInNewPanel;
	QAction* AddSimplifiedOfferBook;
	QAction* AddFullOfferBook;

	s32 addTab(const QString& label, QWidget* widget);
	void removeTab(s32 index);
	void setCurretTab(s32 index);
	s32 currentTab() const;
	QWidget* tabWidget(s32 index) const;

	void findPanelsAndChart();
	bool anyItenIsSelected(s32 tabIndex = -1) const;
	void menuUpdate();
	void loadDefaultProject();
	bool loadSecurity(const QString& securitySymbol);
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
