//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCSEARCHENGINE_H__
#define __VIZCSEARCHENGINE_H__
//-----------------------------------------------------------------------------
#include <QCompleter>
#include <QSortFilterProxyModel>
//#include "VizCSearchView.h"
#include "VizCSecuritySearchModel.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCSearchEngine : public QCompleter
{	
	Q_OBJECT
public:
	VizCSearchEngine(QObject* parent = 0);
	~VizCSearchEngine();

	// Mandar a prioridade através da posição dentro da lista, o default é crescente
	// Exemplo 1: list (3,2,1,0) vai dar prioridade inversa as colunas
	// Exemplo 2: list (1, 2, 0) vai dar prioridade ao elemento 1, depois ao 1, e depois ao 0
	void setLastSearchableColumn(int last);
	void setColumnPriorityList(const QList<int>& list);
	void resetToFirstPriority();
	void checkActiveColumn();
	void setModel(QAbstractItemModel* searchModel);
	void setFilterSortModel(QAbstractItemModel* searchModel, QSortFilterProxyModel* proxyModel);

signals:
	void notHighlighted();

private:
	int CompletionColumnPriorityIndex;
	QList<int> ColumnPriorityList;
	int LastSearchableColumn;

};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
