//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCPropertiesAliasNames.h"
#include <QObject>
//-----------------------------------------------------------------------------
namespace viz
{
//-----------------------------------------------------------------------------
namespace gui
{
//-----------------------------------------------------------------------------
VizCPropertiesAliasNames* VizCPropertiesAliasNames::Instance = 0;
//-----------------------------------------------------------------------------
VizCPropertiesAliasNames* VizCPropertiesAliasNames::instance()
{
	if(!Instance)
	{
		Instance = new VizCPropertiesAliasNames;
	}
	return Instance;
}
//-----------------------------------------------------------------------------
VizCPropertiesAliasNames::VizCPropertiesAliasNames()
{
	createAliasPropertiesNames();
}
//-----------------------------------------------------------------------------
void VizCPropertiesAliasNames::createAliasPropertiesName(const QString& name, const QString& alias)
{
	AliasPropertiesNames[name] = alias;
	AliasPropertiesNames[alias] = name;
}
//-----------------------------------------------------------------------------
void VizCPropertiesAliasNames::createAliasPropertiesNames()
{
	createAliasPropertiesName("backGroundColor", QObject::tr("Cor de Fundo"));
	createAliasPropertiesName("penWidth", QObject::tr("Largura da Linha"));
	createAliasPropertiesName("scaleInYOptions", QObject::tr("Escala Y"));
	createAliasPropertiesName("color", QObject::tr("Cor"));
	createAliasPropertiesName("upColor", QObject::tr("Cor Ascendente"));
	createAliasPropertiesName("downColor", QObject::tr("Cor Descendente"));
	createAliasPropertiesName("timePeriod", QObject::tr("Periodo"));
	createAliasPropertiesName("indicatorStyle", QObject::tr("Estilo"));
	createAliasPropertiesName("priceField", QObject::tr("Preço"));
	createAliasPropertiesName("movingAverageMethod", QObject::tr("Método"));
	createAliasPropertiesName("indicatorStyle", QObject::tr("Estilo"));
	createAliasPropertiesName("volumeField", QObject::tr("Volume"));
	
	createAliasPropertiesName("standardDeviationUp", QObject::tr("Desvio Padrão Up"));
	createAliasPropertiesName("standardDeviationDown", QObject::tr("Desvio Padrão Down"));
	createAliasPropertiesName("standardDeviation", QObject::tr("Desvio Padrão"));


	createAliasPropertiesName("firstDateTime", QObject::tr("Primeira Data"));
	createAliasPropertiesName("lastDateTime", QObject::tr("Última Data"));
	createAliasPropertiesName("rightMargin", QObject::tr("Margem Direita"));
	createAliasPropertiesName("leftMargin", QObject::tr("Margem Esquerda"));
	createAliasPropertiesName("semiLogarithmicScale", QObject::tr("Semi-Logarítima"));
	createAliasPropertiesName("invertScale", QObject::tr("Inverter"));
	createAliasPropertiesName("autoScale", QObject::tr("Automático"));
	createAliasPropertiesName("maximum", QObject::tr("Máximo"));
	createAliasPropertiesName("minimum", QObject::tr("Mínimo"));
	createAliasPropertiesName("font", QObject::tr("Fonte"));
	createAliasPropertiesName("border", QObject::tr("Borda"));
	createAliasPropertiesName("periodicity", QObject::tr("Periodicidade"));

	createAliasPropertiesName("gridVisible", QObject::tr("Grade Visível"));
	createAliasPropertiesName("gridColor", QObject::tr("Cor da Grade"));
	createAliasPropertiesName("gridPenWidth", QObject::tr("Largura da Grade"));
	createAliasPropertiesName("gridPenStyle", QObject::tr("Estilo do Grade"));
	
	createAliasPropertiesName("lastTradeVisible", QObject::tr("Último Período Visível"));
	createAliasPropertiesName("penStyle", QObject::tr("Estilo da Linha"));
	createAliasPropertiesName("value", QObject::tr("Valor"));

	createAliasPropertiesName("horizontalShift", QObject::tr("Deslocamento Horizontal"));
	createAliasPropertiesName("verticalShift", QObject::tr("Deslocamento Vertical %"));

	createAliasPropertiesName("titleBarVisible", QObject::tr("Título Visível"));
	createAliasPropertiesName("verticalScaleVisible", QObject::tr("Escala Y Visível"));


	createAliasPropertiesName("startValue", QObject::tr("Valor Inicial"));
	createAliasPropertiesName("endValue", QObject::tr("Valor Final"));

	createAliasPropertiesName("startDate", QObject::tr("Data Inicial"));
	createAliasPropertiesName("endDate", QObject::tr("Data Final"));

	createAliasPropertiesName("leftExtention", QObject::tr("Extensão Esquerda"));
	createAliasPropertiesName("rightExtention", QObject::tr("Extensão Direita"));
	createAliasPropertiesName("snapToPrice", QObject::tr("Prender no Preço"));


	createAliasPropertiesName("showValuesInTheTitleBar", QObject::tr("Mostrar Valores no Título"));
	createAliasPropertiesName("useColorOfPlotInValuesOfTitleBar", QObject::tr("Usar Cores do Desenho nos Valores"));

}
//-----------------------------------------------------------------------------
QString VizCPropertiesAliasNames::propertyAliasName(const QString& propertyName)
{
	if(AliasPropertiesNames.contains(propertyName))
	{
		return AliasPropertiesNames[propertyName];
	}
	return propertyName;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------

