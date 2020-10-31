//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCMdiSubWindow.h"
#include "VizCSecurityWidget.h"
#include "VizCOpenSecurityWidget.h"
#include "VizCOpenIndicatorWidget.h"
#include "VizCPropertyEditor.h"
#include "VizCGraphicsPanel.h"
#include "VizCToolManager.h"
#include "VizCSceneInspector.h"
#include "VizCUndoView.h"
#include "VizCWorkArea.h"
#include "VizCGraphicsPanel.h"
#include "VizCOfferBookPanel.h"
#include "VizIChart.h"
#include <QMessageBox>
#include <QDebug>
#include <QToolButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QStyle>
#include <QProgressBar>
#include <QLabel>
#include <QMovie>
#include <QDialogButtonBox>
#include <QPushButton>
//-----------------------------------------------------------------------------
namespace viz
{
namespace gui
{
//-----------------------------------------------------------------------------
static const QString DefaultChart = "viz::gui::VizCCandlesticksChart";
//-----------------------------------------------------------------------------
VizCMdiSubWindow::VizCMdiSubWindow(VizCWorkArea* workArea) : QMdiSubWindow(), WorkArea(workArea), SelectedPanel(0), PrimaryPanel(0), SelectedChart(0), PrimaryChart(0)
{
	Q_ASSERT(WorkArea);

	//setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::CustomizeWindowHint);
	setWindowIcon(QIcon(":/VizTrader/Resources/windowIcon.png"));
	setParent(WorkArea->mdiArea());
	setAttribute(Qt::WA_DeleteOnClose);

	TabWidget = new QTabWidget(this);
	TabWidget->setDocumentMode(true);
	TabWidget->setMovable(true);
	connect(TabWidget, SIGNAL(currentChanged(int)), this, SLOT(currentChanged(int)));

	QPalette pal = palette();
	pal.setColor(QPalette::Active, QPalette::Button, pal.color(QPalette::Active, QPalette::Window));
	pal.setColor(QPalette::Disabled, QPalette::Button, pal.color(QPalette::Disabled, QPalette::Window));
	pal.setColor(QPalette::Inactive, QPalette::Button, pal.color(QPalette::Inactive, QPalette::Window));

	NewTabButton = new QToolButton(this);
	NewTabButton->setText(tr("Adicionar"));
	NewTabButton->setCursor(Qt::ArrowCursor);
	//NewTabButton->setArrowType(Qt::DownArrow);
	NewTabButton->setIcon(QIcon(":/VizTrader/Resources/addtab.png"));
	NewTabButton->setAutoRaise(true);
	NewTabButton->setPopupMode(QToolButton::InstantPopup);
	NewTabButton->setToolTip(tr("Adicionar..."));
	QObject::connect(NewTabButton, SIGNAL(clicked()), this, SLOT(menuOpen()));
	
	CloseTabButton = new QToolButton(this);
	CloseTabButton->setPalette(pal);
	CloseTabButton->setCursor(Qt::ArrowCursor);
	CloseTabButton->setAutoRaise(true);
	//QPixmap ClosePixmap = style()->standardIcon(QStyle::SP_TitleBarCloseButton).pixmap(QSize(16, 16));
	CloseTabButton->setIcon(QIcon(":/VizTrader/Resources/closetab.png"));
	CloseTabButton->setToolTip(tr("Fechar"));
	CloseTabButton->setEnabled(false);
	QObject::connect(CloseTabButton, SIGNAL(clicked()), this, SLOT(closeCurrentTab()));

	TabWidget->setCornerWidget(NewTabButton, Qt::TopLeftCorner);
	TabWidget->setCornerWidget(CloseTabButton, Qt::TopRightCorner);

	AddMenu = new QMenu(NewTabButton);

	MenuAddChart = AddMenu->addMenu(tr("Gráfico"));
		AddPrimaryChart = MenuAddChart->addAction(tr("Primário"));
		MenuAddSecondaryChart = MenuAddChart->addMenu(tr("Secundário"));
			AddSecundaryChartInNewPanel = MenuAddSecondaryChart->addAction(tr("Em Novo Painel"));
			AddSecundaryChartInSelectedPanel = MenuAddSecondaryChart->addAction(tr("No Painel Atual"));
			AddSecundaryChartInPrimaryPanel = MenuAddSecondaryChart->addAction(tr("No Painel do Gráfico Primário"));
	
	MenuAddIndicator = AddMenu->addMenu(tr("Indicador"));
		MenuAddIndicatorPrimaryChart = MenuAddIndicator->addMenu(tr("Base Gráfico Primário"));
			AddIndicatorBasedOnPrimaryChartInNewPanel = MenuAddIndicatorPrimaryChart->addAction(tr("Em Novo Painel"));
			AddIndicatorBasedOnPrimaryChartInSelectedPanel = MenuAddIndicatorPrimaryChart->addAction(tr("No Painel Atual"));
			AddIndicatorBasedOnPrimaryChartInPrimaryPanel = MenuAddIndicatorPrimaryChart->addAction(tr("No Painel do Gráfico Primário"));

		MenuAddIndicatorSelectedChart = MenuAddIndicator->addMenu(tr("Base Gráfico Selecionado"));
			AddIndicatorBasedOnSelectedChartInNewPanel = MenuAddIndicatorSelectedChart->addAction(tr("Em Novo Painel"));
			AddIndicatorBasedOnSelectedChartInSelectedPanel = MenuAddIndicatorSelectedChart->addAction(tr("No Painel Atual"));
			AddIndicatorBasedOnSelectedChartInPrimaryPanel = MenuAddIndicatorSelectedChart->addAction(tr("No Painel do Gráfico Primário"));

	MenuAddOfferBook = AddMenu->addMenu(tr("Book de Ofertas"));
		AddSimplifiedOfferBook = MenuAddOfferBook->addAction(tr("Simplificado"));
		AddFullOfferBook = MenuAddOfferBook->addAction(tr("Completo"));



	QObject::connect(AddPrimaryChart, SIGNAL(triggered()), this, SLOT(addPrimaryChart()));

	QObject::connect(AddSecundaryChartInSelectedPanel, SIGNAL(triggered()), this, SLOT(addSecundaryChartInSelectedPanel()));
	QObject::connect(AddSecundaryChartInPrimaryPanel, SIGNAL(triggered()), this, SLOT(addSecundaryChartInPrimaryPanel()));
	QObject::connect(AddSecundaryChartInNewPanel, SIGNAL(triggered()), this, SLOT(addSecundaryChartInNewPanel()));
	
	QObject::connect(AddIndicatorBasedOnPrimaryChartInSelectedPanel, SIGNAL(triggered()), this, SLOT(addIndicatorBasedOnPrimaryChartInSelectedPanel()));
	QObject::connect(AddIndicatorBasedOnPrimaryChartInPrimaryPanel, SIGNAL(triggered()), this, SLOT(addIndicatorBasedOnPrimaryChartInPrimaryPanel()));
	QObject::connect(AddIndicatorBasedOnPrimaryChartInNewPanel, SIGNAL(triggered()), this, SLOT(addIndicatorBasedOnPrimaryChartInNewPanel()));

	QObject::connect(AddIndicatorBasedOnSelectedChartInSelectedPanel, SIGNAL(triggered()), this, SLOT(addIndicatorBasedOnSelectedChartInSelectedPanel()));
	QObject::connect(AddIndicatorBasedOnSelectedChartInPrimaryPanel, SIGNAL(triggered()), this, SLOT(addIndicatorBasedOnSelectedChartInPrimaryPanel()));
	QObject::connect(AddIndicatorBasedOnSelectedChartInNewPanel, SIGNAL(triggered()), this, SLOT(addIndicatorBasedOnSelectedChartInNewPanel()));

	QObject::connect(AddSimplifiedOfferBook, SIGNAL(triggered()), this, SLOT(addSimplifiedOfferBook()));
	QObject::connect(AddFullOfferBook, SIGNAL(triggered()), this, SLOT(addFullOfferBook()));


	//NewTabButton->setMenu(Menu);

	setWidget(TabWidget);

	OpenSecurityWidget = VizCOpenSecurityWidget::instance();
	OpenIndicatorWidget = VizCOpenIndicatorWidget::instance();
	DataFacade = io::VizCDataFacade::instance();

}
//-----------------------------------------------------------------------------
VizCMdiSubWindow::~VizCMdiSubWindow()
{
}
//-----------------------------------------------------------------------------
s32 VizCMdiSubWindow::addTab(const QString& label, QWidget* widget)
{
	return TabWidget->addTab(widget, label);
}
//-----------------------------------------------------------------------------
void VizCMdiSubWindow::removeTab(s32 index)
{
	Q_ASSERT(index < 0 || index >= TabWidget->count());

	if(anyItenIsSelected(index))
	{
		WorkArea->clearSelection();
	}

	TabWidget->removeTab(index);
}
//-----------------------------------------------------------------------------
void VizCMdiSubWindow::closeCurrentTab()
{
	removeTab(currentTab());
}
//-----------------------------------------------------------------------------
void VizCMdiSubWindow::setCurretTab(s32 index)
{
	Q_ASSERT(index >= 0 || index < TabWidget->count());

	TabWidget->setCurrentIndex(index);
}
//-----------------------------------------------------------------------------
QWidget* VizCMdiSubWindow::tabWidget(s32 index) const
{
	Q_ASSERT(index >= 0 || index < TabWidget->count());

	return TabWidget->widget(index);
}
//-----------------------------------------------------------------------------
s32 VizCMdiSubWindow::currentTab() const
{
	return TabWidget->currentIndex();
}
//-----------------------------------------------------------------------------
void VizCMdiSubWindow::menuOpen()
{
	findPanelsAndChart();
	menuUpdate();
	QPoint pos = NewTabButton->mapToGlobal(NewTabButton->geometry().bottomLeft());
	AddMenu->popup(pos);
}
//-----------------------------------------------------------------------------
void VizCMdiSubWindow::menuUpdate()
{

	bool isCurrentTabSecurityWidget = static_cast<bool>(qobject_cast<VizCSecurityWidget*>(tabWidget(currentTab())));

	//ajust chart menus
	MenuAddChart->setEnabled(false); //ok
	MenuAddSecondaryChart->setEnabled(false); //ok
	AddPrimaryChart->setEnabled(false); //ok
	AddSecundaryChartInSelectedPanel->setEnabled(false); //ok
	AddSecundaryChartInPrimaryPanel->setEnabled(false); //ok
	AddSecundaryChartInNewPanel->setEnabled(false); //ok

	MenuAddChart->setEnabled(true);
	AddPrimaryChart->setEnabled(true);

	if(isCurrentTabSecurityWidget)
	{
		MenuAddSecondaryChart->setEnabled(true);
		AddSecundaryChartInNewPanel->setEnabled(true);

		if(SelectedPanel)
		{
			AddSecundaryChartInSelectedPanel->setEnabled(true);
		}
		if(PrimaryPanel)
		{
			AddSecundaryChartInPrimaryPanel->setEnabled(true);
		}
	}

	//ajust indicators menus
	MenuAddIndicator->setEnabled(false); //ok
	MenuAddIndicatorPrimaryChart->setEnabled(false); //ok
	MenuAddIndicatorSelectedChart->setEnabled(false); //ok
	AddIndicatorBasedOnPrimaryChartInSelectedPanel->setEnabled(false); //ok
	AddIndicatorBasedOnPrimaryChartInPrimaryPanel->setEnabled(false); //ok
	AddIndicatorBasedOnPrimaryChartInNewPanel->setEnabled(false); //ok
	AddIndicatorBasedOnSelectedChartInSelectedPanel->setEnabled(false); //ok
	AddIndicatorBasedOnSelectedChartInPrimaryPanel->setEnabled(false); //ok
	AddIndicatorBasedOnSelectedChartInNewPanel->setEnabled(false); //ok

	if(isCurrentTabSecurityWidget)
	{
		MenuAddIndicator->setEnabled(true);

		if(SelectedChart)
		{
			MenuAddIndicatorSelectedChart->setEnabled(true);
			AddIndicatorBasedOnSelectedChartInNewPanel->setEnabled(true);
			
			if(SelectedPanel)
			{
				AddIndicatorBasedOnSelectedChartInSelectedPanel->setEnabled(true);
			}
			if(PrimaryPanel)
			{
				AddIndicatorBasedOnSelectedChartInPrimaryPanel->setEnabled(true);
			}
			
		}
		if(PrimaryChart)
		{
			MenuAddIndicatorPrimaryChart->setEnabled(true);
			AddIndicatorBasedOnPrimaryChartInNewPanel->setEnabled(true);

			if(SelectedPanel)
			{
				AddIndicatorBasedOnPrimaryChartInSelectedPanel->setEnabled(true);
			}
			if(PrimaryPanel)
			{
				AddIndicatorBasedOnPrimaryChartInPrimaryPanel->setEnabled(true);
			}
		}
	}

	//ajust offer book menus
	MenuAddOfferBook->setEnabled(false);
	AddSimplifiedOfferBook->setEnabled(false);
	AddFullOfferBook->setEnabled(false);


	if(isCurrentTabSecurityWidget)
	{
		MenuAddOfferBook->setEnabled(true);
		AddSimplifiedOfferBook->setEnabled(true);
	}
}

//-----------------------------------------------------------------------------
void VizCMdiSubWindow::loadDefaultProject()
{

	setWindowTitle(SecuritySymbolBase);
	VizCSecurityWidget* SecurityWidget = new VizCSecurityWidget(SecuritySymbolBase, false, WorkArea, this);

	VizCGraphicsPanel* InnerPanel = SecurityWidget->newGraphicsPanel(false);
	s32 index = addTab(SecuritySymbolBase, SecurityWidget);
	setCurretTab(index);

	InnerPanel->setLoadingState(true);
	InnerPanel->newGraphicsItemBasedOnSecurity(DefaultChart, SecuritySymbolBase, viz::VESIYO_DEFAULT, false);

	if(!loadSecurity(SecuritySymbolBase))
	{
		removeTab(index);
	}

}
//-----------------------------------------------------------------------------
void VizCMdiSubWindow::loadProject(const QString& securitySymbol)
{


	//se nao achou .xml do securitySymbol, carrega padrao
	SecuritySymbolBase = securitySymbol;
	loadDefaultProject();



}
//-----------------------------------------------------------------------------
void VizCMdiSubWindow::saveProject()
{
	//save SecuritySymbolBase
}
//-----------------------------------------------------------------------------
bool VizCMdiSubWindow::loadSecurity(const QString& securitySymbol)
{
	
	bool ok = DataFacade->loadSecurity(securitySymbol);
	
	if(!ok)
	{
		QMessageBox::critical(0, tr("Erro de Leitura"), tr("Esse ativo não existe."));
	}

	return ok;

}
//-----------------------------------------------------------------------------
void VizCMdiSubWindow::addPrimaryChart()
{
	OpenSecurityWidget->setWindowTitle(tr("Adicionar Ativo"));
	if(OpenSecurityWidget->exec() == QDialog::Rejected)
	{
		return;
	}
	QString securitySymbol = OpenSecurityWidget->securityId();


	//datafacade tem que ter um teste pra ver se da pra adicionar security
	//antes de chamar o loadSecurity 
	// load security nao pode retornar bool, tem que passar por evento o erro
	//bool canSecurityLoad ->so testa se eh possivel abrir
	//void loadSecurity -> abre e emite eventos de securityChanged, e loadSecurityError
	//void loadSecurityError(symbol, error)

	VizCSecurityWidget* SecurityWidget = new VizCSecurityWidget(securitySymbol, true, WorkArea, this); 
	VizCGraphicsPanel* InnerPanel = SecurityWidget->newGraphicsPanel(false);
	s32 index = addTab(securitySymbol, SecurityWidget);
	setCurretTab(index);

	InnerPanel->setLoadingState(true);
	InnerPanel->newGraphicsItemBasedOnSecurity(DefaultChart, securitySymbol, viz::VESIYO_DEFAULT, false);
	
	if(!loadSecurity(securitySymbol))
	{
		removeTab(index);
	}
	
}
//-----------------------------------------------------------------------------
void VizCMdiSubWindow::addSecundaryChartInSelectedPanel()
{
	Q_ASSERT(SelectedPanel);

	OpenSecurityWidget->setWindowTitle(tr("Adicionar Ativo"));
	if(OpenSecurityWidget->exec() == QDialog::Rejected)
	{
		return;
	}
	QString securitySymbol = OpenSecurityWidget->securityId();
	
	SelectedPanel->setLoadingState(true);
	VizIGraphicsItem* item = SelectedPanel->newGraphicsItemBasedOnSecurity(DefaultChart, securitySymbol, viz::VESIYO_DEFAULT, true);

	if(!loadSecurity(securitySymbol))
	{
		SelectedPanel->setLoadingState(false);
		SelectedPanel->removeItem(item);
		return;
	}

}
//-----------------------------------------------------------------------------
void VizCMdiSubWindow::addSecundaryChartInPrimaryPanel()
{
	Q_ASSERT(PrimaryPanel);

	OpenSecurityWidget->setWindowTitle(tr("Adicionar Ativo"));
	if(OpenSecurityWidget->exec() == QDialog::Rejected)
	{
		return;
	}
	QString securitySymbol = OpenSecurityWidget->securityId();
	
	PrimaryPanel->setLoadingState(true);
	VizIGraphicsItem* item = PrimaryPanel->newGraphicsItemBasedOnSecurity(DefaultChart, securitySymbol, viz::VESIYO_DEFAULT, true);

	if(!loadSecurity(securitySymbol))
	{
		PrimaryPanel->setLoadingState(false);
		PrimaryPanel->removeItem(item);
		return;
	}

}
//-----------------------------------------------------------------------------
void VizCMdiSubWindow::addSecundaryChartInNewPanel()
{
	OpenSecurityWidget->setWindowTitle(tr("Adicionar Ativo"));
	if(OpenSecurityWidget->exec() == QDialog::Rejected)
	{
		return;
	}

	QString securitySymbol = OpenSecurityWidget->securityId();

	VizCSecurityWidget* SecurityWidget = qobject_cast<VizCSecurityWidget*>(tabWidget(currentTab()));
	Q_ASSERT(SecurityWidget);
	
	VizCGraphicsPanel* InnerPanel = SecurityWidget->newGraphicsPanel(true);
	
	InnerPanel->setLoadingState(true);
	InnerPanel->newGraphicsItemBasedOnSecurity(DefaultChart, securitySymbol, viz::VESIYO_DEFAULT, false);

	if(!loadSecurity(securitySymbol))
	{
		s32 index = SecurityWidget->indexOfPanel(InnerPanel);
		SecurityWidget->closeInnerPanel(index);
	}

}
//-----------------------------------------------------------------------------
void VizCMdiSubWindow::addIndicatorBasedOnPrimaryChartInSelectedPanel()
{
	Q_ASSERT(SelectedPanel && PrimaryChart);
	
	if(OpenIndicatorWidget->exec() == QDialog::Rejected)
	{
		return;
	}
	QString indicatorClassName = OpenIndicatorWidget->indicatorClassName();
	QString securitySymbol = PrimaryChart->securitySymbol();
	
	SelectedPanel->setLoadingState(true);
	VizIGraphicsItem* item = SelectedPanel->newGraphicsItemBasedOnSecurity(indicatorClassName, securitySymbol, viz::VESIYO_DEFAULT, true);

	if(!loadSecurity(securitySymbol))
	{
		SelectedPanel->setLoadingState(false);
		SelectedPanel->removeItem(item);
	}

	
}
//-----------------------------------------------------------------------------
void VizCMdiSubWindow::addIndicatorBasedOnPrimaryChartInPrimaryPanel()
{
	Q_ASSERT(PrimaryPanel && PrimaryChart);


	if(OpenIndicatorWidget->exec() == QDialog::Rejected)
	{
		return;
	}
	QString indicatorClassName = OpenIndicatorWidget->indicatorClassName();
	QString securitySymbol = PrimaryChart->securitySymbol();
	
	PrimaryPanel->setLoadingState(true);
	VizIGraphicsItem* item = PrimaryPanel->newGraphicsItemBasedOnSecurity(indicatorClassName, securitySymbol, viz::VESIYO_DEFAULT, true);

	if(!loadSecurity(securitySymbol))
	{
		PrimaryPanel->setLoadingState(false);
		PrimaryPanel->removeItem(item);
	}
	

}
//-----------------------------------------------------------------------------
void VizCMdiSubWindow::addIndicatorBasedOnPrimaryChartInNewPanel()
{
	Q_ASSERT(PrimaryChart);
	if(OpenIndicatorWidget->exec() == QDialog::Rejected)
	{
		return;
	}

	QString indicatorClassName = OpenIndicatorWidget->indicatorClassName();
	QString securitySymbol = PrimaryChart->securitySymbol();

	VizCSecurityWidget* SecurityWidget = qobject_cast<VizCSecurityWidget*>(tabWidget(currentTab()));
	Q_ASSERT(SecurityWidget);
	
	VizCGraphicsPanel* InnerPanel = SecurityWidget->newGraphicsPanel(true);
	InnerPanel->setLoadingState(true);
	InnerPanel->newGraphicsItemBasedOnSecurity(indicatorClassName, securitySymbol, viz::VESIYO_DEFAULT, false);

	if(!loadSecurity(securitySymbol))
	{
		s32 index = SecurityWidget->indexOfPanel(InnerPanel);
		SecurityWidget->closeInnerPanel(index);
	}


}
//-----------------------------------------------------------------------------
void VizCMdiSubWindow::addIndicatorBasedOnSelectedChartInSelectedPanel()
{
	Q_ASSERT(SelectedPanel && SelectedChart);


	if(OpenIndicatorWidget->exec() == QDialog::Rejected)
	{
		return;
	}
	QString indicatorClassName = OpenIndicatorWidget->indicatorClassName();
	QString securitySymbol = SelectedChart->securitySymbol();

	SelectedPanel->setLoadingState(true);
	VizIGraphicsItem* item = SelectedPanel->newGraphicsItemBasedOnSecurity(indicatorClassName, securitySymbol, viz::VESIYO_DEFAULT, true);

	if(!loadSecurity(securitySymbol))
	{
		SelectedPanel->setLoadingState(false);
		SelectedPanel->removeItem(item);
	}

}
//-----------------------------------------------------------------------------
void VizCMdiSubWindow::addIndicatorBasedOnSelectedChartInPrimaryPanel()
{
	Q_ASSERT(PrimaryPanel && SelectedChart);

	if(OpenIndicatorWidget->exec() == QDialog::Rejected)
	{
		return;
	}

	QString indicatorClassName = OpenIndicatorWidget->indicatorClassName();
	QString securitySymbol = SelectedChart->securitySymbol();

	PrimaryPanel->setLoadingState(true);
	VizIGraphicsItem* item = PrimaryPanel->newGraphicsItemBasedOnSecurity(indicatorClassName, securitySymbol, viz::VESIYO_DEFAULT, true);

	if(loadSecurity(securitySymbol))
	{
		PrimaryPanel->setLoadingState(false);
		PrimaryPanel->removeItem(item);
	}

}
//-----------------------------------------------------------------------------
void VizCMdiSubWindow::addIndicatorBasedOnSelectedChartInNewPanel()
{
	Q_ASSERT(SelectedChart);

	if(OpenIndicatorWidget->exec() == QDialog::Rejected)
	{
		return;
	}
	QString indicatorClassName = OpenIndicatorWidget->indicatorClassName();
	QString securitySymbol = SelectedChart->securitySymbol();

	VizCSecurityWidget* SecurityWidget = qobject_cast<VizCSecurityWidget*>(tabWidget(currentTab()));
	Q_ASSERT(SecurityWidget);

	VizCGraphicsPanel* InnerPanel = SecurityWidget->newGraphicsPanel(true);
	InnerPanel->setLoadingState(true);
	InnerPanel->newGraphicsItemBasedOnSecurity(indicatorClassName, securitySymbol, viz::VESIYO_DEFAULT, false);
	
	if(!loadSecurity(securitySymbol))
	{
		s32 index = SecurityWidget->indexOfPanel(InnerPanel);
		SecurityWidget->closeInnerPanel(index);
	}

}
//-----------------------------------------------------------------------------
void VizCMdiSubWindow::addSimplifiedOfferBook()
{
	VizCSecurityWidget* SecurityWidget = qobject_cast<VizCSecurityWidget*>(tabWidget(currentTab()));
	Q_ASSERT(SecurityWidget);

	VizCOfferBookPanel* offerBookPanel = SecurityWidget->newOfferBookPanel(true);

}
//-----------------------------------------------------------------------------
void VizCMdiSubWindow::addFullOfferBook()
{


	//tem que fazer o widget do book de oferta completo e simplificado

	



}
//-----------------------------------------------------------------------------
void VizCMdiSubWindow::closeEvent(QCloseEvent* event)
{
	saveProject();

	if(anyItenIsSelected())
	{
		WorkArea->clearSelection();
	}
	
	setParent(0);

	if(VizCToolManager::instance()->isToolChecked(VETC_WINDOW_AUTO_TILE))
	{
		WorkArea->mdiArea()->tileSubWindows();
	}

	event->accept();
	
	
}
//-----------------------------------------------------------------------------
void VizCMdiSubWindow::findPanelsAndChart()
{

	SelectedPanel = 0;
	PrimaryPanel = 0;
	SelectedChart = 0;
	PrimaryChart = 0;

	VizCSecurityWidget* sw = qobject_cast<VizCSecurityWidget*>(tabWidget(currentTab()));
	if(sw)
	{
		for(s32 j = 0; j < sw->countInnerPanel(); j++)
		{
			VizCGraphicsPanel* gp = qobject_cast<VizCGraphicsPanel*>(sw->innerPanel(j));
			if(gp)
			{
				if(!PrimaryChart)
				foreach(VizIGraphicsItem* item, gp->items())
				{
					VizIChart* chart = qobject_cast<VizIChart*>(item);
					if(chart && !chart->isRemovable())
					{
						PrimaryChart = chart;
						break;
					}
				}

				if(!SelectedChart)
				foreach(VizIGraphicsItem* item, gp->items())
				{
					VizIChart* chart = qobject_cast<VizIChart*>(item);
					if(chart && chart->isSelected())
					{
						SelectedChart = chart;
						break;
					}
				}

				if(!PrimaryPanel)
				if(!gp->isClosable())
				{
					PrimaryPanel = gp;
				}

				if(!SelectedPanel)
				if(gp->isSelected())
				{
					SelectedPanel = gp;
				}

			}
		}
	}
}
//-----------------------------------------------------------------------------
bool VizCMdiSubWindow::anyItenIsSelected(s32 tabIndex) const
{

	bool haveOneSelected = static_cast<bool>(WorkArea->selectedVizOBject());

	if(haveOneSelected)
	{
		QVariant v;

		if(tabIndex == -1)
		{
			for(s32 i = 0; i < TabWidget->count(); i++)
			{
				QWidget* w = TabWidget->widget(i);
				VizCSecurityWidget* sw = qobject_cast<VizCSecurityWidget*>(w);
				if(sw)
				{
					VizCHorizontalScale* hs = sw->horizontalScale();
					if(vizObjectIsSelected(hs))
					{
						return true;
					}
					for(s32 j = 0; j < sw->countInnerPanel(); j++)
					{
						VizIInnerPanel* ip = sw->innerPanel(j);
						if(vizObjectIsSelected(ip))
						{
							return true;
						}
						VizCGraphicsPanel* gp = qobject_cast<VizCGraphicsPanel*>(ip);
						if(gp)
						{
							VizCVerticalScale* vs = gp->verticalScale();
							if(vizObjectIsSelected(vs))
							{
								return true;
							}
							foreach(VizIGraphicsItem* item, gp->items())
							{
								if(vizObjectIsSelected(item))
								{
									return true;
								}
							}
						}
					}
				}
			}
		}
		else
		{
			QWidget* w = tabWidget(tabIndex);
			VizCSecurityWidget* sw = qobject_cast<VizCSecurityWidget*>(w);
			if(sw)
			{
				VizCHorizontalScale* hs = sw->horizontalScale();
				if(vizObjectIsSelected(hs))
				{
					return true;
				}
				for(s32 j = 0; j < sw->countInnerPanel(); j++)
				{
					VizIInnerPanel* ip = sw->innerPanel(j);
					if(vizObjectIsSelected(ip))
					{
						return true;
					}
					VizCGraphicsPanel* gp = qobject_cast<VizCGraphicsPanel*>(ip);
					if(gp)
					{
						VizCVerticalScale* vs = gp->verticalScale();
						if(vizObjectIsSelected(vs))
						{
							return true;
						}
						foreach(VizIGraphicsItem* item, gp->items())
						{
							if(vizObjectIsSelected(item))
							{
								return true;
							}
						}
					}
				}
			}
		}
	}

	return false;

}
//-----------------------------------------------------------------------------
void VizCMdiSubWindow::currentChanged(int index)
{
	VizCSecurityWidget* tab = qobject_cast<VizCSecurityWidget*>(tabWidget(currentTab()));

	if(tab)
	{
		if(!tab->isClosable())
		{
			TabWidget->cornerWidget(Qt::TopRightCorner)->setEnabled(false);
			return;
		}
	}
	TabWidget->cornerWidget(Qt::TopRightCorner)->setEnabled(true);
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
