//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCIndicatorSearchModel.h"
#include <QStringList>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
bool IndicatorRowLessThan(const QStringList &s1, const QStringList &s2)
{
	return s1.at(1).toLower() < s2.at(1).toLower();
}
//-----------------------------------------------------------------------------
VizCIndicatorSearchModel::VizCIndicatorSearchModel(QObject * parent) : QAbstractTableModel(parent)
{
	Content.append
	(
		QStringList()
		<< tr("MA")
		<< tr("Média Móvel")
		<< tr("Moving Average (MA)")
		<< tr("Media Movel")
		<< "viz::gui::VizCMovingAverageIndicator"
	);
	Content.append
	(
		QStringList()
		<< tr("VOL")
		<< tr("Volume")
		<< tr("Volume (vol)")
		<< tr("Volume")
		<< "viz::gui::VizCVolumeIndicator"
	);
	Content.append
	(
		QStringList()
		<< tr("-DI")
		<< tr("Indicador Direcional Negativo")
		<< tr("Negative Directional Indicator (-DI)")
		<< tr("Indicador Direcional Negativo")
		<< "viz::gui::VizCMinusDirectionalIndicator"
	);
	Content.append
	(
		QStringList()
		<< tr("+DI")
		<< tr("Indicador Direcional Positivo")
		<< tr("Positive Directional Indicator (-DI)")
		<< tr("Indicador Direcional Positivo")
		<< "viz::gui::VizCPlusDirectionalIndicator"
	);
	Content.append
	(
		QStringList()
		<< tr("ADX")
		<< tr("Índice Direcional Médio")
		<< tr("Average Directional Index (ADX)")
		<< tr("Indice Direcional Medio")
		<< "viz::gui::VizCAverageDirectionalMovementIndexIndicator"
	);

	Content.append
	(
		QStringList()
		<< tr("RSI")
		<< tr("Índice de Força Relativa")
		<< tr("Relative Strength Index (RSI)")
		<< tr("Indice de Forca Relativa")
		<< "viz::gui::VizCRelativeStrengthIndexIndicator"
	);

	Content.append
	(
		QStringList()
		<< tr("BB")
		<< tr("Bandas de Bollinger")
		<< tr("Bollinger Bands (BB)")
		<< tr("Bandas de Bollinger")
		<< "viz::gui::VizCBollingerBandsIndicator"
	);


	qSort(Content.begin(), Content.end(), IndicatorRowLessThan);
	
}
//-----------------------------------------------------------------------------
VizCIndicatorSearchModel::~VizCIndicatorSearchModel()
{

}
//-----------------------------------------------------------------------------
QVariant VizCIndicatorSearchModel::data(const QModelIndex &index, int role) const
{
	if (((!index.isValid()) 
		|| (index.row() > Content.size())) 
		|| (index.column() > columnCount()))
		return QVariant();

	switch (role)
	{
		case Qt::ToolTipRole:
		case Qt::DisplayRole:
		case Qt::EditRole:
		return Content[index.row()].at(index.column());
		break;
	}

	return QVariant();
}
//-----------------------------------------------------------------------------
QVariant VizCIndicatorSearchModel::headerData(int section ,
								Qt::Orientation orientation,
								int role) const
{
	if ((role != Qt::DisplayRole) && (role != Qt::Horizontal)) 
		return QVariant();

	switch (section) 
	{
	case 0:
		return QString("Sigla");
		break;
	case 1:
		return QString("Nome");
		break;
	case 2:
		return QString("Nome em Inglês");
		break;
	case 3:
		return QString("name pt-br clean");
		break;
	case 4:
		return QString("class name");
		break;
	}

	return QVariant();
}
//-----------------------------------------------------------------------------
int VizCIndicatorSearchModel::rowCount(const QModelIndex & /* parent */) const
{
	return Content.size();
}
//-----------------------------------------------------------------------------
int VizCIndicatorSearchModel::columnCount(const QModelIndex & /* parent */) const
{
	return 5;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
