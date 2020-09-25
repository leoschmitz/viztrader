//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZOBJECT_H__
#define __VIZOBJECT_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
inline bool isVizObject(const QObject* obj)
{
	if(!obj) return false;

	const QMetaObject* mo = obj->metaObject();
	bool b1 = obj->property("name").isValid();
	bool b2 = mo->indexOfProperty("selected") != -1;
	//bool b3 = mo->indexOfMethod("resetToDefaults") != -1;
	return b1 && b2;
}
//-----------------------------------------------------------------------------
inline bool vizObjectResetToDefaults(QObject* obj)
{
	bool ok = QMetaObject::invokeMethod(obj, "resetToDefaults");
	return ok;
}
//-----------------------------------------------------------------------------
inline void vizObjectGetAllPropertiesNamesAndValues(const QObject* obj, QStringList& names, QVariantList& values)
{
	names.clear();
	values.clear();

	const QMetaObject* metaobject = obj->metaObject();
	for(int i = 0; i < metaobject->propertyCount(); ++i)
	{
		const QMetaProperty& metaproperty = metaobject->property(i);
		if(!metaproperty.isUser())
		{
			continue;
		}
		const char* property_name = metaproperty.name();
		names << property_name;
		QVariant value = metaproperty.read(obj);
		values << value;
	}
}
//-----------------------------------------------------------------------------
inline void vizObjectSetName(QObject* obj, const QString& name)
{
	bool b = obj->setProperty("name", name);
	//Q_ASSERT(b);
}
//-----------------------------------------------------------------------------
inline QString vizObjectName(const QObject* obj)
{
	QVariant v = obj->property("name");
	Q_ASSERT(v.isValid());
	return v.toString();
}
//-----------------------------------------------------------------------------
inline void vizObjectSetSelected(QObject* obj, bool value)
{
	bool b = obj->setProperty("selected", value);
	Q_ASSERT(b);
}
//-----------------------------------------------------------------------------
inline bool vizObjectIsSelected(const QObject* obj)
{
	QVariant v = obj->property("selected");
	Q_ASSERT(v.isValid());
	return v.toBool();
}
//-----------------------------------------------------------------------------
inline void vizObjectBlockSignals(QObject* obj, bool b)
{
	if (obj)
	{
		obj->blockSignals(b);
		const QObjectList& list = obj->children();
		foreach(QObject* o, list)
		{
			if (o != obj) vizObjectBlockSignals(o, b);
		}
	}
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
