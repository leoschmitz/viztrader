//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include <QApplication>
#include <QMessageBox>
#include <QtGlobal>
#include "viztrader.h"
//-----------------------------------------------------------------------------
//#include "VizCMemoryLeakReport.h"
//viz::system::VizCMemoryLeakReport MemoryLeakReport;
//-----------------------------------------------------------------------------
void VizMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& message)
{
	QByteArray bytes = message.toLocal8Bit();
	const char* msg = bytes.data();
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
	QT_REQUIRE_VERSION(argc, argv, "5.1.15");
	Q_ASSERT(QSystemTrayIcon::isSystemTrayAvailable());

	qInstallMessageHandler(VizMessageOutput);

	QApplication app(argc, argv);

	VizTrader vizTrader;
	//app.setActivationWindow(&vizTrader);

	app.connect(&app, SIGNAL(commitDataRequest(QSessionManager&)), &vizTrader, SLOT(commitDataRequest(QSessionManager&)));

	vizTrader.show();
	return app.exec();
}
//-----------------------------------------------------------------------------
