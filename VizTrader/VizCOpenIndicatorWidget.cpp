//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCOpenIndicatorWidget.h"
#include "VizCSecurityPeriodsManipulator.h"
#include <QTextStream>
#include <QFile>
#include <QtGui>
#include <QVariant>
#include <QDebug>
#include <QStringList>
#include <QDir>
#include <QApplication>
#include <QListView>
#include <QTreeView>
#include <QHeaderView>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
VizCOpenIndicatorWidget* VizCOpenIndicatorWidget::Instance = 0; 
//-----------------------------------------------------------------------------
VizCOpenIndicatorWidget::VizCOpenIndicatorWidget(QWidget* parent) : QDialog(parent), IndicatorClassName("")
{
	setWindowIcon(QIcon(":/VizTrader/Resources/windowIcon.png"));
	//setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
	setMinimumSize(QSize(600, 450));
	setWindowTitle(tr("Adicionar Indicador"));		

	Model = new VizCIndicatorSearchModel(this);
	SearchEngine = new VizCSearchEngine();
	SearchEngine->setCaseSensitivity(Qt::CaseInsensitive);
	SearchEngine->setModel(Model);
	SearchEngine->setColumnPriorityList(QList<int>() << 0 << 3 << 1 << 2);
	//SearchEngine->setLastSearchableColumn()

	LineEdit = new VizCSearchLineEdit;
	LineEdit->setCompleter(SearchEngine);

	BrowserView = new VizCBrowserView;
	BrowserView->setModel(SearchEngine->completionModel());
	BrowserView->setColumnHidden(3, true);
	BrowserView->setColumnHidden(4, true);
	BrowserView->resizeColumnToContents(0);
	BrowserView->resizeColumnToContents(1);
	BrowserView->resizeColumnToContents(2);

	
	QObject::connect(BrowserView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(insertBrowserCompletion(const QModelIndex&)));
	QObject::connect(BrowserView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(lineEditInsertCompletion(const QModelIndex&)));
	QObject::connect(SearchEngine, SIGNAL(highlighted(const QModelIndex&)), this, SLOT(highlighted(const QModelIndex&)));
	QObject::connect(SearchEngine, SIGNAL(notHighlighted()), this, SLOT(notHighlighted()));	
	QObject::connect(this, SIGNAL(finished(int)), SLOT(onClose(int)));	
	

	QVBoxLayout* rows = new QVBoxLayout;
	QVBoxLayout* rows1 = new QVBoxLayout;
	QHBoxLayout* cols = new QHBoxLayout;
	QHBoxLayout* buttonLayout = new QHBoxLayout;

	PreviewGraphics = new VizCPreviewGraphics;
	OpenButton = new QPushButton(tr("Adicionar"));
	OpenButton->setEnabled(false);
	CancelButton = new QPushButton(tr("Cancelar"));


/*	QVector<core::VizCTradePeriod> periods = core::VizCSecurityPeriodsManipulator::instance()->convertPeriodicityInteval(DataFacade->securityPeriodsDaily("PETR3"), VESP_YEARLY);
	PreviewGraphics->setPeriods(periods);
	PreviewGraphics->setRenderType(viz::gui::VEPGT_CHART);
	*/



	BrowserView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	PreviewGraphics->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	
	s32 w1 = 330;
	//s32 w2 = w1 / 1.618;
	BrowserView->setMinimumWidth(w1);
	


	QObject::connect(OpenButton, SIGNAL(clicked()), this, SLOT(accept()));
	QObject::connect(CancelButton, SIGNAL(clicked()), this, SLOT(reject()));

	QSpacerItem* horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	buttonLayout->addItem(horizontalSpacer);
	buttonLayout->addWidget(OpenButton);
	buttonLayout->addWidget(CancelButton);

	
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
VizCOpenIndicatorWidget::~VizCOpenIndicatorWidget()
{
}
//-----------------------------------------------------------------------------
VizCOpenIndicatorWidget* VizCOpenIndicatorWidget::instance()
{
	if(!Instance)
	{
		Instance = new VizCOpenIndicatorWidget;
	} 
	return Instance; 
}
//-----------------------------------------------------------------------------
void VizCOpenIndicatorWidget::highlighted(const QModelIndex& index)
{

	IndicatorClassName = SearchEngine->model()->data(SearchEngine->model()->index(index.row(), 4)).toString();
	OpenButton->setEnabled(true);

}
//-----------------------------------------------------------------------------
void VizCOpenIndicatorWidget::notHighlighted()
{

	IndicatorClassName = "";
	OpenButton->setEnabled(false);

}
//-----------------------------------------------------------------------------
QString VizCOpenIndicatorWidget::indicatorClassName() const
{
	return IndicatorClassName;
}
//-----------------------------------------------------------------------------
void VizCOpenIndicatorWidget::lineEditInsertCompletion(const QModelIndex& index)
{
	IndicatorClassName = SearchEngine->model()->data(SearchEngine->model()->index(index.row(), 4)).toString();
	OpenButton->setEnabled(true);
	LineEdit->insertCompletion(index);
}
//-----------------------------------------------------------------------------
void VizCOpenIndicatorWidget::insertBrowserCompletion(const QModelIndex& index)
{ 
	IndicatorClassName = SearchEngine->model()->data(SearchEngine->model()->index(index.row(), 4)).toString();
	OpenButton->setEnabled(true);
	accept();
}
//-----------------------------------------------------------------------------
int VizCOpenIndicatorWidget::exec()
{	
	IndicatorClassName = "";
	OpenButton->setEnabled(false);
	LineEdit->clear();

	return QDialog::exec();
}
//-----------------------------------------------------------------------------
void VizCOpenIndicatorWidget::onClose(int closeCondition)
{
	/*if(closeCondition == 0)
	{
		return;
	}*/
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
