//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCSECURITYSEARCHMODEL_H__
#define __VIZCSECURITYSEARCHMODEL_H__
//-----------------------------------------------------------------------------
#include <QAbstractTableModel>

#include "VizCDataFacade.h"
#include "VizCSortSecuritySearchModel.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCSecuritySearchModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	
	VizCSecuritySearchModel(QObject* parent = 0);	
	~VizCSecuritySearchModel();
private:
	net::VizCTP* VizTP;
	io::VizCDataFacade* DataFacade;
	
	QDate SyncTimeStamp;	
	QSet<QString> InDiskNegotiableList;
	
	QVector<QStringList> Content;

	bool saveSecurityList();
	bool loadSecurityList();	
	void appendSecurityInfoComplete(QVector<QStringList>& completeList);
	void appendSecurityInfo(QStringList& list);
	bool removeSecurityInfo(const QString& symbol);
	void requestFullNegotiableList();
	void addLocalFilesInNegotiableList();

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	void initialize();	
private slots:
	void startSyncNegotiableList();
	void updateNegotiableList();
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------