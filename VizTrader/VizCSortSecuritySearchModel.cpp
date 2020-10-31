//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCSortSecuritySearchModel.h"
#include "VizCTP.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
VizCSortSecuritySearchModel::VizCSortSecuritySearchModel(QObject* parent) : QSortFilterProxyModel(parent), Online(false)
{
	connect(net::VizCTP::instance(), SIGNAL(connected()), this, SLOT(connected()));
	connect(net::VizCTP::instance(), SIGNAL(disconnected()), this, SLOT(disconnected()));		
}
//-----------------------------------------------------------------------------
VizCSortSecuritySearchModel::~VizCSortSecuritySearchModel()
{

}
//-----------------------------------------------------------------------------
bool VizCSortSecuritySearchModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	QVariant leftData = sourceModel()->data(left);
	QVariant rightData = sourceModel()->data(right);

	if (leftData.type() == QVariant::String) 
	{
		QString leftString = leftData.toString();
		QString rightString = rightData.toString();

		if (leftString.left(4) == rightString.left(4))
		{
			leftString.remove(0, 4);
			rightString.remove(0, 4);
			int leftDataPriority = securityNamePriority(leftString);
			int rightDataPriority = securityNamePriority(rightString);

			return (leftDataPriority < rightDataPriority);
		}		

		return (leftString < rightString);
	}
	return QSortFilterProxyModel::lessThan(left, right);
}
//-----------------------------------------------------------------------------
int VizCSortSecuritySearchModel::securityNamePriority(QString& securityType) const
{
	if (securityType == "3")
		return 1;
	else if (securityType == "4")
		return 10;
	else if (securityType == "3F")
		return 20;			
	else if (securityType == "3T")
		return 30;			
	else if (securityType == "4F")
		return 40;			
	else if (securityType == "4T")
		return 50;			
	else if (securityType.left(1) == "-")
		return 60;			

	//default:
	return 100;
}
//-----------------------------------------------------------------------------
bool VizCSortSecuritySearchModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
	if ((!Online) && (sourceModel()->data(sourceModel()->index(source_row, 5, source_parent)).toString().isEmpty()))
		return false;

	return true;
}
//-----------------------------------------------------------------------------
void VizCSortSecuritySearchModel::setStatus(bool online)
{
	Online = online;
}
//-----------------------------------------------------------------------------
void VizCSortSecuritySearchModel::connected()
{
 	setStatus(true);
	setFilterKeyColumn(-1);
}
//-----------------------------------------------------------------------------
void VizCSortSecuritySearchModel::disconnected()
{
	setStatus(false);
	setFilterKeyColumn(0);
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
