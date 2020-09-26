//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCBROWSERVIEW_H__
#define __VIZCBROWSERVIEW_H__
//-----------------------------------------------------------------------------
#include <QDebug>
#include <QHeaderView>
#include <QTableView>
#include <QPainter>
#include <QModelIndex>
#include <QHeaderView>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCBrowserView : public QTableView
{
	Q_OBJECT
public:
	VizCBrowserView(QWidget* parent = 0) : QTableView(parent)
	{
		setEditTriggers(QAbstractItemView::NoEditTriggers);
		setWordWrap(false);
		setShowGrid(false);
		setAlternatingRowColors(true);
		setTabKeyNavigation(true);
		setSelectionMode(QAbstractItemView::SingleSelection);
		setSelectionBehavior(QAbstractItemView::SelectRows);
		setSortingEnabled(true);

		QHeaderView* HeaderView = horizontalHeader();
		HeaderView->setDefaultAlignment(Qt::AlignLeft);
		HeaderView->setStretchLastSection(true);
		HeaderView->setSectionResizeMode(QHeaderView::Interactive);
		HeaderView->setSectionsClickable(false);
		HeaderView->setSectionsMovable(false);

		verticalHeader()->setVisible(false);

	}
	~VizCBrowserView()
	{
	}	
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------