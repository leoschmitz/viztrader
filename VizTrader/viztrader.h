//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZTRADER_H__
#define __VIZTRADER_H__
//-----------------------------------------------------------------------------
#include "VizLib.h"
#include <QMainWindow>
#include <QMetaType>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QSignalMapper>
#include <QAction>
#include <QMenu>
#include <QDockWidget>
#include <QSystemTrayIcon>
#include <QComboBox>
//-----------------------------------------------------------------------------
using namespace viz;
using namespace core;
using namespace gui;
using namespace net;
using namespace io;
using namespace system;
//-----------------------------------------------------------------------------
class VizTrader : public QMainWindow
{
	Q_OBJECT

public:
	VizTrader();
	~VizTrader();

protected:
	void closeEvent(QCloseEvent *event);

public slots:
	void commitDataRequest(QSessionManager& sm);

private slots:
	void vizServerConnected();
	void vizServerDisconnected();
	void vizServerCertificateReady(bool isValid);
	void vizServerError(VizCTP::VizTPError vetperror);
	void login();
	void logout();
	void certificateInfo();

	void open();
	void save();
	void saveAll();
	void about();
	void updateMenus();
	void updateWindowMenu();
	void setActiveSubWindowByWindowMenu(QWidget* subWindow);

	void copy();
	void paste();
	void cut();
	void remove();
	void clipboardDataChanged();
	void updateEditMenu();

	void selectionTool();
	void crossHairsTool();
	void zoomBoxTool();
	void panTool();
	void horizontalLine();
	void verticalLine();
	void trendLine();
	void semiLogTrendLine();

	void showHideStatusBar();
	void showHideDataWindow();
	void fullScreen();
	void fullDesktop();
	void autoAjustWindow();
	
	void updateToolBar();
	void aplicationSettingsChange();

	void closeActiveSubWindow();
	void closeAllSubWindows();
	void tileSubWindows();
	void cascadeSubWindows();
	void activateNextSubWindow();
	void activatePreviousSubWindow();

	void workAreaManagement();
	void workAreaNext();
	void workAreaPrev();
	void workAreaChange(int index);
	void workAreaChangeUndoAction();
	void workAreaChangeRedoAction();
	void workAreaChangeApplicationTitle();
	void workAreaRemoved(int index);
	void workAreaAdded(int index);
	void workAreaRename(int index);
	void workAreaMove(int from, int to);
	
	void undo();
	void redo();

	void refresh();

protected:
	virtual void keyPressEvent(QKeyEvent* event);

private:
	
	void workAreaSave();
	void workAreaLoad();
	void setActiveMdiSubWindow(VizCMdiSubWindow* w);
	VizCMdiSubWindow* activeMdiSubWindow();
	VizCMdiSubWindow* findMdiSubWindow(const QString& securitySymbol);

	void applicationClose();
	bool checkUserAndPassword(const QString& user, const QString& pass) const;
	void changeClientStatus();
	void applicationSettings();
	void registerMetaTypes();
	void createActions();
	void createMenus();
	void createToolBars();
	void createStatusBar();
	void createDocks();
	void readSettings();
	void writeSettings();

	QMenu* WorkAreaMenu;
	QMenu* FileMenu;
	QMenu* EditMenu;
	QMenu* ViewMenu;
	QMenu* WindowMenu;
	QMenu* HelpMenu;

	//view
	QAction* ShowHideStatusBarAct;
	QAction* FullScreenAct;
	QAction* FullDesktopAct;
	QAction* ShowHideDataWindowAct;

	QToolBar* LoginToolBar;
	QAction* LoginAct;
	QAction* LogoutAct;

	QToolBar* StandardToolBar;
	QAction* OpenAct;
	QAction* CloseAct;
	QAction* CloseAllAct;
	QAction* SaveAct;
	QAction* SaveAllAct;
	QAction* ExitAct;
	QAction* SeparatorAct;
	QAction* AboutAct;
	QAction* RefreshAct;

	VizCEditActions* EditActions;
	QAction* ShowSettingsDialogAct;

	QToolBar* TreendLinesToolBar;
	QAction* HorizontalLineAct;
	QAction* VerticalLineAct;
	QAction* TrendLineAct;
	QAction* SemiLogTrendLineAct;
	
	QToolBar* NavigationToolBar;
	QAction* SelectionToolAct;
	QAction* CrossHairsAct;
	QAction* ZoomBoxAct;
	QAction* PanAct;

	QToolBar* WorkAreaToolBar;
	QAction* WorkAreaManageAct;
	QAction* WorkAreaNextAct;
	QAction* WorkAreaPreviousAct;

	QToolBar* WindowToolBar;
	QAction* AutoAjustWindowAct;
	QAction* TileAct;
	QAction* CascadeAct;
	QAction* NextAct;
	QAction* PreviousAct;

	QDockWidget* PropertyEditorDock;
	QDockWidget* SceneInspectorDock;
	QDockWidget* UndoViewDock;
	QDockWidget* DataInspectorDock;
	
	QSystemTrayIcon* SystemTrayIcon;

	io::VizCDataFacade* DataFacade;
	VizCOpenSecurityWidget* OpenSecurityWidget;
	VizCToolManager* ToolManager;
	VizCApplicationSettings* ApplicationSettings;
	io::VizCSettings* Settings;
	VizCClipboard* Clipboard;
	VizCApplicationData* ApplicationData;
	
	net::VizCTP* TP;
	VizCCertificateInformation* CertificateInformation;

	QWidget* ClientStatus;
	QToolButton* ClientStatusButton;
	QLabel* LoadingMovie;
	QLabel* UserName;
	s32 ApplicationNetworkStatus;
	bool AutoConnecting;

	VizCDataInspector* DataInspector;
	VizCPropertyEditor* PropertyEditor;
	VizCSceneInspector* SceneInspector;
	VizCUndoView* UndoView;
	VizCUndoManager* UndoManager;
	QMdiArea* MdiArea;
	VizCWorkArea* WorkArea;
	VizCWorkAreaManager* WorkAreaManager;
	VizCWorkAreaManagerWidget* WorkAreaManagerWidget;
	QStackedWidget* StackedWidget;

	QSignalMapper* WindowMapper;
	QString WokAreaNamePrev;
	QString WokAreaNameNext;
	QComboBox* WorkAreaComboBox;
	QLabel* WokAreaLabel;
	s32 CurrentWorkAreaIndex;
	QString CurrentWorkAreaName;

	QDialog* LoginDialog;
	QLabel* LoginDialogImageLabel;
	QLineEdit* LoginDialogNameLineEdit;
	QLineEdit* LoginDialogPasswordLineEdit;
	QCheckBox* LoginDialogSaveAccountInThisComputerCheckBox;
	QCheckBox* LoginDialogAutoConnectCheckBox;
	QDialogButtonBox* LoginDialogButtonBox;

	void createLoginDialog();

private slots:
	void loginDialogNameLineEditTextEdited(const QString& text);
	void loginDialogPasswordLineEditTextEdited(const QString& text);
	void loginDialogSaveAccountInThisComputerCheckBoxStateChanged(int state);

};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
