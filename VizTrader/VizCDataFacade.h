//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCDATAFACADE_H__
#define __VIZCDATAFACADE_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include "VizCCompany.h"
#include "VizCCache.h"
#include "VizCFile.h"
#include "VizCTP.h"

#include <QString>
#include <QByteArray>
#include <QMap>
#include <QVector>
#include <QThreadPool>
//-----------------------------------------------------------------------------
namespace viz
{
namespace io
{
//-----------------------------------------------------------------------------
class VizCDataFacade : public QObject
{

	Q_OBJECT
signals:
	void securityPeriodChanged(const QString& securitySymbol);
	void securityPercentageLoaded(const QString& securitySymbol, int percent);
	void securityChanged(const QString& securitySymbol, bool input);

public:
	static VizCDataFacade* instance();

	const QVector<core::VizCSecurityPeriod>& securityPeriodsDaily(const QString& securitySymbol);

	bool loadSecurity(const QString& symbol);
	bool isCached(const QString& symbol);
	bool isLocal(const QString& symbol);

	void loadDiskNegotiableList(QVector<QStringList>& list);

public slots:
	void reloadSecurity(const QString& symbol);

private:
	VizCDataFacade();
	~VizCDataFacade();
	static VizCDataFacade* Instance;

	QList<QString> SecuritiesFromServer;		

	bool loadFromServer(const QString& symbol);
	void loadFromDisk(const QString& symbol);		
	bool updateDailyAvailable(const QDate& lastDate);
	void updateSecurity(core::VizCSecurity& sec);

	VizCFile* InterfaceToDisk;
	VizCCache* InterfaceToCache;	
	net::VizCTP* InterfaceToServer;	

private slots:
	void serverPercentage(VizETPClientPacket packetType, f32 perUnit);
	
	void syncCachedData();
	void securityReady();	
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
