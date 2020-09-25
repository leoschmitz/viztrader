//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCINDICATORSEARCHMODEL_H__
#define __VIZCINDICATORSEARCHMODEL_H__
//-----------------------------------------------------------------------------
#include <QAbstractTableModel>
#include <QVector>
#include <QList>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCIndicatorSearchModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	
	VizCIndicatorSearchModel(QObject* parent = 0);
	~VizCIndicatorSearchModel();
private:
	QVector<QList<QString> > Content;

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------