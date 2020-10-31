//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCPropertyEditor.h"
#include "VizMath.h"
#include "VizObjectUtils.h"
#include "VizIGraphicsItem.h"
#include <QVBoxLayout>
#include <QStringList>
#include <QtVariantEditorFactory>
#include <QtVariantProperty>
#include <QMetaObject>
#include <QMetaProperty>
#include <QMap>
#include <QIcon>
#include <QStyle>
#include <QDebug>
#include <QPropertyAnimation>
//-----------------------------------------------------------------------------
namespace viz
{
//-----------------------------------------------------------------------------
namespace gui
{
//-----------------------------------------------------------------------------
/*class VizCFadeAnimation : public QPropertyAnimation
{
public:
	VizCFadeAnimation(QObject* target, const QByteArray& propertyNameToChange, const QVariant& valueToChange, QObject* parent = 0) : QPropertyAnimation(target, "visibility", parent)
	{
		PropertyNameToChange = propertyNameToChange;
		ValueToChange = valueToChange;
		Duration = 400;
		ChangeProperty = false;

		setDuration(Duration);
		setStartValue(1.0);
		setEndValue(0.0);
		setEasingCurve(QEasingCurve::SineCurve);
	}
	virtual ~VizCFadeAnimation()
	{
	}

protected:
	virtual void updateCurrentValue(const QVariant& value)
	{

		if(currentTime() >= Duration * 0.5 && !ChangeProperty)
		{
			ChangeProperty = true;
			bool ok = false;
			ok = targetObject()->setProperty(PropertyNameToChange, ValueToChange);
		}

		QPropertyAnimation::updateCurrentValue(value);

	}
private:
	bool ChangeProperty;
	s32 Duration;
	QByteArray PropertyNameToChange;
	QVariant ValueToChange;

};*/
//-----------------------------------------------------------------------------
const s32 TimePeriodMin = 2;
const s32 TimePeriodMax = 100000;
const f64 MinimumY = -99999999999999999999.0;
const f64 MaximumY = +99999999999999999999.0;
//-----------------------------------------------------------------------------
VizCPropertyEditor::VizCPropertyEditor() : VariantManager(0), VariantFactory(0), TreePropertyBrowser(0), Label(0), Object(0)
{
	TreePropertyBrowser = new QtTreePropertyBrowser(this);
	
	PropertiesAliasNames = VizCPropertiesAliasNames::instance();

	VariantManager = new QtVariantPropertyManager(TreePropertyBrowser);
	VariantFactory = new QtVariantEditorFactory(TreePropertyBrowser);
	TreePropertyBrowser->setFactoryForManager(VariantManager, VariantFactory);
	TreePropertyBrowser->setRootIsDecorated(true);
	TreePropertyBrowser->setResizeMode(QtTreePropertyBrowser::Interactive);
	
	Label = new QLabel(this);
	Label->setText(tr(""));

	ToolButton = new QToolButton(this);
	ToolButton->setAutoRaise(true);
	ToolButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
	ToolButton->setText(tr("Opções"));
	

	QVBoxLayout* layoutGeral = new QVBoxLayout;
	QHBoxLayout* layoutHeader = new QHBoxLayout;
	layoutHeader->addWidget(Label);
	layoutHeader->addSpacing(1);
	layoutHeader->addWidget(ToolButton);
	layoutHeader->setContentsMargins(5,0,0,0);
	layoutHeader->setSpacing(0);

	layoutGeral->addLayout(layoutHeader);
	layoutGeral->addWidget(TreePropertyBrowser);
	layoutGeral->setMargin(0);
	layoutGeral->setSpacing(0);

	setLayout(layoutGeral);

	createMenu();

	TreePropertyBrowser->setFocusPolicy(Qt::NoFocus);
	Label->setFocusPolicy(Qt::NoFocus);
	ToolButton->setFocusPolicy(Qt::NoFocus);
	setFocusPolicy(Qt::NoFocus);


}
//-----------------------------------------------------------------------------
VizCPropertyEditor::~VizCPropertyEditor()
{
}
//-----------------------------------------------------------------------------
void VizCPropertyEditor::slotResetToDefaultAction()
{
	if(Object)
	{
		vizObjectResetToDefaults(Object);
		updateAllProperties();
	}

}
//-----------------------------------------------------------------------------
void VizCPropertyEditor::createMenu()
{
	ResetToDefaultAction = new QAction(tr("&Valores Padrão"), this);
	ResetToDefaultAction->setStatusTip(tr("Restaura os valores padrão para este ítem"));

	connect(ResetToDefaultAction, SIGNAL(triggered()), this, SLOT(slotResetToDefaultAction()));
	
	Menu = new QMenu();
	Menu->addAction(ResetToDefaultAction);
	ToolButton->setPopupMode(QToolButton::InstantPopup);
	ToolButton->setMenu(Menu);

	ToolButton->setEnabled(false);

}
//-----------------------------------------------------------------------------
void VizCPropertyEditor::variantValueChanged(QtProperty* property, const QVariant& value)
{
	
	Q_ASSERT(Object);
	QString name = PropertiesAliasNames->propertyAliasName(property->propertyName());
	bool ok = false;
	ok = Object->setProperty(qPrintable(name), value);
	//Q_ASSERT(ok);
	

	/*Q_ASSERT(Object);
	QString name = PropertiesAliasNames->propertyAliasName(property->propertyName());
	{

		QVariant start_value = Object->property(qPrintable(name));
		QVariant end_value = value;

		bool isAnimable = start_value.type() == QVariant::Double;

		VizIGraphicsItem* item = qobject_cast<VizIGraphicsItem*>(Object);
		if(item)
		{
			VizCFadeAnimation* animation = new VizCFadeAnimation(item, qPrintable(name), value);
			animation->start();
		}
		else
		{
			if(isAnimable)
			{
				QPropertyAnimation* animation = new QPropertyAnimation(Object, qPrintable(name));
				animation->setDuration(500);
				animation->setStartValue(start_value);
				animation->setEndValue(end_value);
				animation->setEasingCurve(QEasingCurve::InOutCubic);
				animation->start();
			}
			else
			{
				bool ok = false;
				ok = Object->setProperty(qPrintable(name), value);
			}
			
		}
	}*/


}
//-----------------------------------------------------------------------------
bool VizCPropertyEditor::propertyLessThan(const QMetaProperty& p1, const QMetaProperty& p2)
{
	VizCPropertiesAliasNames* instance = VizCPropertiesAliasNames::instance();

	QString name1 = instance->propertyAliasName(p1.name());
	QString name2 = instance->propertyAliasName(p2.name());
	return name1.toLower() < name2.toLower();
}
//-----------------------------------------------------------------------------
QVector<QMetaProperty> VizCPropertyEditor::sortProperties(const QMetaObject* metaobject)
{
	QVector<QMetaProperty> sorted;

	int count = metaobject->propertyCount();
	for(int i = 0; i < count; ++i)
	{
		QMetaProperty metaproperty = metaobject->property(i);
		sorted.push_back(metaproperty);
	}

	qSort(sorted.begin(), sorted.end(), propertyLessThan);


	return sorted;
}
//-----------------------------------------------------------------------------
void VizCPropertyEditor::updateAllProperties()
{
	disconnect(VariantManager, SIGNAL(valueChanged(QtProperty*, const QVariant &)), this, SLOT(variantValueChanged(QtProperty *, const QVariant &)));
	TreePropertyBrowser->clear();

	const QMetaObject* metaobject = Object->metaObject();
	QString obj_name = vizObjectName(Object);
	Label->setText(obj_name);

	QVector<QMetaProperty> SortedProperties = sortProperties(metaobject);
	int count = SortedProperties.size();
	for(int i = 0; i < count; ++i)
	{
		const QMetaProperty& metaproperty = SortedProperties[i];
		if(!metaproperty.isUser())
		{
			continue;
		}
		QtVariantProperty* property = 0;
		const char* property_name = metaproperty.name();
		QString propertyUniqueName = hash(Object, property_name);
		if(Properties.contains(propertyUniqueName))
		{
			property = Properties[propertyUniqueName];
			Q_ASSERT(property);

			//update property
			propertyChanged(Object, property_name);
		}
		QtBrowserItem* item = TreePropertyBrowser->addProperty(property);
	}

	connect(VariantManager, SIGNAL(valueChanged(QtProperty*, const QVariant &)), this, SLOT(variantValueChanged(QtProperty *, const QVariant &)));

}
//-----------------------------------------------------------------------------
void VizCPropertyEditor::setObject(QObject* object)
{

	disconnect(VariantManager, SIGNAL(valueChanged(QtProperty*, const QVariant &)), this, SLOT(variantValueChanged(QtProperty *, const QVariant &)));
	TreePropertyBrowser->clear();

	if(!object)
	{
		Object = 0;
		Label->setText(tr(""));
		ToolButton->setEnabled(false);
		parentWidget()->setWindowTitle(tr("Propriedades"));
	}
	else
	{
		Object = object;
		ToolButton->setEnabled(true);
		
		const QMetaObject* metaobject = Object->metaObject();
		QString obj_name = vizObjectName(Object);

		Label->setText(obj_name);

		QVector<QMetaProperty> SortedProperties = sortProperties(metaobject);
		int count = SortedProperties.size();
		for(int i = 0; i < count; ++i)
		{
			const QMetaProperty& metaproperty = SortedProperties[i];
			if(!metaproperty.isUser())
			{
				continue;
			}
			QtVariantProperty* property = 0;
			const char* property_name = metaproperty.name();
			QString propertyUniqueName = hash(Object, property_name);
			if(Properties.contains(propertyUniqueName))
			{
				property = Properties[propertyUniqueName];
				Q_ASSERT(property);

				//update property
				propertyChanged(Object, property_name);
			}
			else
			{
				QVariant value = object->property(property_name);
				Q_ASSERT(value.isValid());
				property = createProperty(property_name, value);
				Q_ASSERT(property);
				Properties[propertyUniqueName] = property;
			}
			QtBrowserItem* item = TreePropertyBrowser->addProperty(property);
		}
	}

	connect(VariantManager, SIGNAL(valueChanged(QtProperty*, const QVariant &)), this, SLOT(variantValueChanged(QtProperty *, const QVariant &)));

}
//-----------------------------------------------------------------------------
QString VizCPropertyEditor::hash(const QObject* obj, const QString& propertyName)
{
	unsigned long id = reinterpret_cast<unsigned long>(obj);
	return QString("%1_%2").arg(id).arg(propertyName);
}
//-----------------------------------------------------------------------------
void VizCPropertyEditor::propertyChanged(const QObject* obj, const QString& propertyName)
{
	QString propertyUniqueName = hash(obj, propertyName);
	if(Properties.contains(propertyUniqueName))
	{
		disconnect(VariantManager, SIGNAL(valueChanged(QtProperty*, const QVariant &)), this, SLOT(variantValueChanged(QtProperty *, const QVariant &)));

		QVariant value = obj->property(qPrintable(propertyName));
		QtVariantProperty* p = Properties[propertyUniqueName];
		if(VariantManager->value(p) != value)
		{
			VariantManager->setValue(p, value);
		}

		connect(VariantManager, SIGNAL(valueChanged(QtProperty*, const QVariant &)), this, SLOT(variantValueChanged(QtProperty *, const QVariant &)));
	}

}
//-----------------------------------------------------------------------------
void VizCPropertyEditor::propertiesChanged(const QObject* obj, const QStringList& propertiesName)
{
	
	disconnect(VariantManager, SIGNAL(valueChanged(QtProperty*, const QVariant &)), this, SLOT(variantValueChanged(QtProperty *, const QVariant &)));
	foreach(QString propertyName, propertiesName)
	{
		QString propertyUniqueName = hash(obj, propertyName);
		if(Properties.contains(propertyUniqueName))
		{
			QVariant value = obj->property(qPrintable(propertyName));
			QtVariantProperty* p = Properties[propertyUniqueName];
			if(VariantManager->value(p) != value)
			{
				VariantManager->setValue(p, value);
			}
		}
	}
	connect(VariantManager, SIGNAL(valueChanged(QtProperty*, const QVariant &)), this, SLOT(variantValueChanged(QtProperty *, const QVariant &)));
}
//-----------------------------------------------------------------------------
QtVariantProperty* VizCPropertyEditor::createProperty(const QString& name, const QVariant& value)
{
	QString aliasName = PropertiesAliasNames->propertyAliasName(name);
	QtVariantProperty* p = 0;

	if(name == "timePeriod")
	{
		p = VariantManager->addProperty(QVariant::Int, aliasName);
		p->setToolTip(aliasName);
		s32 tp = value.value<s32>();
		p->setAttribute("minimum", TimePeriodMin);
		p->setAttribute("maximum", TimePeriodMax);
		p->setValue(tp);
	}
	else
	if(name == "penWidth")
	{
		p = VariantManager->addProperty(QtVariantPropertyManager::enumTypeId(), aliasName);
		QStringList types;
		types << tr("1 pixel")<< tr("2 pixel")<< tr("3 pixel")<< tr("4 pixel")<< tr("5 pixel");
		p->setAttribute("enumNames", types);
		p->setValue(value);
	}
	else
	if(name == "scaleInYOptions")
	{
		p = VariantManager->addProperty(QtVariantPropertyManager::enumTypeId(), aliasName);
		QStringList types;
		types << tr("Ajustada") << tr("Sobreposta");
		p->setAttribute("enumNames", types);
		s32 pw = value.value<s32>();
		p->setValue(pw);
	}
	else
	if(name == "indicatorStyle")
	{
		p = VariantManager->addProperty(QtVariantPropertyManager::enumTypeId(), aliasName);
		QStringList types;
		types << tr("Linhas")<< tr("Pontos")<< tr("Histograma");
		p->setAttribute("enumNames", types);
		s32 pw = value.value<s32>();
		p->setValue(pw);
	}
	else
	if(name == "priceField")
	{
		p = VariantManager->addProperty(QtVariantPropertyManager::enumTypeId(), aliasName);
		QStringList types;
		types << tr("Abertura")<< tr("Fechamento")<< tr("Máximo")<< tr("Mínimo");
		p->setAttribute("enumNames", types);
		s32 pw = value.value<s32>();
		p->setValue(pw);
	}
	else
	if(name == "movingAverageMethod")
	{
		p = VariantManager->addProperty(QtVariantPropertyManager::enumTypeId(), aliasName);
		QStringList types;
		types << tr("Simples")
			<< tr("Exponencial")
			<< tr("Com Pesos")
			<< tr("Duplamente Exponencial")
			<< tr("Triplamente Exponencial")
			<< tr("Triangular")
			<< tr("Kayfman")
			<< tr("MESA")
			<< tr("T3")
			;
		p->setAttribute("enumNames", types);
		s32 pw = value.value<s32>();
		p->setValue(pw);
	}
	else
	if(name == "volumeField")
	{
		p = VariantManager->addProperty(QtVariantPropertyManager::enumTypeId(), aliasName);
		QStringList types;
		types << tr("Venda") << tr("Compra");
		p->setAttribute("enumNames", types);
		s32 pw = value.value<s32>();
		p->setValue(pw);
	}
	else
	if(name == "border")
	{
		p = VariantManager->addProperty(QVariant::Double, aliasName);
		p->setToolTip(aliasName);
		f64 tp = value.value<f64>();
		p->setAttribute("minimum", 0.0);
		p->setAttribute("maximum", 100.0);
		p->setAttribute("singleStep", 1.0);
		p->setValue(tp);
	}
	else
	if(name == "periodicity")
	{
		p = VariantManager->addProperty(QtVariantPropertyManager::enumTypeId(), aliasName);
		QStringList types;
		types << tr("Diário")<< tr("Semanal")<< tr("Mensal")<< tr("Trimestral")<< tr("Anual");
		QtIconMap enumIcons;
		enumIcons[0] = QIcon(":/VizTrader/Resources/PeriodicityDaily.png");
		enumIcons[1] = QIcon(":/VizTrader/Resources/PeriodicityWeekly.png");
		enumIcons[2] = QIcon(":/VizTrader/Resources/PeriodicityMonthly.png");
		enumIcons[3] = QIcon(":/VizTrader/Resources/PeriodicityQuarterly.png");
		enumIcons[4] = QIcon(":/VizTrader/Resources/PeriodicityYearly.png");
		QVariant icons = qVariantFromValue<QtIconMap>(enumIcons);
		p->setAttribute("enumIcons", icons);
		p->setAttribute("enumNames", types);
		s32 pw = value.value<s32>();
		p->setValue(pw);
	}
	else
	if(name == "gridPenWidth")
	{
		p = VariantManager->addProperty(QtVariantPropertyManager::enumTypeId(), aliasName);
		QStringList types;
		types << tr("1 pixel")<< tr("2 pixel")<< tr("3 pixel")<< tr("4 pixel")<< tr("5 pixel");
		p->setAttribute("enumNames", types);
		p->setValue(value);
	}
	else
	if(name == "gridPenStyle" || name == "penStyle")
	{
		p = VariantManager->addProperty(QtVariantPropertyManager::enumTypeId(), aliasName);
		QStringList types;
		types << tr("Invisível")<< tr("Linha Sólida")<< tr("Traços")<< tr("Pontos")<< tr("Traço/Ponto")<< tr("Traço/Ponto/Ponto");
		p->setAttribute("enumNames", types);
		p->setValue(value);
	}
	else
	if(name == "rightMargin" || name == "leftMargin")
	{
		p = VariantManager->addProperty(QVariant::Int, aliasName);
		p->setToolTip(aliasName);
		f64 tp = value.value<f64>();
		p->setAttribute("minimum", 0);
		p->setAttribute("maximum", 100);
		p->setValue(tp);
	}
	else
	if(name == "horizontalShift")
	{
		p = VariantManager->addProperty(QVariant::Int, aliasName);
		p->setToolTip(aliasName);
		f64 tp = value.value<f64>();
		p->setAttribute("minimum", -1000);
		p->setAttribute("maximum", 1000);
		p->setValue(tp);
	}
	else
	if(name == "verticalShift")
	{
		p = VariantManager->addProperty(QVariant::Double, aliasName);
		p->setToolTip(aliasName);
		f64 tp = value.value<f64>();
		p->setAttribute("minimum", -100.0);
		p->setAttribute("maximum", 100.0);
		p->setAttribute("singleStep", 1.0);
		p->setValue(tp);
	}
	else
	if(value.type() == QVariant::Double)
	{
		p = VariantManager->addProperty(QVariant::Double, aliasName);
		p->setToolTip(aliasName);
		f64 tp = value.value<f64>();
		p->setAttribute("minimum", MinimumY);
		p->setAttribute("maximum", MaximumY);
		p->setAttribute("singleStep", 1.0);
		p->setValue(tp);
	}
	else
	{
		Q_ASSERT(VariantManager->isPropertyTypeSupported(value.type()));
		p = VariantManager->addProperty(value.type(), aliasName);
		p->setValue(value);

		QList<QtProperty *> ps = p->subProperties();
		foreach(QtProperty* pr, ps)
		{
			p->removeSubProperty(pr);
		}
		
	}



	return p;

}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------

