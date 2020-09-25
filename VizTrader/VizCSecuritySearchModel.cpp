//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCSecuritySearchModel.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
const QString NameOfTheFileWhichHasCompleteList = "trades.vzl";
const QString VersionOfTheFileWhichHasCompleteList = "0.0.5";
//-----------------------------------------------------------------------------
inline quint16 listCheckSum(const QString& symbol, const QString& wholeName) 
{
	QString getCheckSum = symbol + wholeName;
	return qChecksum(getCheckSum.toLatin1(), getCheckSum.size());
}
//-----------------------------------------------------------------------------
VizCSecuritySearchModel::VizCSecuritySearchModel(QObject * parent) : QAbstractTableModel(parent) 
{
	initialize();
}
//-----------------------------------------------------------------------------
void VizCSecuritySearchModel::startSyncNegotiableList()
{
	VizTP->requestNegotiableList(SyncTimeStamp);
}
//-----------------------------------------------------------------------------
void VizCSecuritySearchModel::updateNegotiableList()
{
	int rows = rowCount();
	for (int i = 0; i < VizTP->NegotiableListElementsToAdd.size(); i++)
	{
		if (!InDiskNegotiableList.contains(VizTP->NegotiableListElementsToAdd[i].at(0)))
			appendSecurityInfo(VizTP->NegotiableListElementsToAdd[i]);
	}

	for (int i = 0; i < VizTP->NegotiableListElementsToRemove.size(); i++)
	{ 
		if (!InDiskNegotiableList.contains(VizTP->NegotiableListElementsToRemove[i]))
			removeSecurityInfo(VizTP->NegotiableListElementsToRemove[i]);
	}

	emit layoutChanged();

	VizTP->NegotiableListElementsToAdd.clear();
	VizTP->NegotiableListElementsToRemove.clear();

	saveSecurityList();
}
//-----------------------------------------------------------------------------
bool VizCSecuritySearchModel::saveSecurityList()
{
	//////////////////////////////////////////////////////////////////////////
	// Formato do Arquivo
	// ==================
	// (QString) VizList
	// (QString) Version
	// (QDate) Ultima Atualização
	// (QString) SimboloCompleto
	// (QString) NomeCompleto
	// (QString) Peridiocidade
	// (QString) Isodate Start
	// (QString) Isodate end
	// (quint16) crc
	// ...
	// 
	//////////////////////////////////////////////////////////////////////////
	io::VizCFile::setDataDirectory();

	QFile file(NameOfTheFileWhichHasCompleteList);

	if (!file.open(QIODevice::WriteOnly))
		return false;

	QDataStream fileStream(&file);

	QString vizList = "VizList";

	fileStream << vizList;
	fileStream << VersionOfTheFileWhichHasCompleteList;
	fileStream << QDate::currentDate();
	for (int i = 0; i < rowCount(); i++)
	{
		fileStream 
			<< Content[i].at(0) 
			<< Content[i].at(1) 
			<< Content[i].at(2) 
			<< Content[i].at(3) 
			<< Content[i].at(4) 
			<< listCheckSum(Content[i].at(0), Content[i].at(1));
	}

	file.close();

	return true;	
}

//-----------------------------------------------------------------------------
bool VizCSecuritySearchModel::loadSecurityList()
{
	io::VizCFile::setDataDirectory();

	QFile file(NameOfTheFileWhichHasCompleteList);

	if (!file.exists())
	{
		startSyncNegotiableList();
		return false;
	}

	if (!file.open(QIODevice::ReadOnly))
		return false;

	QByteArray fileWithNames = file.readAll();

	file.close();

	quint16 crc(0);
	QString vizList, version, symbol, wholeName, peridiocity, dateStart, dateEnd;

	QDataStream fileStream(&fileWithNames, QIODevice::ReadOnly);

	fileStream 
		>> vizList 
		>> version
		>> SyncTimeStamp;
	if ((vizList != "VizList") ||(version != VersionOfTheFileWhichHasCompleteList))
	{
		if (!file.setPermissions(QFile::WriteUser))
			return false;
		file.remove();
		startSyncNegotiableList();
		return false;
	}

	QVector<QStringList> container;
	while(!fileStream.atEnd())
	{
		fileStream >> symbol >> wholeName >> peridiocity >> dateStart >> dateEnd >> crc;
		if(crc != listCheckSum(symbol, wholeName))
		{
			return false;
		}
		if (!InDiskNegotiableList.contains(symbol))
			container.push_back(QStringList() << symbol << wholeName << peridiocity << dateStart << dateEnd);
	}

	appendSecurityInfoComplete(container);

	return true;
}
//-----------------------------------------------------------------------------
QVariant VizCSecuritySearchModel::data(const QModelIndex &index, int role) const
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
QVariant VizCSecuritySearchModel::headerData(int section ,
								Qt::Orientation orientation,
								int role) const
{
	if ((role != Qt::DisplayRole) && (role != Qt::Horizontal)) 
		return QVariant();

	switch (section) 
	{
	case 0:
		return QString(tr("Símbolo"));
		break;
	case 1:
		return QString(tr("Nome"));
		break;
	case 2:
		return QString(tr("Peridiocidade"));
		break;
	case 3:
		return QString(tr("Data Inicial"));
		break;
	case 4:
		return QString(tr("Data Final"));
		break;
	case 5:
		return QString(tr("Offline"));
		break;
	}

	return QVariant();
}
//-----------------------------------------------------------------------------
int VizCSecuritySearchModel::rowCount(const QModelIndex & /* parent */) const
{
	return Content.size();
}
//-----------------------------------------------------------------------------
int VizCSecuritySearchModel::columnCount(const QModelIndex & /* parent */) const
{
	return 6;
}
//-----------------------------------------------------------------------------
void VizCSecuritySearchModel::addLocalFilesInNegotiableList()
{
	QVector<QStringList> fileList;

	DataFacade->loadDiskNegotiableList(fileList);

	for (int i = 0; i < fileList.size(); i++)
	{
		appendSecurityInfo(fileList[i]);

		InDiskNegotiableList.insert(fileList[i].at(0));
	}

}
//-----------------------------------------------------------------------------
//bool VizCSecuritySearchModel::removeSecurityInfo(QPair<QString, QString>& symbolToName)
bool VizCSecuritySearchModel::removeSecurityInfo(const QString& symbol)
{

//  	if (!Content.contains(symbol))
// 		return false;
// 
// 	Content.remove()

	return false;
// 
// 	takeRow(SymbolToRow.value(symbolToName.first));
// 	
// 	SymbolToRow.remove(symbolToName.first);
// 
// 	return true;
	//return false;
}
//-----------------------------------------------------------------------------
void VizCSecuritySearchModel::appendSecurityInfoComplete(QVector<QStringList>& completeList)
{
	foreach (QStringList list, completeList)
		appendSecurityInfo(list);
}
//-----------------------------------------------------------------------------
void VizCSecuritySearchModel::appendSecurityInfo(QStringList& list)
{
	list << ((DataFacade->isLocal(list.at(0))) ? QString("Sim") : QString(""));
	
	Content.append(list);
}
//-----------------------------------------------------------------------------
VizCSecuritySearchModel::~VizCSecuritySearchModel()
{

}
//-----------------------------------------------------------------------------
void VizCSecuritySearchModel::initialize()
{
	VizTP = net::VizCTP::instance();
	DataFacade = io::VizCDataFacade::instance();

	connect(VizTP, SIGNAL(connected()), this, SLOT(startSyncNegotiableList()));
	connect(VizTP, SIGNAL(negotiableListReady()), this, SLOT(updateNegotiableList()));

	addLocalFilesInNegotiableList();

	//loadSecurityList();
}	
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
