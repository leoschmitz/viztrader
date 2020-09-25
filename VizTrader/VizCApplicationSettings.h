//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCAPPLICATIONSETTINGS_H__
#define __VIZCAPPLICATIONSETTINGS_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include "ui_VizCApplicationSettings.h"
#include "VizCSettings.h"
#include <QKeyEvent>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCApplicationSettings: public QWidget, private Ui::UIVizCApplicationSettings
{
	Q_OBJECT

public:
	static VizCApplicationSettings* instance();
	void load();
	void save();

public slots:
	void loadAndShow();

signals:
	void settingsChange();

protected:
	virtual void keyPressEvent(QKeyEvent* event);


private:
	VizCApplicationSettings(QWidget* parent = 0);
	~VizCApplicationSettings();
	static VizCApplicationSettings* Instance;
	
	io::VizCSettings* Settings;
	QButtonGroup* AppearanceButtonGroup;

	QStyle* Style;
	QColor Color[7];
	QFont Font[2];

	u64 TotalSystemMemory;

	void createGeneralTab();
	void loadGeneralTab();
	void saveGeneralTab();
	void setToDefaultGeneralTab();

	void createAppearanceTab();
	void loadAppearanceTab();
	void saveAppearanceTab();
	void setToDefaultAppearanceTab();

	void createPerformanceTab();
	void loadPerformanceTab();
	void savePerformanceTab();
	void setToDefaultPerformanceTab();

	void createNetworkTab();
	void loadNetworkTab();
	void saveNetworkTab();
	void setToDefaultNetworkTab();
	
private slots:
	void slotResetToDefault();
	void slotApply();
	void slotCancel();
	void slotOk();

	void Appearance_PushButton_Clicked(QAbstractButton * button);
	void slotMemoryPercentValueChanged(int i);

};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
