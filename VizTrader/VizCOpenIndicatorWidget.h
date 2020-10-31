//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCOPENINDICATORWIDGET_H__
#define __VIZCOPENINDICATORWIDGET_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include "VizCSearchEngine.h"
#include "VizCPreviewGraphics.h"
#include "VizCIndicatorSearchModel.h"
#include "VizCSearchLineEdit.h"
#include "VizCBrowserView.h"
#include <QString>
#include <QDialog>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCOpenIndicatorWidget : public QDialog
{
	Q_OBJECT
public:
	static VizCOpenIndicatorWidget* instance();
	
	QString indicatorClassName() const;

public slots:
	int exec();

private:	
	VizCSearchLineEdit* LineEdit;
	VizCSearchEngine* SearchEngine;
	VizCBrowserView* BrowserView;

	QPushButton* OpenButton;
	QPushButton* CancelButton;
	VizCPreviewGraphics* PreviewGraphics;
	VizCIndicatorSearchModel* Model;
	QString IndicatorClassName;

	static VizCOpenIndicatorWidget* Instance;
	VizCOpenIndicatorWidget(QWidget* parent = 0);
	~VizCOpenIndicatorWidget();

private slots:
	void insertBrowserCompletion(const QModelIndex& index);
	void lineEditInsertCompletion(const QModelIndex& index);
	void highlighted(const QModelIndex& index);
	void notHighlighted();
	void onClose(int closeCondition);

};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
