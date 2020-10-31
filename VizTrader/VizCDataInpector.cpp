//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCDataInspector.h"
#include <QTreeWidgetItem>
//-----------------------------------------------------------------------------
namespace viz
{
//-----------------------------------------------------------------------------
namespace gui
{
//-----------------------------------------------------------------------------
VizCDataInspector::VizCDataInspector() : QTreeWidget()
{
	setColumnCount(2);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setItemsExpandable(true);
	setExpandsOnDoubleClick(true);
	setHeaderHidden(true);
	setSortingEnabled(false);
	setWordWrap(true);
	setEditTriggers(NoEditTriggers);

	/*QStringList headers;
	headers << QObject::tr("Ítens") << QObject::tr("Visível");
	setHeaderLabels(headers);
	*/
	

	setRootIsDecorated(true);
	setAlternatingRowColors(true);

	setFocusPolicy(Qt::NoFocus);
}
//-----------------------------------------------------------------------------
VizCDataInspector::~VizCDataInspector()
{
}
//-----------------------------------------------------------------------------
void VizCDataInspector::setData(const QString& securitySymbol, f64 value, const core::VizCTradePeriod& period)
{

	clear();

	QList<QTreeWidgetItem*> items;

	
	
/*
QString s = QString("%L1\nDate: %L2\nOpen: %L3\nHigh: %L4\nLow: %L5\nClose: %L6\nVolume: %L7")
.arg(SecuritySymbol)
.arg(period.TimeStamp.date().toString(Qt::SystemLocaleShortDate))
.arg(period.OpenPrice, 0, 'f', 2)
.arg(period.MaxPrice, 0, 'f', 2)
.arg(period.MinPrice, 0, 'f', 2)
.arg(period.ClosePrice, 0, 'f', 2)
.arg(period.TotalVolume, 0, 'f', 2);

*/
	
	
	
	
	items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList() << securitySymbol << QString("")));
	items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList() << QString(tr("Valor:")) << QString("%L1").arg(value, 0, 'f', 2)));
	items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList() << QString("Data:") << QString("%L1").arg(period.TimeStamp.date().toString(Qt::SystemLocaleShortDate))));
	items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList() << QString("Abertura:") << QString("%L1").arg(period.OpenPrice, 0, 'f', 2)));
	items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList() << QString("Maior:") << QString("%L1").arg(period.MaxPrice, 0, 'f', 2)));
	items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList() << QString("Menor:") << QString("%L1").arg(period.MinPrice, 0, 'f', 2)));
	items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList() << QString("Fechamento:") << QString("%L1").arg(period.ClosePrice, 0, 'f', 2)));
	items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList() << QString("Volume:") << QString("%L1").arg(period.TotalVolume, 0, 'f', 2)));

	//twi->setFlags(twi->flags() | Qt::ItemIsUserCheckable);
	//twi->setToolTip(0, name);




	



	insertTopLevelItems(0, items);
	expandAll();

}
//-----------------------------------------------------------------------------
void VizCDataInspector::clearData()
{
	clear();
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------

