//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "viztrader.h"
#include <QStyleFactory>
#include <QLocale>
#include <QStatusBar>
#include <QMessageBox>
#include <QApplication>
#include <QMenuBar>
#include <QInputDialog>
#include <QToolBar>
#include <QTime>
#include <QSplashScreen>
#include <QThread>
#include <QClipboard>
#include <QMimeData>
#include <QTextBrowser>
#include <QDialogButtonBox>
#include <QErrorMessage>
#include <QFile>
#include <QMovie>
#include <QRegExp>
#include <QTextCodec>
//-----------------------------------------------------------------------------
using namespace viz;
using namespace core;
using namespace gui;
using namespace net;
using namespace io;
using namespace system;
//-------f----------------------------------------------------------------------
VizTrader::VizTrader()	: DataFacade(0), OpenSecurityWidget(0), PropertyEditor(0), SceneInspector(0), DataInspector(0), ToolManager(0), ApplicationSettings(0), ApplicationNetworkStatus(0), MdiArea(0), WorkArea(0), UndoManager(0), WorkAreaComboBox(0), CurrentWorkAreaIndex(0), AutoConnecting(false)
{
	setWindowIcon(QIcon(":/VizTrader/Resources/windowIconBig.png"));
	ApplicationData = VizCApplicationData::instance();
	QPixmap image(":/VizTrader/Resources/SplashScreen.png");
	QSplashScreen* SplashScreen = new QSplashScreen(image);
	////////////////////////////////////////////////
	QLabel* versionLabel = new QLabel(QString("<font color=\"#292929\">Versão: <b>%1</b></font>").arg(ApplicationData->applicationVersion().toShortString()));
	versionLabel->setParent(SplashScreen);
	versionLabel->setTextFormat(Qt::RichText);
	versionLabel->setGeometry(245, 0, versionLabel->geometry().width(), 335);
	////////////////////////////////////////////////
	SplashScreen->show();
	versionLabel->show();
	////////////////////////////////////////////////
	SplashScreen->showMessage("Carregando Configurações Globais...", Qt::AlignLeft | Qt::AlignBottom, Qt::black);
	applicationSettings();
	registerMetaTypes();
	Settings = VizCSettings::instance();
	ApplicationSettings = VizCApplicationSettings::instance();
	connect(ApplicationSettings, SIGNAL(settingsChange()), this, SLOT(aplicationSettingsChange()));
	////////////////////////////////////////////////
	SplashScreen->showMessage("Carregando Papeis...", Qt::AlignLeft | Qt::AlignBottom, Qt::black);
	DataFacade = io::VizCDataFacade::instance();
	CertificateInformation = VizCCertificateInformation::instance();
	////////////////////////////////////////////////
	SplashScreen->showMessage("Carregando Ambiente...", Qt::AlignLeft | Qt::AlignBottom, Qt::black);
	OpenSecurityWidget = VizCOpenSecurityWidget::instance();

	ToolManager = VizCToolManager::instance();
	Clipboard = VizCClipboard::instance();
	EditActions = VizCEditActions::instance();
	
	TP = VizCTP::instance();
	connect(TP, SIGNAL(connected()), this, SLOT(vizServerConnected()));
	connect(TP, SIGNAL(disconnected()), this, SLOT(vizServerDisconnected()));
	connect(TP, SIGNAL(certificateReady(bool)), this, SLOT(vizServerCertificateReady(bool)));
	connect(TP, SIGNAL(error(VizCTP::VizTPError)), this, SLOT(vizServerError(VizCTP::VizTPError)));

	WorkAreaManager = VizCWorkAreaManager::instance();
	WorkAreaManagerWidget = VizCWorkAreaManagerWidget::instance();
	WorkAreaManagerWidget->setWorkAreaManager(WorkAreaManager);
	WindowMapper = new QSignalMapper(this);
	StackedWidget = new QStackedWidget;
	connect(WorkAreaManager, SIGNAL(workAreaRemoved(int)), this, SLOT(workAreaRemoved(int)));
	connect(WorkAreaManager, SIGNAL(workAreaAdded(int)), this, SLOT(workAreaAdded(int)));
	connect(WorkAreaManager, SIGNAL(workAreaRename(int)), this, SLOT(workAreaRename(int)));
	connect(WorkAreaManager, SIGNAL(workAreaMove(int, int)), this, SLOT(workAreaMove(int, int)));

	setCentralWidget(StackedWidget);
	setFocusPolicy(Qt::StrongFocus);
	createActions();
	createMenus();
	createToolBars();
	createStatusBar();
	createDocks();
	setWindowTitle(ApplicationData->appicationName());
	setUnifiedTitleAndToolBarOnMac(true);
	
	////////////////////////////////////////////////
	workAreaLoad();
	////////////////////////////////////////////////

	connect(WindowMapper, SIGNAL(mapped(QWidget *)), this, SLOT(setActiveSubWindowByWindowMenu(QWidget *)));
	connect(ToolManager, SIGNAL(changed()), this, SLOT(updateToolBar()));
	////////////////////////////////////////////////
	
	SplashScreen->showMessage("Carregando Configurações do Usuário...", Qt::AlignLeft | Qt::AlignBottom, Qt::black);
	readSettings();
	////////////////////////////////////////////////
	SplashScreen->finish(this);

	SystemTrayIcon = new QSystemTrayIcon(this);

	changeClientStatus();

	createLoginDialog();

	aplicationSettingsChange();

}
//-----------------------------------------------------------------------------
VizTrader::~VizTrader()
{
}
//-----------------------------------------------------------------------------
void VizTrader::applicationSettings()
{
	QLocale locale(QLocale::Portuguese, QLocale::Brazil);
	QLocale::setDefault(locale);
	setLocale(locale);

	//QTextCodec::setCodecForTr(QTextCodec::codecForName("latin1"));

	QCoreApplication::setOrganizationName(ApplicationData->organizationName());
	QCoreApplication::setOrganizationDomain(ApplicationData->organizationDomain());
	QCoreApplication::setApplicationName(ApplicationData->appicationName());
	QCoreApplication::setApplicationVersion(ApplicationData->applicationVersion().toShortString());
}
//-----------------------------------------------------------------------------
void VizTrader::registerMetaTypes()
{

	//charts
	qRegisterMetaType<viz::gui::VizCLineChart>(viz::gui::VizCLineChart::staticMetaObject.className());
	qRegisterMetaType<viz::gui::VizCMountainChart>(viz::gui::VizCMountainChart::staticMetaObject.className());
	qRegisterMetaType<viz::gui::VizCCandlesticksChart>(viz::gui::VizCCandlesticksChart::staticMetaObject.className());
	qRegisterMetaType<viz::gui::VizCBarsChart>(viz::gui::VizCBarsChart::staticMetaObject.className());
	qRegisterMetaType<viz::gui::VizCHeikinAshiBarsChart>(viz::gui::VizCHeikinAshiBarsChart::staticMetaObject.className());
	qRegisterMetaType<viz::gui::VizCHeikinAshiCandlesticksChart>(viz::gui::VizCHeikinAshiCandlesticksChart::staticMetaObject.className());

	//indicators
	qRegisterMetaType<viz::gui::VizCMovingAverageIndicator>(viz::gui::VizCMovingAverageIndicator::staticMetaObject.className());
	qRegisterMetaType<viz::gui::VizCVolumeIndicator>(viz::gui::VizCVolumeIndicator::staticMetaObject.className());
	qRegisterMetaType<viz::gui::VizCMinusDirectionalIndicator>(viz::gui::VizCMinusDirectionalIndicator::staticMetaObject.className());
	qRegisterMetaType<viz::gui::VizCPlusDirectionalIndicator>(viz::gui::VizCPlusDirectionalIndicator::staticMetaObject.className());
	qRegisterMetaType<viz::gui::VizCAverageDirectionalMovementIndexIndicator>(viz::gui::VizCAverageDirectionalMovementIndexIndicator::staticMetaObject.className());
	qRegisterMetaType<viz::gui::VizCRelativeStrengthIndexIndicator>(viz::gui::VizCRelativeStrengthIndexIndicator::staticMetaObject.className());
	qRegisterMetaType<viz::gui::VizCBollingerBandsIndicator>(viz::gui::VizCBollingerBandsIndicator::staticMetaObject.className());
	qRegisterMetaType<viz::gui::VizCBollingerUpperBandIndicator>(viz::gui::VizCBollingerUpperBandIndicator::staticMetaObject.className());
	qRegisterMetaType<viz::gui::VizCBollingerLowerBandIndicator>(viz::gui::VizCBollingerLowerBandIndicator::staticMetaObject.className());

	//shapes
	qRegisterMetaType<viz::gui::VizCHorizontalLine>(viz::gui::VizCHorizontalLine::staticMetaObject.className());
	qRegisterMetaType<viz::gui::VizCVerticalLine>(viz::gui::VizCVerticalLine::staticMetaObject.className());
	qRegisterMetaType<viz::gui::VizCTrendLine>(viz::gui::VizCTrendLine::staticMetaObject.className());
	qRegisterMetaType<viz::gui::VizCSemiLogTrendLine>(viz::gui::VizCSemiLogTrendLine::staticMetaObject.className());
	
}
//-----------------------------------------------------------------------------
void VizTrader::applicationClose()
{
	writeSettings();
	workAreaSave();
	TP->disconnectFromVizServer();
	SystemTrayIcon->hide();

	for(s32 i = 0; i < WorkAreaManager->workAreaCount(); i++)
	{
		WorkAreaManager->workArea(i).second->mdiArea()->closeAllSubWindows();
	}
}
//-----------------------------------------------------------------------------
void VizTrader::commitDataRequest(QSessionManager& sm)
{
	//no user interaction is possible
	applicationClose();

}
//-----------------------------------------------------------------------------
void VizTrader::closeEvent(QCloseEvent* event)
{
	applicationClose();
	event->accept();
}
//-----------------------------------------------------------------------------
void VizTrader::undo()
{
	if(UndoManager)
	{
		UndoManager->undoAction()->trigger();
	}
}
//-----------------------------------------------------------------------------
void VizTrader::redo()
{
	if(UndoManager)
	{
		UndoManager->redoAction()->trigger();
	}
}
//-----------------------------------------------------------------------------
void VizTrader::workAreaChangeUndoAction()
{
	if(UndoManager)
	{
		bool b2 = UndoManager->undoAction()->isEnabled();
		EditActions->UndoAction->setEnabled(b2);
	}
}
//-----------------------------------------------------------------------------
void VizTrader::workAreaChangeRedoAction()
{
	if(UndoManager)
	{
		bool b1 = UndoManager->redoAction()->isEnabled();
		EditActions->RedoAction->setEnabled(b1);
	}
}
//-----------------------------------------------------------------------------
void VizTrader::workAreaManagement()
{
	/*if(WorkAreaManagerWidget->exec() != 0)
	{
		workAreaChange(0);
	}
	*/
	WorkAreaManagerWidget->exec();
}
//-----------------------------------------------------------------------------
void VizTrader::workAreaNext()
{
	s32 n = WorkAreaManager->workAreaCount();
	s32 index = CurrentWorkAreaIndex + 1 < n ? CurrentWorkAreaIndex + 1 : 0;
	WorkAreaComboBox->setCurrentIndex(index);
}
//-----------------------------------------------------------------------------
void VizTrader::workAreaPrev()
{
	s32 n = WorkAreaManager->workAreaCount();
	s32 index = CurrentWorkAreaIndex > 0 ? CurrentWorkAreaIndex - 1 : n - 1;
	WorkAreaComboBox->setCurrentIndex(index);
}
//-----------------------------------------------------------------------------
void VizTrader::workAreaChangeApplicationTitle()
{
	this->setWindowTitle(ApplicationData->appicationName() + " - {" + CurrentWorkAreaName + "}");
}
//-----------------------------------------------------------------------------
void VizTrader::workAreaChange(int index)
{
	if(UndoManager)
	{
		disconnect(UndoManager->undoAction(), SIGNAL(changed()), this, SLOT(workAreaChangeUndoAction()));
		disconnect(UndoManager->redoAction(), SIGNAL(changed()), this, SLOT(workAreaChangeRedoAction()));
	}

	if(WorkArea)
	{
		disconnect(WorkArea, SIGNAL(vizObjectAvailable()), this, SLOT(updateEditMenu()));
		disconnect(WorkArea->mdiArea(), SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(updateMenus()));
	}

	
	CurrentWorkAreaIndex = index;
	QPair<QString, VizCWorkArea*> pair = WorkAreaManager->workArea(CurrentWorkAreaIndex);
	WorkArea = pair.second;
	CurrentWorkAreaName = pair.first;
	MdiArea = WorkArea->mdiArea();
	UndoManager = WorkArea->undoManager();
	WorkArea->updateActiveSubWindow();

	PropertyEditor = WorkArea->propertyEditor();
	PropertyEditor->setParent(PropertyEditorDock);
	PropertyEditorDock->setWidget(PropertyEditor);

	SceneInspector = WorkArea->sceneInspector();
	SceneInspector->setParent(SceneInspectorDock);
	SceneInspectorDock->setWidget(SceneInspector);

	DataInspector = WorkArea->dataInspector();
	DataInspector->setParent(DataInspectorDock);
	DataInspectorDock->setWidget(DataInspector);


	UndoView = WorkArea->undoView();
	UndoView->setParent(UndoViewDock);
	UndoViewDock->setWidget(UndoView);

	if(UndoManager)
	{
		connect(UndoManager->undoAction(), SIGNAL(changed()), this, SLOT(workAreaChangeUndoAction()));
		connect(UndoManager->redoAction(), SIGNAL(changed()), this, SLOT(workAreaChangeRedoAction()));
	}

	if(WorkArea)
	{
		connect(WorkArea, SIGNAL(vizObjectAvailable()), this, SLOT(updateEditMenu()));
		connect(WorkArea->mdiArea(), SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(updateMenus()));
	}


	WorkAreaComboBox->blockSignals(true);
	WorkAreaComboBox->clear();
	for(s32 i = 0; i < WorkAreaManager->workAreaCount(); i++)
	{
		WorkAreaComboBox->addItem(WorkAreaManager->workArea(i).first);
	}
	WorkAreaComboBox->setCurrentIndex(CurrentWorkAreaIndex);
	WorkAreaComboBox->blockSignals(false);


	workAreaChangeUndoAction();
	workAreaChangeRedoAction();

	updateMenus();
	updateWindowMenu();
	updateEditMenu();
	updateToolBar();

	StackedWidget->setCurrentIndex(CurrentWorkAreaIndex);

	workAreaChangeApplicationTitle();
}
//-----------------------------------------------------------------------------
void VizTrader::workAreaRename(int index)
{
	workAreaChange(index);
}
//-----------------------------------------------------------------------------
void VizTrader::workAreaMove(int from, int to)
{

	for(s32 i = 0; i < StackedWidget->count(); i++)
	{
		QWidget* w = StackedWidget->widget(i);
		StackedWidget->removeWidget(w);
	}

	for(s32 i = 0; i < WorkAreaManager->workAreaCount(); i++)
	{
		QPair<QString, VizCWorkArea*> wa = WorkAreaManager->workArea(i);
		StackedWidget->insertWidget(i, wa.second->mdiArea());
	}	

	workAreaChange(to);
}
//-----------------------------------------------------------------------------
void VizTrader::workAreaAdded(int index)
{
	QPair<QString, VizCWorkArea*> wa = WorkAreaManager->workArea(index);
	StackedWidget->insertWidget(index, wa.second->mdiArea());

	workAreaChange(index);
}
//-----------------------------------------------------------------------------
void VizTrader::workAreaRemoved(int index)
{

	QWidget* w = StackedWidget->widget(index);
	Q_ASSERT(w);
	StackedWidget->removeWidget(w);

	if(CurrentWorkAreaIndex == index)
	{
		workAreaNext();
	}
	
}
//-----------------------------------------------------------------------------
void VizTrader::workAreaSave()
{
}
//-----------------------------------------------------------------------------
void VizTrader::workAreaLoad()
{
	WorkAreaManager->newWorkArea("");
	//WorkAreaManager->newWorkArea("");

	/*QPair<QString, VizCWorkArea*> wa = WorkAreaManager->workArea(WorkAreaManager->newWorkArea(""));
	VizCMdiSubWindow* project = new VizCMdiSubWindow(wa.second);
	project->loadProject("PETR3");
	project->show();*/

	//aqui vai carregar toda a area de trabalho, no inicio da aplicacao
	//pelo menos tem que ter 1


	/*//carrega 2 areas de trabalho simulando o Load projeto
	//temporario essa coisa aqui
	for(s32 count = 0; count < 2; count++)
	{
		QString name = tr("Área de Trabalho %1").arg(count);
		s32 index = WorkAreaManager->newWorkArea(name);

		QPair<QString, VizCWorkArea*> wa = WorkAreaManager->workArea(index);
		StackedWidget->insertWidget(index, wa.second->mdiArea());


		workAreaChange(index);


 		VizCMdiSubWindow* project = new VizCMdiSubWindow(WorkArea);
 		project->loadProject("PETR3");
 		project->show();
	}
	*/


	if(ToolManager->isToolChecked(VETC_WINDOW_AUTO_TILE))
	{
		MdiArea->tileSubWindows();
	}

	///////


}
//-----------------------------------------------------------------------------
void VizTrader::closeActiveSubWindow()
{
	MdiArea->closeActiveSubWindow();
}
//-----------------------------------------------------------------------------
void VizTrader::closeAllSubWindows()
{
	MdiArea->closeAllSubWindows();
}
//-----------------------------------------------------------------------------
void VizTrader::tileSubWindows()
{
	MdiArea->tileSubWindows();
}
//-----------------------------------------------------------------------------
void VizTrader::cascadeSubWindows()
{
	MdiArea->cascadeSubWindows();
}
//-----------------------------------------------------------------------------
void VizTrader::activateNextSubWindow()
{
	MdiArea->activateNextSubWindow();
}
//-----------------------------------------------------------------------------
void VizTrader::activatePreviousSubWindow()
{
	MdiArea->activatePreviousSubWindow();
}
//-----------------------------------------------------------------------------
void VizTrader::changeClientStatus()
{
	/*bool isOnline = TP->onlineAndAuthenticate();
	if(isOnline)
	{
		ApplicationNetworkStatus = 3;
	}*/

	if(ApplicationNetworkStatus == 0)
	{
		SystemTrayIcon->setIcon(QIcon(":/VizTrader/Resources/systrayIconOffline.png"));
		SystemTrayIcon->showMessage(ApplicationData->appicationName(), tr("Desconectado"), QSystemTrayIcon::Information, 1 * 1000);
		SystemTrayIcon->setToolTip(tr("%1: Desconectado").arg(ApplicationData->appicationName()));

		UserName->setVisible(false);
		LoadingMovie->setVisible(false);
		ClientStatusButton->setDisabled(true);
		ClientStatusButton->setText(tr("Desconectado"));
		ClientStatusButton->setToolTip(tr(""));
		ClientStatusButton->setToolButtonStyle(Qt::ToolButtonTextOnly);

		LoginAct->setEnabled(true);
		LogoutAct->setEnabled(false);
	}
	else
	if(ApplicationNetworkStatus == 1)
	{
		SystemTrayIcon->setIcon(QIcon(":/VizTrader/Resources/systrayIconOffline.png"));
		SystemTrayIcon->showMessage(ApplicationData->appicationName(), tr("Conectando..."), QSystemTrayIcon::Information, 1 * 1000);
		SystemTrayIcon->setToolTip(tr("%1: Conectando...").arg(ApplicationData->appicationName()));

		UserName->setVisible(false);
		LoadingMovie->setVisible(true);
		ClientStatusButton->setDisabled(true);
		ClientStatusButton->setText(tr("Conectando..."));
		ClientStatusButton->setToolTip(tr(""));
		ClientStatusButton->setToolButtonStyle(Qt::ToolButtonTextOnly);

		LoginAct->setEnabled(false);
		LogoutAct->setEnabled(true);
	}
	else
	if(ApplicationNetworkStatus == 2)
	{
		SystemTrayIcon->setIcon(QIcon(":/VizTrader/Resources/systrayIconOffline.png"));
		SystemTrayIcon->showMessage(ApplicationData->appicationName(), tr("Autenticando..."), QSystemTrayIcon::Information, 1 * 1000);
		SystemTrayIcon->setToolTip(tr("%1: Autenticando...").arg(ApplicationData->appicationName()));

		UserName->setVisible(false);
		LoadingMovie->setVisible(true);
		ClientStatusButton->setDisabled(true);
		ClientStatusButton->setText(tr("Autenticando..."));
		ClientStatusButton->setToolTip(tr(""));
		ClientStatusButton->setToolButtonStyle(Qt::ToolButtonTextOnly);

		LoginAct->setEnabled(false);
		LogoutAct->setEnabled(true);
	}
	else
	if(ApplicationNetworkStatus == 3)
	{
		SystemTrayIcon->setIcon(QIcon(":/VizTrader/Resources/systrayIconOnline.png"));
		SystemTrayIcon->showMessage(ApplicationData->appicationName(), tr("Conectado"), QSystemTrayIcon::Information, 1 * 1000);
		SystemTrayIcon->setToolTip(tr("%1: Conectado").arg(ApplicationData->appicationName()));

		UserName->setVisible(true);
		LoadingMovie->setVisible(false);
		ClientStatusButton->setDisabled(false);
		ClientStatusButton->setText(tr("Conectado"));
		ClientStatusButton->setToolTip(tr("Informações do certificado de segurança"));
		ClientStatusButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

		LoginAct->setEnabled(false);
		LogoutAct->setEnabled(true);
	}

	SystemTrayIcon->show();

}
//-----------------------------------------------------------------------------
void VizTrader::vizServerConnected()
{
	ApplicationNetworkStatus = 3;
	changeClientStatus();
}
//-----------------------------------------------------------------------------
void VizTrader::vizServerDisconnected()
{
	ApplicationNetworkStatus = 0;
	changeClientStatus();
}
//-----------------------------------------------------------------------------
bool VizTrader::checkUserAndPassword(const QString& user, const QString& pass) const
{
	//QRegExp rx1("^(?=.*\d)(?=.*[a-z])(?=.*[A-Z]).{6,40}$");
	
	bool userOk = !user.isEmpty();
	bool passOk = !pass.isEmpty();

	return userOk && passOk;
}
//-----------------------------------------------------------------------------
void VizTrader::vizServerCertificateReady(bool isValid)
{
	bool certificateError = !isValid;

	if(certificateError)
	{
		QLabel* warningLabel = new QLabel;

		QFile certificateError1(":/VizTrader/Resources/CertificateError_Text.txt");
		certificateError1.open(QIODevice::ReadOnly);
		QTextStream inCertificate1(&certificateError1);
		QString Message = inCertificate1.readAll();
		certificateError1.close();

		QFile certificateError2(":/VizTrader/Resources/CertificateError_DetailedText.txt");
		certificateError2.open(QIODevice::ReadOnly);
		QTextStream inCertificate2(&certificateError2);
		QString DetailedText = inCertificate2.readAll();
		certificateError2.close();

		QMessageBox msgBox(QMessageBox::Warning, tr("O certificado de segurança não é confiável."), Message, 0, this);
		msgBox.setTextFormat(Qt::RichText);
		QPushButton* b1 = msgBox.addButton(tr("Continuar mesmo assim"), QMessageBox::AcceptRole);
		QPushButton* b2 = msgBox.addButton(tr("Voltar à segurança"), QMessageBox::RejectRole);
		msgBox.setDetailedText(DetailedText);

		msgBox.setDefaultButton(b1);
		msgBox.setEscapeButton(b2);

		if(msgBox.exec() == QMessageBox::RejectRole)
		{
			logout();
			return;
		}
	}

	UserName->setText("");

	if(AutoConnecting)
	{
		QString name = Settings->value("Network/Account/User", "").toString();
		QString Md5Password = Settings->value("Network/Account/Password", "").toString();

		UserName->setText(name);
		ApplicationNetworkStatus = 2;
		TP->connectToVizServer(name, Md5Password);

		changeClientStatus();

		AutoConnecting = false;

		return;

	}

	if(certificateError)
	{
		QPixmap image(":/VizTrader/Resources/loginLogoCertificateError.png");
		LoginDialogImageLabel->setPixmap(image);
		LoginDialogImageLabel->setToolTip(tr("O certificado de segurança não é confiável"));

		LoginDialog->setStyleSheet("QDialog { background: QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #8c1021, stop: 1 #fefdfe); }");
	}
	else
	{
		QPixmap image(":/VizTrader/Resources/loginLogo.png");
		LoginDialogImageLabel->setPixmap(image);
		LoginDialogImageLabel->setToolTip(tr("Ambiente seguro com proteção SSL"));

		LoginDialog->setStyleSheet("QDialog { background: QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #77a0b8, stop: 1 #fefdfe); }");
	}


	bool SaveAccountInThisComputer = Settings->value("Network/SaveAccountInThisComputer", false).toBool();
	bool AutoConnect = Settings->value("Network/AutoConnect", false).toBool();


	QString dummyPassword = "xxxxxxxx";
	QString Md5Password = Settings->value("Network/Account/Password", "").toString();

	if(SaveAccountInThisComputer)
	{
		LoginDialogNameLineEdit->setText(Settings->value("Network/Account/User", "").toString());
		LoginDialogPasswordLineEdit->setText(dummyPassword);
		LoginDialogSaveAccountInThisComputerCheckBox->setEnabled(true);
		LoginDialogSaveAccountInThisComputerCheckBox->setChecked(true);
		LoginDialogAutoConnectCheckBox->setEnabled(true);
		LoginDialogButtonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
	}
	else
	{
		LoginDialogNameLineEdit->setText("");
		LoginDialogPasswordLineEdit->setText("");
		LoginDialogSaveAccountInThisComputerCheckBox->setEnabled(false);
		LoginDialogSaveAccountInThisComputerCheckBox->setChecked(false);
		LoginDialogAutoConnectCheckBox->setEnabled(false);
		LoginDialogButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	}

	if(AutoConnect)
	{
		LoginDialogAutoConnectCheckBox->setChecked(true);
	}
	else
	{
		LoginDialogAutoConnectCheckBox->setChecked(false);
	}

	LoginDialogNameLineEdit->setFocus();

DIALOG_EXEC:

	if(LoginDialog->exec() == QDialog::Accepted)
	{
		QString name = LoginDialogNameLineEdit->text();
		QString pass = LoginDialogPasswordLineEdit->text();

		bool saveAccount = LoginDialogSaveAccountInThisComputerCheckBox->isEnabled() && LoginDialogSaveAccountInThisComputerCheckBox->checkState() == Qt::Checked;
		bool autoConnect = LoginDialogAutoConnectCheckBox->isEnabled() && LoginDialogAutoConnectCheckBox->checkState() == Qt::Checked;

		Settings->setValue("Network/SaveAccountInThisComputer", saveAccount);
		Settings->setValue("Network/AutoConnect", autoConnect);
		Settings->setValue("Network/Account/User", "");
		Settings->setValue("Network/Account/Password", "");

		if(!checkUserAndPassword(name, pass))
		{
			QMessageBox msgBox(QMessageBox::Warning, tr("Erro"), tr("Usuário ou senha inválido."), 0, this);
			msgBox.addButton(QMessageBox::Ok);
			msgBox.exec();
			
			goto DIALOG_EXEC;
		}


		if(pass != dummyPassword)
		{
			Md5Password = core::md5(pass);
		}
		
		if(saveAccount)
		{
			Settings->setValue("Network/Account/User", name);
			Settings->setValue("Network/Account/Password", Md5Password);
		}

		UserName->setText(name);
		ApplicationNetworkStatus = 2;
		TP->connectToVizServer(name, Md5Password);

		changeClientStatus();
	}
	else
	{
		logout();
	}
}
//-----------------------------------------------------------------------------
void VizTrader::createLoginDialog()
{
	LoginDialog = new QDialog();

	QFont defaultFont;
	defaultFont.fromString(Settings->value("DefaultFont", QApplication::font()).toString());
	LoginDialog->setFont(defaultFont);
	LoginDialog->setWindowTitle (tr("Conectar"));
	LoginDialog->setWindowIcon(QIcon(":/VizTrader/Resources/windowIcon.png"));

	QVBoxLayout* layoutMain = new QVBoxLayout;

	layoutMain->setContentsMargins(0, 20, 0, 0);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->setSizeConstraint(QLayout::SetFixedSize);
	layout->setContentsMargins(30, 20, 30, 20);

	QHBoxLayout* layoutButtons = new QHBoxLayout;

	QFormLayout* formLayout = new QFormLayout;
	formLayout->setFormAlignment(Qt::AlignRight);
	formLayout->setSpacing(15);

	QPixmap image(":/VizTrader/Resources/loginLogo.png");
	LoginDialogImageLabel = new QLabel(LoginDialog);
	LoginDialogImageLabel->setPixmap(image);
	LoginDialogImageLabel->setToolTip(tr("Ambiente seguro com proteção SSL"));

	LoginDialog->setStyleSheet("QDialog { background: QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #77a0b8, stop: 1 #fefdfe); }");

	LoginDialogNameLineEdit = new QLineEdit(LoginDialog);
	LoginDialogPasswordLineEdit = new QLineEdit(LoginDialog);
	LoginDialogNameLineEdit->setMaxLength(255);
	LoginDialogPasswordLineEdit->setMaxLength(255);
	LoginDialogPasswordLineEdit->setEchoMode(QLineEdit::Password);
	LoginDialogSaveAccountInThisComputerCheckBox = new QCheckBox(tr("Salvar meus dados neste computador"), LoginDialog);
	LoginDialogAutoConnectCheckBox = new QCheckBox(tr("Conectar automaticamente ao iniciar"), LoginDialog);

	LoginDialogSaveAccountInThisComputerCheckBox->setEnabled(false);
	LoginDialogAutoConnectCheckBox->setEnabled(false);

	connect(LoginDialogNameLineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(loginDialogNameLineEditTextEdited(const QString&)));
	connect(LoginDialogPasswordLineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(loginDialogPasswordLineEditTextEdited(const QString&)));
	connect(LoginDialogSaveAccountInThisComputerCheckBox, SIGNAL(stateChanged(int)), this, SLOT(loginDialogSaveAccountInThisComputerCheckBoxStateChanged(int)));

	QLabel* forgotPasswordLabel = new QLabel(QString("<a href=\"%1\">Esqueceu a senha?</a>").arg(ApplicationData->forgotPasswordURL()), LoginDialog);
	forgotPasswordLabel->setTextFormat(Qt::RichText);
	forgotPasswordLabel->setOpenExternalLinks(true);
	forgotPasswordLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

	QPushButton* certificateErrorButton = new QPushButton(tr("Informações do Certificado..."));
	connect(certificateErrorButton, SIGNAL(clicked()), this, SLOT(certificateInfo()));
	LoginDialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	connect(LoginDialogButtonBox, SIGNAL(accepted()), LoginDialog, SLOT(accept()));
	connect(LoginDialogButtonBox, SIGNAL(rejected()), LoginDialog, SLOT(reject()));

	layoutMain->addWidget(LoginDialogImageLabel);
	layoutMain->addLayout(layout);
	layout->addLayout(formLayout);
	formLayout->addRow(tr("&Usuário:"), LoginDialogNameLineEdit);
	formLayout->addRow(tr("&Senha:"), LoginDialogPasswordLineEdit);
	formLayout->addRow("", LoginDialogSaveAccountInThisComputerCheckBox);
	formLayout->addRow("", LoginDialogAutoConnectCheckBox);
	formLayout->addRow("", forgotPasswordLabel);

	layoutButtons->addWidget(certificateErrorButton);
	layoutButtons->addWidget(LoginDialogButtonBox);

	layout->addSpacing(30);
	layout->addLayout(layoutButtons);
	LoginDialog->setLayout(layoutMain);
	LoginDialog->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
}
//-----------------------------------------------------------------------------
void VizTrader::loginDialogNameLineEditTextEdited(const QString& text)
{
	QString name = LoginDialogNameLineEdit->text();
	QString pass = LoginDialogPasswordLineEdit->text();
	if(!name.isEmpty() && !pass.isEmpty())
	{
		LoginDialogSaveAccountInThisComputerCheckBox->setEnabled(true);
		LoginDialogButtonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
	}
	else
	{
		LoginDialogSaveAccountInThisComputerCheckBox->setChecked(false);
		LoginDialogSaveAccountInThisComputerCheckBox->setEnabled(false);
		LoginDialogButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	}
}
//-----------------------------------------------------------------------------
void VizTrader::loginDialogPasswordLineEditTextEdited(const QString& text)
{
	QString name = LoginDialogNameLineEdit->text();
	QString pass = LoginDialogPasswordLineEdit->text();
	if(!name.isEmpty() && !pass.isEmpty())
	{
		LoginDialogSaveAccountInThisComputerCheckBox->setEnabled(true);
		LoginDialogButtonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
	}
	else
	{
		LoginDialogSaveAccountInThisComputerCheckBox->setChecked(false);
		LoginDialogSaveAccountInThisComputerCheckBox->setEnabled(false);
		LoginDialogButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	}

}
//-----------------------------------------------------------------------------
void VizTrader::loginDialogSaveAccountInThisComputerCheckBoxStateChanged(int state)
{
	
	if(LoginDialogSaveAccountInThisComputerCheckBox->isChecked())
	{
		LoginDialogAutoConnectCheckBox->setEnabled(true);
	}
	else
	{
		LoginDialogAutoConnectCheckBox->setChecked(false);
		LoginDialogAutoConnectCheckBox->setEnabled(false);

	}

}
//-----------------------------------------------------------------------------
void VizTrader::vizServerError(VizCTP::VizTPError vetperror)
{
	QString Message;

	switch(vetperror)
	{
		case VizCTP::DIFFERENT_PROTOCOL_VERSION_ERROR:
		{
			Message = tr("\nFalha na comunicação:\n\nVersão do protocolo de comunicação desatualizada.\n");
			break;
		}
		case VizCTP::AUTHENTICATION_ERROR:
		{
			Message = tr("\nFalha na autenticação:\n\nO nome de usuário e senha digitados são incorretos.\n");
			break;
		}
		case VizCTP::ACCOUNT_EXPIRED_ERROR:
		{
			Message = tr("\nFalha na autenticação:\n\nSua conta expirou.\n");
			break;
		}
		case VizCTP::CERTIFICATE_INVALID_ERROR:
		{
			Message = tr("\nFalha na autenticação:\n\nCertificado de segurança inválido.\n");
			break;
		}
		case VizCTP::AUTHENTICATION_WITHOUT_ENCRYPTION_ERROR:
		{
			Message = tr("\nFalha na autenticação:\n\nSem conexão.\n");
			break;
		}
		case VizCTP::CONNECTION_REFUSED_ERROR:
		{
			Message = tr("\nFalha na conexão:\n\nA Conexão foi recusada pelo servidor (ou tempo limite esgotado).\n");
			break;
		}
		case VizCTP::HOST_NOT_FOUND_ERROR:
		{
			Message = tr("\nFalha na conexão:\n\nO servidor não foi encontrado.\n");
			break;
		}
		case VizCTP::SOCKET_TIMEOUT_ERROR:
		{
			Message = tr("\nFalha na operação:\n\nTempo esgotado.\n");
			break;
		}
		case VizCTP::NETWORK_ERROR:
		{
			Message = tr("\nFalha na rede:\n\nOcorreu um erro com a rede (por exemplo, o cabo de rede foi acidentalmente desconectado).\n");
			break;
		}
		case VizCTP::SSL_HANDSHAKE_ERROR:
		{
			Message = tr("\nFalha na autenticação:\n\nOcorreu um erro na operação Handshake do protocolo SSL.\n");
			break;
		}
		case VizCTP::PROXY_AUTHENTICATION_REQUIRED_ERROR:
		{
			Message = tr("\nFalha no proxy:\n\nO proxy usado requer autenticação.\n");
			break;
		}
		case VizCTP::PROXY_CONNECTION_REFUSED_ERROR:
		{
			Message = tr("\nFalha no proxy:\n\nNão foi possível contactar o servidor proxy porque a conexão com o servidor que foi negado.\n");
			break;
		}
		case VizCTP::PROXY_CONNECTION_CLOSED_ERROR:
		{
			Message = tr("\nFalha no proxy:\n\nA conexão com o servidor proxy foi fechada inesperadamente.\n");
			break;
		}
		case VizCTP::PROXY_CONNECTION_TIMEOUT_ERROR:
		{
			Message = tr("\nFalha no proxy:\n\nO tempo limite da conexão com o servidor proxy esgotadou ou o servidor proxy parou de responder na fase de autenticação.\n");
			break;
		}
		case VizCTP::PROXY_NOT_FOUND_ERROR:
		{
			Message = tr("\nFalha no proxy:\n\nO endereço proxy configurado na aplicação não foi encontrado.\n");
			break;
		}
		case VizCTP::PROXY_PROTOCOL_ERROR:
		{
			Message = tr("\nFalha no proxy:\n\nErro no protocolo do proxy.\n");
			break;
		}
		case VizCTP::SOCKET_ERROR:
		{
			Message = tr("\nFalha na comunicação:\n\nErro no protocolo de rede.\n");
			break;
		}
		case VizCTP::UNKNOWN_ERROR:
		default:
		{
			Message = tr("\nFalha na comunicação:\n\nErro não identificado.\n");
			break;
		}
	};

	if(ApplicationNetworkStatus == 1 || ApplicationNetworkStatus == 2)
	{
		ApplicationNetworkStatus = 0;
		changeClientStatus();
	}

	QMessageBox msgBox(QMessageBox::Warning, tr("Erro na comunicação com o servidor."), Message, 0, this);
	msgBox.exec();
}
//-----------------------------------------------------------------------------
void VizTrader::login()
{
	ApplicationNetworkStatus = 1;
	changeClientStatus();
	
	TP->getCertificate();
}
//-----------------------------------------------------------------------------
void VizTrader::logout()
{
	/*ApplicationNetworkStatus = 0;
	changeClientStatus();
	*/
	TP->disconnectFromVizServer();
}
//-----------------------------------------------------------------------------
void VizTrader::certificateInfo()
{

	CertificateInformation->setCertificateChainAndCiphers(TP->chainOfCertificates(), TP->ciphers());
	CertificateInformation->exec();

}
//-----------------------------------------------------------------------------
void VizTrader::open()
{
	OpenSecurityWidget->setWindowTitle(tr("Abrir Ativo"));
	if(OpenSecurityWidget->exec() == QDialog::Rejected)
	{
		return;
	}

	VizCMdiSubWindow* project = findMdiSubWindow(OpenSecurityWidget->securityId());
	if(!project)
	{
		VizCMdiSubWindow* project = new VizCMdiSubWindow(WorkArea);
		project->loadProject(OpenSecurityWidget->securityId());
		project->show();
	}
	else
	{
		setActiveMdiSubWindow(project);
	}
	

	if(ToolManager->isToolChecked(VETC_WINDOW_AUTO_TILE))
	{
		MdiArea->tileSubWindows();
	}
}
//-----------------------------------------------------------------------------
void VizTrader::save()
{
	
	/*if (activeMdiChild() && activeMdiChild()->save())
		statusBar()->showMessage(tr("File saved"), 2000);
		*/

}
//-----------------------------------------------------------------------------
void VizTrader::saveAll()
{
}
//-----------------------------------------------------------------------------
void VizTrader::about()
{
	QDialog* dialog = new QDialog();

	QFont defaultFont;
	defaultFont.fromString(Settings->value("DefaultFont", QApplication::font()).toString());
	dialog->setFont(defaultFont);
	
	dialog->setWindowTitle (tr("Sobre o %1").arg(ApplicationData->appicationName()));
	dialog->setWindowIcon(QIcon(":/VizTrader/Resources/windowIcon.png"));
	 
	QVBoxLayout* layout = new QVBoxLayout;
	layout->setSizeConstraint(QLayout::SetFixedSize);
	layout->setSpacing(0);
	layout->setMargin(0);
	QPixmap image(":/VizTrader/Resources/AboutScreen.png");

	QLabel* imageLabel = new QLabel(dialog);
	imageLabel->setPixmap(image);

	QString text = 
		QString(tr(
		"<p>%1 %2 %3</p>\
		<p>%4</p>\
		<p><a href=\"%5\">%5</a></p>\
		<p><a href=\"mailto:%6\">%6</a></p><br/>\
		"))
		.arg(ApplicationData->organizationName())
		.arg(ApplicationData->appicationName())
		.arg(ApplicationData->applicationVersion().toShortString())
		.arg(ApplicationData->applicationVersion().toLongString())
		.arg(ApplicationData->homepageAddress())
		.arg(ApplicationData->bugReportEmailAddress())
		//.arg(ApplicationData->CopyrightStatement)
		;

	QTextBrowser* licenseText = new QTextBrowser(dialog);
	licenseText->insertHtml(text);
	licenseText->insertHtml(ApplicationData->licenseText());
	licenseText->moveCursor(QTextCursor::Start);
	licenseText->setOpenExternalLinks(true);
	licenseText->setTextInteractionFlags(Qt::TextBrowserInteraction);
	licenseText->setReadOnly(true);
	licenseText->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	licenseText->setStyleSheet("QTextEdit { color: gray; background-color: white; border:1; }");
	//licenseText->setGeometry(286, 68, 286, 241);
	licenseText->setGeometry(286, 28, 286, 281);
	
		
	layout->addWidget(imageLabel);
	
	dialog->setLayout(layout);
	dialog->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
	dialog->exec();

}
//-----------------------------------------------------------------------------
void VizTrader::updateMenus()
{
	bool hasMdiChild = (activeMdiSubWindow() != 0);
	CloseAct->setEnabled(hasMdiChild);
	CloseAllAct->setEnabled(hasMdiChild);
	SaveAct->setEnabled(hasMdiChild);
	SaveAllAct->setEnabled(hasMdiChild);
	TileAct->setEnabled(hasMdiChild);
	CascadeAct->setEnabled(hasMdiChild);
	NextAct->setEnabled(hasMdiChild);
	PreviousAct->setEnabled(hasMdiChild);
	SeparatorAct->setVisible(hasMdiChild);
}
//-----------------------------------------------------------------------------
void VizTrader::updateWindowMenu()
{
	WindowMenu->clear();
	WindowMenu->addAction(CloseAct);
	WindowMenu->addAction(CloseAllAct);
	WindowMenu->addSeparator();
	WindowMenu->addAction(TileAct);
	WindowMenu->addAction(CascadeAct);
	WindowMenu->addAction(AutoAjustWindowAct);
	WindowMenu->addSeparator();
	WindowMenu->addAction(NextAct);
	WindowMenu->addAction(PreviousAct);
	WindowMenu->addAction(SeparatorAct);

	QList<QMdiSubWindow *> windows = MdiArea->subWindowList();
	SeparatorAct->setVisible(!windows.isEmpty());

	for(int i = 0; i < windows.size(); ++i)
	{
		VizCMdiSubWindow* child = qobject_cast<VizCMdiSubWindow*>(windows.at(i));
		if(child)
		{
			QAction* action  = WindowMenu->addAction(child->windowTitle());
			action->setCheckable(true);
			action->setChecked(child == activeMdiSubWindow());
			connect(action, SIGNAL(triggered()), WindowMapper, SLOT(map()));
			WindowMapper->setMapping(action, windows.at(i));
		}
	}
}
//-----------------------------------------------------------------------------
void VizTrader::setActiveSubWindowByWindowMenu(QWidget* subWindow)
{
	if(!subWindow)
	{
		return;
	}
	
	VizCMdiSubWindow* sw = qobject_cast<VizCMdiSubWindow*>(subWindow);
	Q_ASSERT(sw);

	MdiArea->setActiveSubWindow(sw);
}
//-----------------------------------------------------------------------------
void VizTrader::createActions()
{

	LoginAct = new QAction(QIcon(":/VizTrader/Resources/login.png"), tr("&Conectar"), this);
	//LoginAct->setShortcuts(QKeySequence::lo);
	LoginAct->setStatusTip(tr("Conecta no servidor"));
	connect(LoginAct, SIGNAL(triggered()), this, SLOT(login()));

	LogoutAct = new QAction(QIcon(":/VizTrader/Resources/logout.png"), tr("&Desconectar"), this);
	//LogoutAct->setShortcuts(QKeySequence::lo);
	LogoutAct->setStatusTip(tr("Desconecta do servidor"));
	connect(LogoutAct, SIGNAL(triggered()), this, SLOT(logout()));

	/*CertificateInfoAct = new QAction(QIcon(":/VizTrader/Resources/certificateKey.png"), tr("&Informação de Segurança"), this);
	CertificateInfoAct->setStatusTip(tr("Mostra Informações sobre o Certificado de Segurança"));
	connect(CertificateInfoAct, SIGNAL(triggered()), this, SLOT(certificateInfo()));
	*/
	

	OpenAct = new QAction(QIcon(":/VizTrader/Resources/open.png"), tr("&Abrir"), this);
	OpenAct->setShortcuts(QKeySequence::Open);
	OpenAct->setStatusTip(tr("Abre um papel"));
	connect(OpenAct, SIGNAL(triggered()), this, SLOT(open()));


	SaveAct = new QAction(QIcon(":/VizTrader/Resources/save.png"), tr("&Salvar"), this);
	SaveAct->setShortcuts(QKeySequence::Save);
	SaveAct->setStatusTip(tr("Salva um papel no disco"));
	connect(SaveAct, SIGNAL(triggered()), this, SLOT(save()));

	SaveAllAct = new QAction(QIcon(":/VizTrader/Resources/saveAll.png"), tr("Salvar &Tudo"), this);
	SaveAllAct->setShortcuts(QKeySequence::SaveAs);
	SaveAllAct->setStatusTip(tr("Salva todos os papeis"));
	connect(SaveAllAct, SIGNAL(triggered()), this, SLOT(saveAll()));

	ExitAct = new QAction(tr("&Sair"), this);
	ExitAct->setShortcut(tr("Ctrl+Q"));
	ExitAct->setStatusTip(tr("Sair"));
	connect(ExitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

	CloseAct = new QAction(tr("&Fechar"), this);
	CloseAct->setShortcut(QKeySequence::Close);
	CloseAct->setStatusTip(tr("Fecha janela atual"));
	connect(CloseAct, SIGNAL(triggered()), this, SLOT(closeActiveSubWindow()));


	CloseAllAct = new QAction(tr("Fec&har todas"), this);
	CloseAllAct->setStatusTip(tr("Fecha todas as janelas"));
	connect(CloseAllAct, SIGNAL(triggered()), this, SLOT(closeAllSubWindows()));

	TileAct = new QAction(QIcon(":/VizTrader/Resources/TileWindow.png"), tr("&Organizar"), this);
	TileAct->setStatusTip(tr("Organiza as janelas"));
	connect(TileAct, SIGNAL(triggered()), this, SLOT(tileSubWindows()));

	CascadeAct = new QAction(QIcon(":/VizTrader/Resources/CascadeWindow.png"), tr("&Cascata"), this);
	CascadeAct->setStatusTip(tr("Janelas em cascata"));
	connect(CascadeAct, SIGNAL(triggered()), this, SLOT(cascadeSubWindows()));

	AutoAjustWindowAct = new QAction(QIcon(":/VizTrader/Resources/AutoAjustWindow.png"), tr("&Auto Ajuste"), this);
	AutoAjustWindowAct->setStatusTip(tr("Janelas auto-ajustadas"));
	AutoAjustWindowAct->setCheckable(true);
	AutoAjustWindowAct->setChecked(true);
	connect(AutoAjustWindowAct, SIGNAL(triggered()), this, SLOT(autoAjustWindow()));

	NextAct = new QAction(tr("Pró&xima"), this);
	NextAct->setShortcuts(QKeySequence::NextChild);
	NextAct->setStatusTip(tr("Seleciona a próxima janela"));
	connect(NextAct, SIGNAL(triggered()), this, SLOT(activateNextSubWindow()));

	PreviousAct = new QAction(tr("&Anterior"), this);
	PreviousAct->setShortcuts(QKeySequence::PreviousChild);
	PreviousAct->setStatusTip(tr("Seleciona a janela anterior"));
	connect(PreviousAct, SIGNAL(triggered()), this, SLOT(activatePreviousSubWindow()));

	WorkAreaManageAct = new QAction(QIcon(":/VizTrader/Resources/desktop-32.png"), tr("&Gerenciar"), this);
	WorkAreaManageAct->setStatusTip(tr("Gerencia as múltiplas área de trabalho"));
	connect(WorkAreaManageAct, SIGNAL(triggered()), this, SLOT(workAreaManagement()));

	
	WorkAreaNextAct = new QAction(QIcon(":/VizTrader/Resources/go-next.png"), tr("Pró&xima"), this);
	WorkAreaNextAct->setStatusTip(tr("Seleciona a próxima área de trabalho"));
	connect(WorkAreaNextAct, SIGNAL(triggered()), this, SLOT(workAreaNext()));

	WorkAreaPreviousAct = new QAction(QIcon(":/VizTrader/Resources/go-previous.png"), tr("&Anterior"), this);
	WorkAreaPreviousAct->setStatusTip(tr("Seleciona a área de trabalho anterior"));
	connect(WorkAreaPreviousAct, SIGNAL(triggered()), this, SLOT(workAreaPrev()));


	SeparatorAct = new QAction(this);
	SeparatorAct->setSeparator(true);

	AboutAct = new QAction(QIcon(":/VizTrader/Resources/About.png"), tr("&Sobre o VizTrader"), this);
	AboutAct->setStatusTip(tr("Sobre o %1").arg(ApplicationData->appicationName()));
	connect(AboutAct, SIGNAL(triggered()), this, SLOT(about()));

	ShowHideStatusBarAct = new QAction(tr("&Barra de status visível"), this);
	ShowHideStatusBarAct->setStatusTip(tr("Alterna visibilidade da barra de status"));
	ShowHideStatusBarAct->setCheckable(true);
	ShowHideStatusBarAct->setChecked(true);
	connect(ShowHideStatusBarAct, SIGNAL(triggered()), this, SLOT(showHideStatusBar()));


	FullScreenAct = new QAction(tr("Visualização em &tela cheia"), this);
	FullScreenAct->setStatusTip(tr("Visualização em tela cheia"));
	FullScreenAct->setCheckable(true);
	connect(FullScreenAct, SIGNAL(triggered()), this, SLOT(fullScreen()));

	FullDesktopAct = new QAction(tr("Visualização em todo &desktop"), this);
	FullDesktopAct->setStatusTip(tr("Visualização em todo desktop"));
	FullDesktopAct->setCheckable(true);
	connect(FullDesktopAct, SIGNAL(triggered()), this, SLOT(fullDesktop()));

	
	ShowHideDataWindowAct = new QAction(tr("Visualização de dados"), this);
	ShowHideDataWindowAct->setStatusTip(tr("Alterna a visibilidade da ferramenta de visualização de dados"));
	ShowHideDataWindowAct->setCheckable(true);
	connect(ShowHideDataWindowAct, SIGNAL(triggered()), this, SLOT(showHideDataWindow()));

	RefreshAct = new QAction(tr("Atualiza dados"), this);
	RefreshAct->setShortcuts(QKeySequence::Refresh);
	RefreshAct->setStatusTip(tr("Atualiza dados dos papeis no painel atual"));
	connect(RefreshAct, SIGNAL(triggered()), this, SLOT(refresh()));
	
	

	//tools

	SelectionToolAct = new QAction(QIcon(":/VizTrader/Resources/SelectionTool.png"), tr("Ferramenta de &Seleção"), this);
	SelectionToolAct->setStatusTip(tr("Ferramenta de Seleção"));
	SelectionToolAct->setCheckable(true);
	connect(SelectionToolAct, SIGNAL(triggered()), this, SLOT(selectionTool()));

	CrossHairsAct = new QAction(QIcon(":/VizTrader/Resources/Crosshairs.png"), tr("Crosshairs"), this);
	CrossHairsAct->setStatusTip(tr("Crosshairs"));
	CrossHairsAct->setCheckable(true);
	connect(CrossHairsAct, SIGNAL(triggered()), this, SLOT(crossHairsTool()));

	ZoomBoxAct = new QAction(QIcon(":/VizTrader/Resources/ZoomBox.png"), tr("Caixa de zoom"), this);
	ZoomBoxAct->setStatusTip(tr("Caixa de zoom"));
	ZoomBoxAct->setCheckable(true);
	connect(ZoomBoxAct, SIGNAL(triggered()), this, SLOT(zoomBoxTool()));

	PanAct = new QAction(QIcon(":/VizTrader/Resources/Pan.png"), tr("Mão"), this);
	PanAct->setStatusTip(tr("Ferramenta mão"));
	PanAct->setCheckable(true);
	connect(PanAct, SIGNAL(triggered()), this, SLOT(panTool()));


	HorizontalLineAct = new QAction(QIcon(":/VizTrader/Resources/HorizontalLine.png"), tr("Linha &Horizontal"), this);
	HorizontalLineAct->setStatusTip(tr("Adiciona Linha Horizontal"));
	HorizontalLineAct->setCheckable(true);
	connect(HorizontalLineAct, SIGNAL(triggered()), this, SLOT(horizontalLine()));

	TrendLineAct = new QAction(QIcon(":/VizTrader/Resources/TrendLine.png"), tr("&Trend Line"), this);
	TrendLineAct->setStatusTip(tr("Adiciona Trend Line"));
	TrendLineAct->setCheckable(true);
	connect(TrendLineAct, SIGNAL(triggered()), this, SLOT(trendLine()));

	SemiLogTrendLineAct = new QAction(QIcon(":/VizTrader/Resources/SemiLogTrendLine.png"), tr("&Semi-log Trend Line"), this);
	SemiLogTrendLineAct->setStatusTip(tr("Adiciona Semi-log Trend Line"));
	SemiLogTrendLineAct->setCheckable(true);
	connect(SemiLogTrendLineAct, SIGNAL(triggered()), this, SLOT(semiLogTrendLine()));

	
	VerticalLineAct = new QAction(QIcon(":/VizTrader/Resources/VerticalLine.png"), tr("Linha &Vertical"), this);
	VerticalLineAct->setStatusTip(tr("Adiciona Linha Vertical"));
	VerticalLineAct->setCheckable(true);
	connect(VerticalLineAct, SIGNAL(triggered()), this, SLOT(verticalLine()));


	
	EditActions->CutAction = new QAction(QIcon(":/VizTrader/Resources/editcut.png"), tr("Recor&tar"), this);
	EditActions->CutAction->setShortcut(QKeySequence::Cut);
	EditActions->CutAction->setStatusTip(tr("Recortar"));
	
	EditActions->CopyAction = new QAction(QIcon(":/VizTrader/Resources/editcopy.png"), tr("&Copiar"), this);
	EditActions->CopyAction->setShortcut(QKeySequence::Copy);
	EditActions->CopyAction->setStatusTip(tr("Copiar"));

	EditActions->PasteAction = new QAction(QIcon(":/VizTrader/Resources/editpaste.png"), tr("Co&lar"), this);
	EditActions->PasteAction->setShortcut(QKeySequence::Paste);
	EditActions->PasteAction->setStatusTip(tr("Colar"));

	EditActions->DeleteAction = new QAction(QIcon(":/VizTrader/Resources/delete.png"), tr("E&xcluir"), this);
	EditActions->DeleteAction->setShortcut(QKeySequence::Delete);
	EditActions->DeleteAction->setStatusTip(tr("Excluir"));

	EditActions->UndoAction = new QAction(QIcon(":/VizTrader/Resources/editundo.png"), tr("Desfa&zer"), this);
	EditActions->UndoAction->setShortcut(QKeySequence("Ctrl+Z"));
	EditActions->UndoAction->setStatusTip(tr("Desfazer"));

	EditActions->RedoAction = new QAction(QIcon(":/VizTrader/Resources/editredo.png"), tr("Re&fazer"), this);
	EditActions->RedoAction->setShortcut(QKeySequence("Shift+Ctrl+Z"));
	EditActions->RedoAction->setStatusTip(tr("Refazer"));

	EditActions->UndoAction->setEnabled(false);
	EditActions->RedoAction->setEnabled(false);
	EditActions->CutAction->setEnabled(false);
	EditActions->CopyAction->setEnabled(false);
	EditActions->PasteAction->setEnabled(false);
	EditActions->DeleteAction->setEnabled(false);

	connect(EditActions->UndoAction, SIGNAL(triggered()), this, SLOT(undo()));
	connect(EditActions->RedoAction, SIGNAL(triggered()), this, SLOT(redo()));
	connect(EditActions->CutAction, SIGNAL(triggered()), this, SLOT(cut()));
	connect(EditActions->CopyAction, SIGNAL(triggered()), this, SLOT(copy()));
	connect(EditActions->PasteAction, SIGNAL(triggered()), this, SLOT(paste()));
	connect(EditActions->DeleteAction, SIGNAL(triggered()), this, SLOT(remove()));

	//connect(PropertyEditor, SIGNAL(objectAvailable()), this, SLOT(updateEditMenu()));
	connect(Clipboard, SIGNAL(dataChanged()), this, SLOT(clipboardDataChanged()));


	ShowSettingsDialogAct = new QAction(tr("Preferências"), this);
	ShowSettingsDialogAct->setStatusTip(tr("Ajusta preferências da aplicação"));
	connect(ShowSettingsDialogAct, SIGNAL(triggered()), ApplicationSettings, SLOT(loadAndShow()));
	
}
//-----------------------------------------------------------------------------
void VizTrader::updateEditMenu()
{

	VizIGraphicsItem* GraphicsItem = qobject_cast<VizIGraphicsItem*>(WorkArea->selectedVizOBject());

	VizCGraphicsPanel* GraphicsPanel = WorkArea->selectedGraphicsPanel();


	bool CutActEnabled = GraphicsItem ? true : false;
	bool CopyActEnabled = GraphicsItem || GraphicsPanel ? true : false;
	bool PasteActEnabled = GraphicsPanel && Clipboard->graphicsItem();
	bool DeleteActEnabled = GraphicsItem ? true : false;

	//bool RefreshActEnabled = GraphicsPanel ? true : false;


	EditActions->CutAction->setEnabled(CutActEnabled);
	EditActions->CopyAction->setEnabled(CopyActEnabled);
	EditActions->PasteAction->setEnabled(PasteActEnabled);
	EditActions->DeleteAction->setEnabled(DeleteActEnabled);

	//RefreshAct->setEnabled(RefreshActEnabled);
}
//-----------------------------------------------------------------------------
void VizTrader::createMenus()
{
	/*
	WorkAreaMenu = menuBar()->addMenu(tr("Área de &Trabalho"));
	WorkAreaMenu->addAction(WorkAreaManageAct);
	WorkAreaMenu->addAction(WorkAreaPreviousAct);
	WorkAreaMenu->addAction(WorkAreaNextAct);
	*/
	
	FileMenu = menuBar()->addMenu(tr("&Arquivo"));
	FileMenu->addAction(LoginAct);
	FileMenu->addAction(LogoutAct);
	FileMenu->addSeparator();

	WorkAreaMenu = FileMenu->addMenu(tr("Área de &Trabalho"));
	WorkAreaMenu->addAction(WorkAreaManageAct);
	WorkAreaMenu->addAction(WorkAreaPreviousAct);
	WorkAreaMenu->addAction(WorkAreaNextAct);


	FileMenu->addSeparator();
	FileMenu->addAction(OpenAct);
	FileMenu->addSeparator();
	FileMenu->addAction(SaveAct);
	FileMenu->addAction(SaveAllAct);
	FileMenu->addSeparator();
	FileMenu->addAction(CloseAct);
	FileMenu->addAction(CloseAllAct);
	FileMenu->addSeparator();
	FileMenu->addAction(ExitAct);

	EditMenu = menuBar()->addMenu(tr("&Editar"));

	EditMenu->addAction(EditActions->UndoAction);
	EditMenu->addAction(EditActions->RedoAction);
	EditMenu->addSeparator();
	EditMenu->addAction(EditActions->CutAction);
	EditMenu->addAction(EditActions->CopyAction);
	EditMenu->addAction(EditActions->PasteAction);
	EditMenu->addAction(EditActions->DeleteAction);
	EditMenu->addSeparator();
	EditMenu->addAction(RefreshAct);
	EditMenu->addSeparator();
	EditMenu->addAction(ShowSettingsDialogAct);

	ViewMenu = menuBar()->addMenu(tr("E&xibir"));
	ViewMenu->addAction(FullScreenAct);
	ViewMenu->addAction(FullDesktopAct);
	ViewMenu->addSeparator();


	WindowMenu = menuBar()->addMenu(tr("&Janela"));
	//updateWindowMenu();
	connect(WindowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));



	HelpMenu = menuBar()->addMenu(tr("Aj&uda"));
	HelpMenu->addAction(AboutAct);

}
//-----------------------------------------------------------------------------
void VizTrader::createToolBars()
{
	WorkAreaToolBar = addToolBar(tr("Área de trabalho"));
	WorkAreaToolBar->setObjectName("WorkAreaToolBar");
	WorkAreaToolBar->addAction(WorkAreaManageAct);
	WorkAreaToolBar->addAction(WorkAreaPreviousAct);
	WorkAreaToolBar->addAction(WorkAreaNextAct);
	
	LoginToolBar = addToolBar(tr("Conexão"));
	LoginToolBar->setObjectName("LoginToolBar");
	LoginToolBar->addAction(LoginAct);
	LoginToolBar->addAction(LogoutAct);

	StandardToolBar = addToolBar(tr("Padrão"));
	StandardToolBar->setObjectName("StandardToolBar");
	//StandardToolBar->addAction(LoginAct);
	//StandardToolBar->addAction(LogoutAct);
	//StandardToolBar->addSeparator();
	////StandardToolBar->addAction(WorkAreaManageAct);
	//StandardToolBar->addAction(WorkAreaPreviousAct);
	//StandardToolBar->addAction(WorkAreaNextAct);
	//StandardToolBar->addSeparator();
	StandardToolBar->addAction(OpenAct);
	StandardToolBar->addSeparator();
	StandardToolBar->addAction(SaveAct);
	StandardToolBar->addAction(SaveAllAct);
	StandardToolBar->addSeparator();
	StandardToolBar->addAction(EditActions->UndoAction);
	StandardToolBar->addAction(EditActions->RedoAction);
	StandardToolBar->addSeparator();
	StandardToolBar->addAction(EditActions->CutAction);
	StandardToolBar->addAction(EditActions->CopyAction);
	StandardToolBar->addAction(EditActions->PasteAction);
	StandardToolBar->addAction(EditActions->DeleteAction);
	//StandardToolBar->addSeparator();
	//StandardToolBar->addAction(RefreshAct);


	//StandardToolBar->addWidget(OpenSecurityWidget->completerLineEdit());
	//StandardToolBar->setStatusTip(tr("Alterna visibilidade da barra de ferramentas padrão"));
	//StandardToolBar->setToolTip(tr("Alterna visibilidade da barra de ferramentas padrão"));

	NavigationToolBar = new QToolBar(tr("Navegação"));
	NavigationToolBar->setObjectName("NavigationToolBar");
	addToolBar(Qt::LeftToolBarArea, NavigationToolBar);
	NavigationToolBar->addAction(SelectionToolAct);
	NavigationToolBar->addAction(CrossHairsAct);
	NavigationToolBar->addSeparator();
	NavigationToolBar->addAction(ZoomBoxAct);
	NavigationToolBar->addAction(PanAct);

	TreendLinesToolBar = new QToolBar(tr("Trendlines"));
	TreendLinesToolBar->setObjectName("TreendLinesToolBar");
	addToolBar(Qt::LeftToolBarArea, TreendLinesToolBar);
	TreendLinesToolBar->addAction(HorizontalLineAct);
	TreendLinesToolBar->addAction(VerticalLineAct);
	TreendLinesToolBar->addAction(TrendLineAct);
	TreendLinesToolBar->addAction(SemiLogTrendLineAct);

	WindowToolBar = addToolBar(tr("Janelas"));
	WindowToolBar->setObjectName("WindowToolBar");
	WindowToolBar->addAction(TileAct);
	WindowToolBar->addAction(CascadeAct);
	WindowToolBar->addAction(AutoAjustWindowAct);


	QMenu* tb = ViewMenu->addMenu(tr("Barra de ferramentas"));

	tb->addAction(LoginToolBar->toggleViewAction());
	tb->addAction(WorkAreaToolBar->toggleViewAction());
	tb->addAction(StandardToolBar->toggleViewAction());
	tb->addAction(NavigationToolBar->toggleViewAction());
	tb->addAction(TreendLinesToolBar->toggleViewAction());
	tb->addAction(WindowToolBar->toggleViewAction());

}
//-----------------------------------------------------------------------------
void VizTrader::createStatusBar()
{

	QHBoxLayout* layout = new QHBoxLayout;
	layout->setContentsMargins(0,0,0,0);
	ClientStatus = new QWidget;

	QMovie* movie = new QMovie(":/VizTrader/Resources/loading.gif");
	LoadingMovie = new QLabel;
	LoadingMovie->setMovie(movie);
	movie->start();

	UserName = new QLabel;
	UserName->setText("");

	ClientStatusButton = new QToolButton();
	ClientStatusButton->setIcon(QPixmap(":/VizTrader/Resources/certificateKey.png"));
	ClientStatusButton->setIconSize(QSize(10,10));
	ClientStatusButton->setAutoRaise(true);
	connect(ClientStatusButton, SIGNAL(clicked()), this, SLOT(certificateInfo()));


	LoadingMovie->setVisible(false);
	UserName->setVisible(false);
	ClientStatusButton->setDisabled(true);
	ClientStatusButton->setText(tr("Desconectado"));
	ClientStatusButton->setToolTip(tr("Desconectado"));
	ClientStatusButton->setToolButtonStyle(Qt::ToolButtonTextOnly);

	layout->addWidget(LoadingMovie);
	layout->addWidget(UserName);
	layout->addWidget(ClientStatusButton);

	ClientStatus->setLayout(layout);

	WorkAreaComboBox = new QComboBox;
	connect(WorkAreaComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(workAreaChange(int)));
	
	//statusBar()->addPermanentWidget(new QLabel(tr("Área de Trabalho:")));
	statusBar()->addPermanentWidget(WorkAreaComboBox);
	statusBar()->addPermanentWidget(ClientStatus);
	
}
//-----------------------------------------------------------------------------
void VizTrader::createDocks()
{
	PropertyEditorDock = new QDockWidget(tr("Propriedades"), this);
	PropertyEditorDock->setObjectName("PropertyEditorDock");
	PropertyEditorDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	addDockWidget(Qt::RightDockWidgetArea, PropertyEditorDock);

	SceneInspectorDock = new QDockWidget(tr("Inspetor de cena"), this);
	SceneInspectorDock->setObjectName("SceneInspectorDock");
	SceneInspectorDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	addDockWidget(Qt::RightDockWidgetArea, SceneInspectorDock);

	DataInspectorDock = new QDockWidget(tr("Inspetor de dados"), this);
	DataInspectorDock->setObjectName("DataInspectorDock");
	DataInspectorDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	addDockWidget(Qt::RightDockWidgetArea, DataInspectorDock);

	UndoViewDock = new QDockWidget(tr("Histórico"), this);
	UndoViewDock->setObjectName("UndoViewDock");
	UndoViewDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	addDockWidget(Qt::RightDockWidgetArea, UndoViewDock);

	ViewMenu->addAction(PropertyEditorDock->toggleViewAction());
	ViewMenu->addAction(SceneInspectorDock->toggleViewAction());
	ViewMenu->addAction(DataInspectorDock->toggleViewAction());
	ViewMenu->addAction(UndoViewDock->toggleViewAction());
	ViewMenu->addAction(ShowHideStatusBarAct);

}
//-----------------------------------------------------------------------------
void VizTrader::copy()
{
	VizIGraphicsItem* GraphicsItem = qobject_cast<VizIGraphicsItem*>(WorkArea->selectedVizOBject());
	VizCGraphicsPanel* GraphicsPanel = WorkArea->selectedGraphicsPanel();
	VizCMdiSubWindow* MdiSubWindow = activeMdiSubWindow();


	if(!GraphicsPanel && !GraphicsItem && !MdiSubWindow)
	{
		QApplication::beep();
		return;
	}

	if(GraphicsItem)
	{
		VizIGraphicsItemBasedOnSecurity* GraphicsItemBasedOnSecurity = qobject_cast<VizIGraphicsItemBasedOnSecurity*>(GraphicsItem);
		if(GraphicsItemBasedOnSecurity)
		{
			//copiar externa para txt
			Clipboard->setText(GraphicsItemBasedOnSecurity->toPlainText());
		}

		//copia interna
		VizIGraphicsItem* clonedItem = GraphicsPanel->cloneItem(GraphicsItem, false);
		Clipboard->setGraphicsItem(clonedItem);
	}
	else
	if(GraphicsPanel)
	{
		QObject* obj = WorkArea->selectedVizOBject();
		QPixmap pix;

		if(obj)
		{
			//aqui vai dar uma piscadeira, mas ta bom assim
			PropertyEditor->setObject(0);
			pix = QPixmap::grabWidget(MdiSubWindow);
			PropertyEditor->setObject(obj);
		}
		else
		{
			pix = QPixmap::grabWidget(MdiSubWindow);
		}
		Clipboard->setPixmap(pix);

		
		/*
		QObject* obj = WorkArea->selectedVizOBject();
		QPixmap pix;
		VizCSecurityBrowser* sb = activeMdiSubWindow();
		if(obj)
		{
			PropertyEditor->setObject(0);
			pix = QPixmap::grabWidget(sb);
			PropertyEditor->setObject(obj);
		}
		Clipboard->setPixmap(pix);
		*/
	}


}
//-----------------------------------------------------------------------------
void VizTrader::paste()
{

	VizIGraphicsItem* GraphicsItem = Clipboard->graphicsItem();
	VizCGraphicsPanel* GraphicsPanel = WorkArea->selectedGraphicsPanel();

	if(!GraphicsPanel && !GraphicsItem)
	{
		QApplication::beep();
		return;
	}

	if(GraphicsItem && GraphicsPanel->isPossibleAddItem(GraphicsItem))
	{
		GraphicsPanel->addItem(GraphicsItem);

		VizIGraphicsItem* clonedItem = GraphicsPanel->cloneItem(GraphicsItem, false);
		Clipboard->setGraphicsItem(clonedItem);

	}
	else
	{
		QApplication::beep();
	}

}
//-----------------------------------------------------------------------------
void VizTrader::cut()
{
	VizIGraphicsItem* GraphicsItem = qobject_cast<VizIGraphicsItem*>(WorkArea->selectedVizOBject());
	VizCGraphicsPanel* GraphicsPanel = WorkArea->selectedGraphicsPanel();

	if(!GraphicsPanel && !GraphicsItem)
	{
		QApplication::beep();
		return;
	}

	if(GraphicsItem && GraphicsPanel->isPossibleRemoveItem(GraphicsItem))
	{
		VizIGraphicsItem* ClonedItem = GraphicsPanel->cloneItem(GraphicsItem, false);
		Clipboard->setGraphicsItem(ClonedItem);
		GraphicsPanel->removeItem(GraphicsItem);
	}
	else
	{
		QApplication::beep();
	}


}
//-----------------------------------------------------------------------------
void VizTrader::remove()
{

	VizIGraphicsItem* graphicsItem = qobject_cast<VizIGraphicsItem*>(WorkArea->selectedVizOBject());
	VizCGraphicsPanel* GraphicsPanel = WorkArea->selectedGraphicsPanel();
	Q_ASSERT(graphicsItem && GraphicsPanel);

	if(GraphicsPanel->isPossibleRemoveItem(graphicsItem))
	{
		bool removeOk = true;
		if(Settings->value("General/Edit/ConfirmDeletionOfItens").toBool())
		{

			QMessageBox::StandardButton reply;
			reply = QMessageBox::question(this, ApplicationData->appicationName(), "Remover item selecionado?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
			if(reply != QMessageBox::Yes)
			{
				removeOk = false;
			}
		}
		if(removeOk)
		{
			GraphicsPanel->removeItem(graphicsItem);
			GraphicsPanel->repaint();
		}
	}
	else
	{
		QApplication::beep();
	}

}
//-----------------------------------------------------------------------------
void VizTrader::clipboardDataChanged()
{
	updateEditMenu();
}
//-----------------------------------------------------------------------------
void VizTrader::readSettings()
{
	//qDebug() << "loading settings...";

	bool RestoreWorkspaceOnStartup = Settings->value("General/Workspace/RestoreWorkspaceOnStartup", false).toBool();
	if(RestoreWorkspaceOnStartup)
	{
		Settings->setValue("General/Workspace/RestoreWorkspaceOnStartup", false);
	}

	bool autoAjustWindowFlag = Settings->value("General/MainWindow/AutoAjust", true).toBool();
	AutoAjustWindowAct->setChecked(autoAjustWindowFlag);
	if(autoAjustWindowFlag)
	{
		AutoAjustWindowAct->trigger();
	}

	if(!RestoreWorkspaceOnStartup)
	{
		bool statusBarVisible = Settings->value("General/MainWindow/StatusBar", true).toBool();
		ShowHideStatusBarAct->setChecked(statusBarVisible);
		statusBar()->setVisible(statusBarVisible);
		restoreGeometry(Settings->value("General/MainWindow/geometry").toByteArray());
		restoreState(Settings->value("General/MainWindow/windowState").toByteArray());
	}

/*
	QFont defaultFont;
	defaultFont.fromString(Settings->value("DefaultFont", QApplication::font()).toString());
	QApplication::setFont(defaultFont);

	QString styleName = Settings->value("DefaultStyle").toString();
	QApplication::setStyle(QStyleFactory::create(styleName));
*/

	//QFont sansFont("Helvetica [Cronyx]", 14);
	//QApplication::setFont(sansFont);

/*
	QString styleName = QStyleFactory::keys()[0];
	QStyle *style = QStyleFactory::create(styleName);
	Q_ASSERT(style);
	QApplication::setStyle(style);
*/	


}
//-----------------------------------------------------------------------------
void VizTrader::writeSettings()
{
	//qDebug() << "saving settings...";

	Settings->setValue("General/MainWindow/AutoAjust", ToolManager->isToolChecked(VETC_WINDOW_AUTO_TILE));
	Settings->setValue("General/MainWindow/StatusBar", statusBar()->isVisible());
	Settings->setValue("General/MainWindow/Geometry", saveGeometry());
	Settings->setValue("General/MainWindow/WindowState", saveState());

}
//-----------------------------------------------------------------------------
void VizTrader::aplicationSettingsChange()
{
	QFont defaultFont;
	defaultFont.fromString(Settings->value("Appearance/DefaultFontAndColor/Font", Settings->value("DefaultFont")).toString());
	QApplication::setFont(defaultFont);
	QString styleName = Settings->value("Appearance/System/Style", Settings->value("DefaultStyle")).toString();
	QApplication::setStyle(QStyleFactory::create(styleName));
	
	bool enable = Settings->value("Network/Proxy/Enabled", false).toBool();
	bool proxyChange = false;
	if(enable)
	{
		QString host = Settings->value("Network/Proxy/Host", "").toString();
		s32 port = Settings->value("Network/Proxy/Port", 1080).toInt();
		QString user = Settings->value("Network/Proxy/User", "").toString();
		QString pass = Settings->value("Network/Proxy/Password", "").toString();
		proxyChange = TP->setProxy(QNetworkProxy(QNetworkProxy::Socks5Proxy, host, port, user, pass));
	}
	else
	{
		proxyChange = TP->setProxy(QNetworkProxy(QNetworkProxy::NoProxy));
	}

	if(TP->onlineAndAuthenticate() && proxyChange)
	{
		logout();
		//login();
	}

	bool AutoConnect = Settings->value("Network/AutoConnect", false).toBool();
	QString name = Settings->value("Network/Account/User", "").toString();
	QString Md5Password = Settings->value("Network/Account/Password", "").toString();
	if(AutoConnect && !name.isEmpty() && !Md5Password.isEmpty())
	{
		AutoConnecting = true;
		login();
	}

}
//-----------------------------------------------------------------------------
void VizTrader::setActiveMdiSubWindow(VizCMdiSubWindow* w)
{
	MdiArea->setActiveSubWindow(w);
}
//-----------------------------------------------------------------------------
VizCMdiSubWindow* VizTrader::activeMdiSubWindow()
{
	QMdiSubWindow* activeSubWindow = MdiArea->activeSubWindow();
	if(activeSubWindow)
	{
		return qobject_cast<VizCMdiSubWindow*>(activeSubWindow);
	}
	return 0;
}
//-----------------------------------------------------------------------------
VizCMdiSubWindow* VizTrader::findMdiSubWindow(const QString& securitySymbol)
{
	foreach(QMdiSubWindow* window, MdiArea->subWindowList())
	{
		if(window->windowTitle() == securitySymbol)
		{
			return qobject_cast<VizCMdiSubWindow*>(window);
		}
	}
	return 0;
}
//-----------------------------------------------------------------------------
void VizTrader::updateToolBar()
{
	
	HorizontalLineAct->setChecked(ToolManager->isToolSelected(VETS_HORIZONTAL_LINE));
	VerticalLineAct->setChecked(ToolManager->isToolSelected(VETS_VERTICAL_LINE));
	TrendLineAct->setChecked(ToolManager->isToolSelected(VETS_TREND_LINE));
	SemiLogTrendLineAct->setChecked(ToolManager->isToolSelected(VETS_SEMI_LOG_TREND_LINE));
	SelectionToolAct->setChecked(ToolManager->isToolSelected(VETS_SELECTION));
	ZoomBoxAct->setChecked(ToolManager->isToolSelected(VETS_ZOOM_BOX));
	PanAct->setChecked(ToolManager->isToolSelected(VETS_PAN));

	AutoAjustWindowAct->setChecked(ToolManager->isToolChecked(VETC_WINDOW_AUTO_TILE));
	CrossHairsAct->setChecked(ToolManager->isToolChecked(VETC_CROSSHAIRS));

	QList<QMdiSubWindow *> windows = MdiArea->subWindowList();
	for(int i = 0; i < windows.size(); ++i)
	{
		windows.at(i)->repaint();
	}

}
//-----------------------------------------------------------------------------
void VizTrader::horizontalLine()
{
	ToolManager->selectTool(VETS_HORIZONTAL_LINE);
}
//-----------------------------------------------------------------------------
void VizTrader::verticalLine()
{
	ToolManager->selectTool(VETS_VERTICAL_LINE);
}
//-----------------------------------------------------------------------------
void VizTrader::trendLine()
{
	ToolManager->selectTool(VETS_TREND_LINE);
}
//-----------------------------------------------------------------------------
void VizTrader::semiLogTrendLine()
{
	ToolManager->selectTool(VETS_SEMI_LOG_TREND_LINE);
}
//-----------------------------------------------------------------------------
void VizTrader::selectionTool()
{
	ToolManager->selectTool(VETS_SELECTION);
	//PropertyEditor->setObject(0);
	//SceneInspector->setGraphicsPanel(0);
}
//-----------------------------------------------------------------------------
void VizTrader::crossHairsTool()
{
	ToolManager->checkTool(VETC_CROSSHAIRS);
}
//-----------------------------------------------------------------------------
void VizTrader::zoomBoxTool()
{
	ToolManager->selectTool(VETS_ZOOM_BOX);
}
//-----------------------------------------------------------------------------
void VizTrader::panTool()
{
	ToolManager->selectTool(VETS_PAN);
}
//-----------------------------------------------------------------------------
void VizTrader::autoAjustWindow()
{
	ToolManager->checkTool(VETC_WINDOW_AUTO_TILE);

	if(ToolManager->isToolChecked(VETC_WINDOW_AUTO_TILE))
	{
		MdiArea->tileSubWindows();
	}
}
//-----------------------------------------------------------------------------
void VizTrader::showHideStatusBar()
{
	if (statusBar()->isVisible())
	{
		statusBar()->show();
	}
	else
	{
		statusBar()->hide();
	}

}
//-----------------------------------------------------------------------------
void VizTrader::fullScreen()
{
	if(isFullScreen())
	{
		showNormal();
	}
	else
	{
		showFullScreen();
	}
}
//-----------------------------------------------------------------------------
void VizTrader::fullDesktop()
{
}
//-----------------------------------------------------------------------------
void VizTrader::showHideDataWindow()
{

}
//-----------------------------------------------------------------------------
void VizTrader::refresh()
{
	VizCGraphicsPanel* gp = WorkArea->selectedGraphicsPanel();
	if(gp)
	{
		const QList<VizIGraphicsItem*>& items = gp->items();
		
		//if(!gp->isLoadingState())
		foreach(VizIGraphicsItem* item, items)
		{
			VizIGraphicsItemBasedOnSecurity* gibos = qobject_cast<VizIGraphicsItemBasedOnSecurity*>(item);
			if(gibos)
			{
				DataFacade->reloadSecurity(gibos->securitySymbol());
			}
		}
	}
	else
	{
		QApplication::beep();
	}
	
}
//-----------------------------------------------------------------------------
void VizTrader::keyPressEvent(QKeyEvent* event)
{
	switch (event->key())
	{
		case Qt::Key_Escape:
		{
			selectionTool();
			if(ToolManager->isToolChecked(VETC_CROSSHAIRS))
			{
				crossHairsTool();
			}
			
			VizCGraphicsPanel* gp = WorkArea->selectedGraphicsPanel();
			if(gp)
			{
				gp->cancelAddItem();
			}

			WorkArea->clearSelection();

			/*
			PropertyEditor->setObject(0);
			SceneInspector->setGraphicsPanel(0);
			*/
		}
		
	};

	QMainWindow::keyPressEvent(event);
}
//-----------------------------------------------------------------------------
