//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCWORKAREAMANAGERWIDGET_H__
#define __VIZCWORKAREAMANAGERWIDGET_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include "VizCWorkAreaManager.h"
#include <QDialog>
#include <QString>
#include <QListWidget>
#include <QToolButton>
#include <QPoint>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCWorkAreaManagerWidget: public QDialog
{
	Q_OBJECT

public:
	static VizCWorkAreaManagerWidget* instance();

	void setWorkAreaManager(VizCWorkAreaManager* wm);

public slots:
	virtual int	exec();

private:
	VizCWorkAreaManagerWidget();
	~VizCWorkAreaManagerWidget();
	void updateListView();
	void updateButtons();
	QString generateName();


	VizCWorkAreaManager* WorkAreaManager;
	QListWidget* ListWidget;
	QString OldText;
	s32 Modified;
	
	QToolButton* NewWorkAreaButton;
	QToolButton* RemoveWorkAreaButton;
	QToolButton* RenameWorkAreaButton;
	QToolButton* MoveUpWorkAreaButton;
	QToolButton* MoveDownWorkAreaButton;

	static VizCWorkAreaManagerWidget* Instance;

private slots:
	void newWorkArea();
	void removeWorkArea();
	void renameWorkArea();
	void moveUpWorkArea();
	void moveDownWorkArea();

	void slotItemClicked(QListWidgetItem* item);
	void slotItemDoubleClicked(QListWidgetItem* item);
	void slotItemChanged(QListWidgetItem* item);
	 

};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
