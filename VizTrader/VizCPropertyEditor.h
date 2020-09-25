//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCPROPERTYEDITOR_H__
#define __VIZCPROPERTYEDITOR_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include "VizCPropertiesAliasNames.h"
#include <QLabel>
#include <QWidget>
#include <QAction>
#include <QMenu>
#include <QToolButton>
#include <QHash>
#include <QtTreePropertyBrowser>
#include <QtVariantPropertyManager>
#include <QStringList>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCPropertyEditor : public QWidget
{
	Q_OBJECT

public:
	VizCPropertyEditor();
	~VizCPropertyEditor();

	void setObject(QObject* object);
	void propertyChanged(const QObject* obj, const QString& propertyName);
	void propertiesChanged(const QObject* obj, const QStringList& propertiesName);

private slots:
	void variantValueChanged(QtProperty* property, const QVariant& value);
	void slotResetToDefaultAction();
	
private:
	static inline bool propertyLessThan(const QMetaProperty& p1, const QMetaProperty& p2);
	QString hash(const QObject* obj, const QString& propertyName);
	QtVariantProperty* createProperty(const QString& name, const QVariant& value);
	QVector<QMetaProperty> sortProperties(const QMetaObject* metaobject);
	void updateAllProperties();
	void createMenu();
	
	QObject* Object;
	VizCPropertiesAliasNames* PropertiesAliasNames;
	QAction* ResetToDefaultAction;
	QMenu *Menu;
	QLabel* Label;
	QToolButton* ToolButton;
	QtTreePropertyBrowser* TreePropertyBrowser;
	QtVariantPropertyManager* VariantManager;
	QtVariantEditorFactory* VariantFactory;

	QHash<QString, QtVariantProperty*> Properties;
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
