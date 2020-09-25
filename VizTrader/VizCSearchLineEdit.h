//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCSEARCHLINEEDIT_H__
#define __VIZCSEARCHLINEEDIT_H__
//-----------------------------------------------------------------------------
#include <QLineEdit>
#include <QModelIndex>
#include <QKeyEvent>
#include <QDebug>

#include "VizCSearchEngine.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCSearchLineEdit : public QLineEdit
{
	Q_OBJECT
private:
	VizCSearchEngine *Completer;	
	
public:
	VizCSearchLineEdit(QWidget* parent = 0);
	~VizCSearchLineEdit();
	void keyPressEvent(QKeyEvent *e);
	void focusInEvent(QFocusEvent *e);
	void setCompleter(VizCSearchEngine* completer);
	void insertCompletion();
public slots:
	void setInlineText(const QString& s);
	void insertCompletion(const QModelIndex& index);
	
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------