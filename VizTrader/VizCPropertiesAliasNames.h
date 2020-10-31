//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCPROPERTIESALIASNAMES_H__
#define __VIZCPROPERTIESALIASNAMES_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include <QHash>
#include <QString>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCPropertiesAliasNames
{
public:
	VizCPropertiesAliasNames();
	static VizCPropertiesAliasNames* instance();
	QString propertyAliasName(const QString& propertyName);
private:
	static VizCPropertiesAliasNames* Instance;
	void createAliasPropertiesNames();
	void createAliasPropertiesName(const QString& name, const QString& alias);
	QHash<QString, QString> AliasPropertiesNames;
	QString findAliasPropertiesNames(const QString& name);
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
