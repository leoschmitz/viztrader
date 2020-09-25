//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include <QtGui/QApplication>
#include <QtSingleApplication>
#include <QMessageBox>
#include <QtGlobal>
#include "viztrader.h"
//-----------------------------------------------------------------------------
//#include "VizCMemoryLeakReport.h"
//viz::system::VizCMemoryLeakReport MemoryLeakReport;
//-----------------------------------------------------------------------------
void VizMessageOutput(QtMsgType type, const char* msg)
{
	switch (type)
	{
		case QtDebugMsg:
			fprintf(stderr, "Debug: %s\n", msg);
			break;
		case QtWarningMsg:
			fprintf(stderr, "Warning: %s\n", msg);
			break;
		case QtCriticalMsg:
			fprintf(stderr, "Critical: %s\n", msg);
			break;
		case QtFatalMsg:
			fprintf(stderr, "Fatal: %s\n", msg);
			abort();
	}
	/*if (logEnabled())
		log(L_DEBUG, "QT: %s", msg);
		*/
}
//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	QT_REQUIRE_VERSION(argc, argv, "4.6.0");
	Q_ASSERT(QSystemTrayIcon::isSystemTrayAvailable());

	qInstallMsgHandler(VizMessageOutput);

	//QApplication app(argc, argv);
	QtSingleApplication app(argc, argv);
	if(app.isRunning())
	{
		return 0;
	}

	VizTrader vizTrader;
	app.setActivationWindow(&vizTrader);

	app.connect(&app, SIGNAL(commitDataRequest(QSessionManager&)), &vizTrader, SLOT(commitDataRequest(QSessionManager&)));

	vizTrader.show();
	return app.exec();
}
//-----------------------------------------------------------------------------
