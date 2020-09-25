//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCSearchEngine.h"
#include "VizCSortSecuritySearchModel.h"
#include "VizCTP.h"

#include <QHeaderView>
#include <QTreeView>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
VizCSearchEngine::VizCSearchEngine(QObject* parent) : QCompleter(parent), CompletionColumnPriorityIndex(0)
{
	setCompletionMode(QCompleter::InlineCompletion);
	setCaseSensitivity(Qt::CaseInsensitive);

	ColumnPriorityList.append(0);

	setCompletionColumn(ColumnPriorityList.at(CompletionColumnPriorityIndex));		
}
//-----------------------------------------------------------------------------
VizCSearchEngine::~VizCSearchEngine()
{
}
//-----------------------------------------------------------------------------
void VizCSearchEngine::setLastSearchableColumn(int last)
{
	LastSearchableColumn = (last >= ColumnPriorityList.size()) ? (ColumnPriorityList.size() - 1) : last; 
}
//-----------------------------------------------------------------------------
void VizCSearchEngine::setColumnPriorityList(const QList<int>& list)
{
	if (list.size() > ColumnPriorityList.size())
		return;

	QSet<int> defaultOrder = ColumnPriorityList.toSet();
	QSet<int> receivingOrder = list.toSet();

	if (!defaultOrder.contains(receivingOrder))
		return;	

	ColumnPriorityList.clear();

	for (int i = 0; i < list.size(); i++) 
		ColumnPriorityList.append(list.at(i));
		

	if (list.size() < ColumnPriorityList.size())
	{
		defaultOrder.subtract(receivingOrder);
		
		ColumnPriorityList.append(defaultOrder.toList());

		setLastSearchableColumn(list.size() - 1);
	}

	resetToFirstPriority();
}
//-----------------------------------------------------------------------------
void VizCSearchEngine::setModel(QAbstractItemModel* searchModel)
{
	QCompleter::setModel(searchModel);

	for (int i = 1; i < searchModel->columnCount();i++)
		ColumnPriorityList.append(i);

	LastSearchableColumn = searchModel->columnCount() - 1;
}
//-----------------------------------------------------------------------------
void VizCSearchEngine::setFilterSortModel(QAbstractItemModel* searchModel, QSortFilterProxyModel* proxyModel)
{
	proxyModel->setSourceModel(searchModel);	

	setModel(proxyModel);
}
//-----------------------------------------------------------------------------
void VizCSearchEngine::resetToFirstPriority()
{
	CompletionColumnPriorityIndex = 0;

	setCompletionColumn(ColumnPriorityList.at(CompletionColumnPriorityIndex));
}
//-----------------------------------------------------------------------------
void VizCSearchEngine::checkActiveColumn()
{	
	if (completionCount() == 0)	
	{
		if (CompletionColumnPriorityIndex < LastSearchableColumn)
		{
			setCompletionColumn(ColumnPriorityList.at(++CompletionColumnPriorityIndex));
			
			complete();

			checkActiveColumn();
		}
		else
		{
			resetToFirstPriority();
			emit notHighlighted();
		}
	}
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
