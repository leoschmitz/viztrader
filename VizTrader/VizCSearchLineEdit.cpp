//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCSearchLineEdit.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
VizCSearchLineEdit::VizCSearchLineEdit(QWidget* parent) : QLineEdit(parent)
{		
}
//-----------------------------------------------------------------------------
VizCSearchLineEdit::~VizCSearchLineEdit()
{
}
//-----------------------------------------------------------------------------
void VizCSearchLineEdit::keyPressEvent(QKeyEvent *e)
{
	switch (e->key())
	{					
	case Qt::Key_Return:
	case Qt::Key_Enter:		
		insertCompletion();		
		break;
	}

	//por enquanto mantemos o control + e como completador
 	bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_E); 
	if (!Completer || !isShortcut) 
		QLineEdit::keyPressEvent(e);

 	const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
	if (!Completer || (ctrlOrShift && e->text().isEmpty()))
		return;

	static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); 
	bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;

	QString completionPrefix = text();

	if (!isShortcut && (hasModifier || e->text().isEmpty()
		|| eow.contains(e->text().right(1)))) 
	{
		Completer->resetToFirstPriority();
		return;
	}

	if (completionPrefix.length() < 1)
		Completer->resetToFirstPriority();

	if (completionPrefix != Completer->completionPrefix()) 
	{
		Completer->setCompletionPrefix(completionPrefix);
	}

	switch (e->key())
	{			
	case Qt::Key_Delete:
	case Qt::Key_Escape:
	case Qt::Key_Backspace:
		return;
		break;	
	}

	Completer->complete(); 

	Completer->checkActiveColumn();		
}
//-----------------------------------------------------------------------------
void VizCSearchLineEdit::focusInEvent(QFocusEvent *e)
{
	if (Completer)
		Completer->setWidget(this);
	QLineEdit::focusInEvent(e);
}
//-----------------------------------------------------------------------------
void VizCSearchLineEdit::setCompleter(VizCSearchEngine* completer)
{		
	Completer = completer;		  

	if (!Completer)
		return;

	Completer->setWidget(this);		

	QObject::connect(Completer, SIGNAL(highlighted(QString)), this, SLOT(setInlineText(QString)));
	//QObject::connect(Completer, SIGNAL(activated(QModelIndex)), this, SLOT(insertCompletion(QModelIndex)));
}
//-----------------------------------------------------------------------------
void VizCSearchLineEdit::setInlineText(const QString& s)
{
	int currentCursorPosition = cursorPosition();
	QString complet = s.mid(text().size(), s.size() - text().size()).toLower();

	insert(complet);	

	//qDebug() << text();

	setSelection(currentCursorPosition, complet.size());
}
//-----------------------------------------------------------------------------
void VizCSearchLineEdit::insertCompletion(const QModelIndex& index)
{
	if (Completer->widget() != this)
		return;

	QString secId = Completer->completionModel()->data(Completer->completionModel()->index(index.row(), 0)).toString();

	//qDebug() << secId;

	setText(secId);		
}
//-----------------------------------------------------------------------------
void VizCSearchLineEdit::insertCompletion()
{ 
	insertCompletion(Completer->currentIndex());	
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------