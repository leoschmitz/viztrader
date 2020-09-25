//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCOpenSecurityWidget.h"
#include "VizCSecurityPeriodsManipulator.h"
#include "VizCTP.h"
#include <QTextStream>
#include <QFile>
#include <QtGui>
#include <QTreeView>
#include <QHeaderView>
#include <QVariant>
#include <QDebug>
#include <QStringList>
#include <QDir>
#include <QApplication>
#include <QMovie>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
VizCOpenSecurityWidget* VizCOpenSecurityWidget::Instance = 0; 
//-----------------------------------------------------------------------------
VizCOpenSecurityWidget::VizCOpenSecurityWidget(QWidget* parent) : QDialog(parent), SecurityId(""), DataFacade(0)
{
	setWindowIcon(QIcon(":/VizTrader/Resources/windowIcon.png"));
	//setWindowFlags(Qt::Popup | Qt::WindowCloseButtonHint);
	setMinimumSize(QSize(600, 450));
	setWindowTitle(tr("Abrir Ativo"));		

	DataFacade = io::VizCDataFacade::instance();	
	PartialSecurityInfo = new VizCSecuritySearchModel(this);

	VizCSortSecuritySearchModel* proxyModel = new VizCSortSecuritySearchModel(this);
	proxyModel->sort(0, Qt::AscendingOrder);
	proxyModel->sort(4, Qt::DescendingOrder);
	SearchSecurities = new VizCSearchEngine;
	SearchSecurities->setFilterSortModel(PartialSecurityInfo, proxyModel);
	SearchSecurities->setColumnPriorityList(QList<int>() << 0 << 1);

	LineEdit = new VizCSearchLineEdit;
	LineEdit->setCompleter(SearchSecurities);

	BrowserView = new VizCBrowserView;
	BrowserView->setModel(SearchSecurities->completionModel());
	
	BrowserView->resizeColumnToContents(0);
	BrowserView->resizeColumnToContents(1);
	BrowserView->resizeColumnToContents(2);
	BrowserView->resizeColumnToContents(3);
	BrowserView->resizeColumnToContents(4);
	
	
	QObject::connect(BrowserView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(insertBrowserCompletion(QModelIndex)));	
	QObject::connect(BrowserView, SIGNAL(clicked(QModelIndex)), LineEdit, SLOT(insertCompletion(QModelIndex)));
	QObject::connect(this, SIGNAL(finished(int)), SLOT(onClose(int)));	

	QVBoxLayout* rows = new QVBoxLayout;
	QVBoxLayout* rows1 = new QVBoxLayout;
	QHBoxLayout* cols = new QHBoxLayout;
	QHBoxLayout* buttonLayout = new QHBoxLayout;


	PreviewGraphics = new VizCPreviewGraphics;
	OpenButton = new QPushButton(tr("Abrir"));
	CancelButton = new QPushButton(tr("Cancelar"));


	/*
	QVector<core::VizCTradePeriod> periods = core::VizCSecurityPeriodsManipulator::instance()->convertPeriodicityInteval(DataFacade->securityPeriodsDaily("PETR3"), VESP_YEARLY);
	PreviewGraphics->setPeriods(periods);
	PreviewGraphics->setRenderType(viz::gui::VEPGT_CHART);
	*/


	BrowserView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	PreviewGraphics->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	s32 w1 = 330;
	BrowserView->setMinimumWidth(w1);

	QObject::connect(OpenButton, SIGNAL(clicked()), this, SLOT(accept()));
	QObject::connect(CancelButton, SIGNAL(clicked()), this, SLOT(reject()));

	QSpacerItem* horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	buttonLayout->addItem(horizontalSpacer);
	buttonLayout->addWidget(OpenButton);
	buttonLayout->addWidget(CancelButton);


	QMovie* movie = new QMovie(":/VizTrader/Resources/loading.gif");
	LoadingMovie = new QLabel(this);
	LoadingMovie->setMovie(movie);
	LoadingMovie->setScaledContents(true);
	LoadingMovie->setFixedSize(16, 16);
	movie->start();
	LoadingMovie->setVisible(false);
	net::VizCTP* vtp = net::VizCTP::instance();
	QObject::connect(vtp, SIGNAL(negotiableListReady()), this, SLOT(negotiableListReady()));
	QObject::connect(vtp, SIGNAL(syncronizingNegotiableList()), this, SLOT(syncronizingNegotiableList()));
	QObject::connect(vtp, SIGNAL(disconnected()), this, SLOT(negotiableListReady()));

	buttonLayout->addWidget(LoadingMovie);

	
	cols->addWidget(BrowserView);
	//cols->addWidget(PreviewGraphics);

	rows1->addWidget(LineEdit);
	rows1->addLayout(cols);


	QVBoxLayout* box = new QVBoxLayout;
	QGroupBox* gb = new QGroupBox;
	gb->setLayout(rows1);
	box->addWidget(gb);
	

	rows->addLayout(box);
	rows->addLayout(buttonLayout);
	

	setLayout(rows);
}
//-----------------------------------------------------------------------------
VizCOpenSecurityWidget::~VizCOpenSecurityWidget()
{
}
//-----------------------------------------------------------------------------
VizCOpenSecurityWidget* VizCOpenSecurityWidget::instance()
{
	if(!Instance)
	{
		Instance = new VizCOpenSecurityWidget;
	} 
	return Instance; 
}
//-----------------------------------------------------------------------------
int VizCOpenSecurityWidget::exec()
{	
	return QDialog::exec();
}
//-----------------------------------------------------------------------------
QString VizCOpenSecurityWidget::securityId() const
{
	return SecurityId;
}
//-----------------------------------------------------------------------------
void VizCOpenSecurityWidget::insertBrowserCompletion(const QModelIndex& index)
{ 
	QString secId = SearchSecurities->model()->data(SearchSecurities->model()->index(index.row(), 0)).toString();
	LineEdit->setText(secId);
	accept();
}
//-----------------------------------------------------------------------------
void VizCOpenSecurityWidget::onClose(int closeCondition)
{
	if(closeCondition == 0)
	{
		//LineEdit->clear();
		return;
	}
	SecurityId = LineEdit->text().toUpper();	
	//LineEdit->clear();
	/*if (!DataFacade->loadSecurity(SecurityId))
	{
		setResult(0);
	}*/
	//LoadAccept = DataFacade->contains(LastLoad);		
}
//-----------------------------------------------------------------------------
void VizCOpenSecurityWidget::negotiableListReady()
{
	LoadingMovie->setVisible(false);
}
//-----------------------------------------------------------------------------
void VizCOpenSecurityWidget::syncronizingNegotiableList()
{
	LoadingMovie->setVisible(true);
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
