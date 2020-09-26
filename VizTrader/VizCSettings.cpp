//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCSettings.h"
#include "VizCApplicationData.h"
#include "VizGUIUtils.h"
#include <QDebug>
#include <QApplication>
//-----------------------------------------------------------------------------
namespace viz
{
namespace io
{
//-----------------------------------------------------------------------------
io::VizCSettings* VizCSettings::Instance = 0;
//-----------------------------------------------------------------------------
VizCSettings::VizCSettings()
{
	system::VizSVersionNumber vn = system::VizCApplicationData::instance()->applicationVersion();
	ApplicationVersion = QString("%1.%2").arg(vn.Major).arg(vn.Minor);

	QString organization = QCoreApplication::organizationName();
	QString application = QCoreApplication::applicationName();
	Settings = new QSettings(organization, application);

	loadApplicationSettings();
}
//-----------------------------------------------------------------------------
VizCSettings::~VizCSettings()
{
}
//-----------------------------------------------------------------------------
io::VizCSettings* io::VizCSettings::instance()
{
	if(!Instance)
	{
		Instance = new VizCSettings;
	}
	return Instance;
}
//-----------------------------------------------------------------------------
QVariant VizCSettings::value(const QString& key, const QVariant& dafaultValue)
{
	QVariant v = Settings->value(ApplicationVersion + "/" + key, dafaultValue);
	//Q_ASSERT(!v.isNull());
	//return v;
	//QVariant v = Dictionary.value(key, dafaultValue);
	//Q_ASSERT(!v.isNull());
	return v;
}
//-----------------------------------------------------------------------------
void VizCSettings::setValue(const QString& key, const QVariant& value)
{
	//qDebug() << value << " " << value.isValid();
	//Dictionary[key] = value;
	Settings->setValue(ApplicationVersion + "/" + key, value);
}
//-----------------------------------------------------------------------------
void VizCSettings::loadSetting(const QString& key, const QVariant& defaultValue)
{
	setValue(key, Settings->value(ApplicationVersion + "/" + key, defaultValue));
}
//-----------------------------------------------------------------------------
void VizCSettings::loadApplicationSettings()
{
	loadSetting("DefaultStyle", gui::styleValueText(QApplication::style()));
	loadSetting("DefaultFont", QApplication::font());
	//loadSetting("InstalledVersion", ApplicationVersion);

	loadSetting("General/Workspace/RestoreWorkspaceOnStartup", false);
	loadSetting("General/Edit/ConfirmDeletionOfItens", true);
	loadSetting("General/Edit/ReturnToSelectModeAfterAddedItem", true);
	loadSetting("General/Edit/SnapToPriceAfterAddedItem", false);
	loadSetting("General/Edit/AddItemAsAChildOfTheNearestItem", true);

	loadSetting("Appearance/System/Style", gui::styleValueText(QApplication::style()));
	loadSetting("Appearance/DefaultFontAndColor/Font", this->value("DefaultFont"));
	loadSetting("Appearance/Items/Font", this->value("DefaultFont"));
	loadSetting("Appearance/DefaultFontAndColor/ForegroundColor", QColor(Qt::black));
	loadSetting("Appearance/DefaultFontAndColor/BackgroundColor", QColor(Qt::white));
	loadSetting("Appearance/DefaultFontAndColor/GridColor", gui::VizGray);
	loadSetting("Appearance/Charts/UpColor", gui::VizGreen);
	loadSetting("Appearance/Charts/DownColor", gui::VizRed);
	loadSetting("Appearance/Indicators/Color", gui::VizRed);
	loadSetting("Appearance/Items/Color", gui::VizBlue);
	loadSetting("Appearance/DefaultFontAndColor/GridStyle", 2);
	loadSetting("Appearance/DefaultFontAndColor/GridWidth", 1);
	loadSetting("Appearance/Charts/LineWidth", 1);
	loadSetting("Appearance/Indicators/Style", 0);
	loadSetting("Appearance/Indicators/LineStyle", 1);
	loadSetting("Appearance/Indicators/LineWidth", 1);
	loadSetting("Appearance/Items/LineStyle", 1);
	loadSetting("Appearance/Items/LineWidth", 1);

	loadSetting("Performance/Render/AntiAliasing", true);
	loadSetting("Performance/Render/UseGradientToFillShapes", true);
	loadSetting("Performance/Render/MaxPercentOfMemoryToCache", 25);
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
