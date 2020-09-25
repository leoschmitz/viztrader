//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCOPENSECURITYWIDGET_H__
#define __VIZCOPENSECURITYWIDGET_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include "VizCSecurity.h"
#include "VizCSecurityPeriod.h"
#include "VizCDataFacade.h"
#include "VizCSearchEngine.h"
#include "VizCPreviewGraphics.h"
#include "VizCSearchLineEdit.h"
#include "VizCBrowserView.h"
#include "VizCTP.h"
#include <QString>
#include <QDialog>
#include <QMap> 
#include <QLabel>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
class VizCOpenSecurityWidget : public QDialog
{
	Q_OBJECT

public:
	static VizCOpenSecurityWidget* instance();
	QString securityId() const;

public slots:
	int exec();

private slots:
	void onClose(int closeCondition);	
	void insertBrowserCompletion(const QModelIndex& index);
	void negotiableListReady();
	void syncronizingNegotiableList();
	
private:	
	static VizCOpenSecurityWidget* Instance;
	VizCOpenSecurityWidget(QWidget* parent = 0);
	~VizCOpenSecurityWidget();

	io::VizCDataFacade* DataFacade;
	VizCSecuritySearchModel* PartialSecurityInfo;
	VizCSearchLineEdit* LineEdit;
	VizCSearchEngine* SearchSecurities;
	VizCBrowserView* BrowserView;
	QPushButton* OpenButton;
	QPushButton* CancelButton;
	VizCPreviewGraphics* PreviewGraphics;
	QString SecurityId;
	QLabel* LoadingMovie;
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
