//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCSETTINGS_H__
#define __VIZCSETTINGS_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include <QSettings>
#include <QMap>
//-----------------------------------------------------------------------------
namespace viz
{
namespace io
{
//-----------------------------------------------------------------------------
class VizCSettings
{
public:
	static io::VizCSettings* instance();

	QVariant value(const QString& key, const QVariant& dafaultValue = QVariant());
	void setValue(const QString& key, const QVariant& value);

private:
	VizCSettings();
	~VizCSettings();
	static io::VizCSettings* Instance;
	QSettings* Settings;
	QString ApplicationVersion;
	void loadApplicationSettings();
	void loadSetting(const QString& key, const QVariant& defaultValue);
	QMap<QString, QVariant> Dictionary;
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
