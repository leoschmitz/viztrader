//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCSORTSECURITYSEARCHMODEL_H__
#define __VIZCSORTSECURITYSEARCHMODEL_H__
//-----------------------------------------------------------------------------
#include <QSortFilterProxyModel>
#include <QModelIndex>
#include <QDebug>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCSortSecuritySearchModel : public QSortFilterProxyModel
{	
	Q_OBJECT
private:
	bool Online;
	int securityNamePriority(QString& securityType) const;
	void setStatus(bool online);
public:	
	bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

	VizCSortSecuritySearchModel(QObject* parent = 0);
	~VizCSortSecuritySearchModel();

protected:
	bool filterAcceptsRow ( int source_row, const QModelIndex & source_parent ) const;

private slots:
	void connected();
	void disconnected();
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------