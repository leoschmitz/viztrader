//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCDataFacade.h"
#include "VizCFileThread.h"
#include <QDebug>
#include <QThread>
//-----------------------------------------------------------------------------
namespace viz
{
namespace io
{
//-----------------------------------------------------------------------------
VizCDataFacade* VizCDataFacade::Instance = 0;
//-----------------------------------------------------------------------------
VizCDataFacade::VizCDataFacade()  
{
	InterfaceToCache = new VizCCache;
	InterfaceToDisk = new VizCFile;
	InterfaceToServer = net::VizCTP::instance();

	connect(InterfaceToServer, SIGNAL(tcpFullSecurityReady()), this, SLOT(securityReady()));
	connect(InterfaceToServer, SIGNAL(tcpPartialSecurityReady()), this, SLOT(securityReady()));
	connect(InterfaceToServer, SIGNAL(connected()), this, SLOT(syncCachedData()));
	connect(InterfaceToServer, SIGNAL(percentageReady(VizETPClientPacket,f32)), this, SLOT(serverPercentage(VizETPClientPacket, f32)));
	connect(InterfaceToDisk, SIGNAL(percentageReady(QString, int)), this, SIGNAL(securityPercentageLoaded(QString, int)));
	connect(InterfaceToCache, SIGNAL(cacheChanged(QString, bool)), this, SIGNAL(securityChanged(QString, bool)));

}
//-----------------------------------------------------------------------------
VizCDataFacade::~VizCDataFacade()
{
}
//-----------------------------------------------------------------------------
VizCDataFacade* VizCDataFacade::instance()
{
	if(!Instance)
	{
		Instance = new VizCDataFacade;
	}
	return Instance;
}
//-----------------------------------------------------------------------------
bool VizCDataFacade::loadFromServer(const QString& symbol)
{

	if (InterfaceToServer->onlineAndAuthenticate())
	{
		InterfaceToServer->requestSecurity(symbol);

		SecuritiesFromServer.append(symbol);
	}
	else
		return false;

	return true;
}
//-----------------------------------------------------------------------------
void VizCDataFacade::loadFromDisk(const QString& symbol)
{	
//	VizCFileThread semthread(symbol, InterfaceToCache, InterfaceToDisk);
//	semthread.run();

	VizCFileThread* thread = new VizCFileThread(symbol, InterfaceToCache, InterfaceToDisk);

	QThreadPool::globalInstance()->start(thread);
}
//-----------------------------------------------------------------------------
void VizCDataFacade::loadDiskNegotiableList(QVector<QStringList>& list)
{
	InterfaceToDisk->loadTradeList(list);
}
//-----------------------------------------------------------------------------
bool VizCDataFacade::updateDailyAvailable(const QDate& lastDate)
{
	return (lastDate < QDate::currentDate());
}
//-----------------------------------------------------------------------------
void VizCDataFacade::updateSecurity(core::VizCSecurity& sec)
{
	// Aqui entra a sincronização
	if (InterfaceToServer->onlineAndAuthenticate())
	{
		const QDate& lastDate = sec.SecurityPeriods.at(sec.SecurityPeriods.size() - 1).ExchangeDate;

		if (updateDailyAvailable(lastDate))
		{ 
			InterfaceToServer->requestSecurity(sec.Symbol, lastDate);
		}		
	}
}
//-----------------------------------------------------------------------------
// void VizCDataFacade::removeLoadingSecurityFromDisk(const QString& symbol)
// {
// 	//SecuritiesFromDisk.remove(symbol);
// }
//-----------------------------------------------------------------------------
void VizCDataFacade::syncCachedData()
{
// 	foreach (const VizSCacheElement& cacheElement, InterfaceToCache->elements())
// 	{
//  		foreach(core::VizCSecurity security, cacheElement.Company.Securities)
//  		{
//  			updateSecurity(security);		
//  		}
// 	}
}
//-----------------------------------------------------------------------------
bool VizCDataFacade::loadSecurity(const QString& symbol)
{
	// Testa sanidade 
	if (symbol.size() < 5)
		return false;

	if (!isCached(symbol))
	{
		InterfaceToCache->insertDummySecurity(symbol);
	}
	
	
	if (InterfaceToCache->hasDummy(symbol))
	//if ((!isCached(symbol) || InterfaceToCache->hasDummy(symbol)))
	{	
		if (InterfaceToDisk->containsSymbol(symbol))
		{
			loadFromDisk(symbol);
			
			updateSecurity(InterfaceToCache->security(symbol));
		} 
		else
		{
			if (!loadFromServer(symbol))
				return false;
		}
	} 
// 	else
// 	{
// 		emit securityChanged(symbol, true);
// 	}

	if (isCached(symbol) && !InterfaceToCache->hasDummy(symbol))
	{
		emit securityChanged(symbol, true);
		emit securityPercentageLoaded(symbol, 0);
	}

	return true;
}
//-----------------------------------------------------------------------------
void VizCDataFacade::reloadSecurity(const QString& symbol)
{
	if (isCached(symbol))
	{
		InterfaceToCache->removeSecurity(symbol);

		loadSecurity(symbol);
	}	
}
//-----------------------------------------------------------------------------
bool VizCDataFacade::isLocal(const QString& securityId)
{
	return (InterfaceToDisk->containsSymbol(securityId) || (isCached(securityId)));
}
//-----------------------------------------------------------------------------
bool VizCDataFacade::isCached(const QString& symbol)
{
	return InterfaceToCache->contains(symbol);
}
//-----------------------------------------------------------------------------
const QVector<core::VizCSecurityPeriod>& VizCDataFacade::securityPeriodsDaily(const QString& securitySymbol)
{
	return InterfaceToCache->securityPeriodsDaily(securitySymbol);
}
//-----------------------------------------------------------------------------
void VizCDataFacade::serverPercentage(VizETPClientPacket packetType, f32 perUnit)
{
	if (packetType == VETPSP_REQUEST_FULL_HISTO)
	{
		emit securityPercentageLoaded(SecuritiesFromServer.at(0), static_cast<int>(perUnit * 100.0f));
	}
}
//-----------------------------------------------------------------------------
void VizCDataFacade::securityReady()
{
	SecuritiesFromServer.pop_front();

	QPair<core::VizCCompany, s32>& companyAndSize = InterfaceToServer->RequestedCompanies.takeFirst();

	const QString& symbol = companyAndSize.first.Securities.at(0).Symbol;

	// Update do security
// 	if (isLocal(symbol))
// 	{
// 		InterfaceToDisk->updateSecurityFile(symbol, companyAndSize.first.Securities.at(0).SecurityPeriods);
// 
// 		if (isCached(symbol))
// 			InterfaceToCache->appendSecurityPeriods(symbol, companyAndSize.first.Securities.at(0).SecurityPeriods);
// 
// 		return;
// 	}	

	InterfaceToDisk->appendDaily(companyAndSize.first.Securities.at(0).Symbol, companyAndSize.first);		

	 companyAndSize.first.Securities[0].SecurityPeriods
		= core::VizCSecurityPeriodsManipulator::instance()->adjustSecurityPeriodsSplits(companyAndSize.first.Splits, 
			companyAndSize.first.Securities[0].SecurityPeriods);

	 InterfaceToCache->insertSecurity(companyAndSize.first, companyAndSize.second);	
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
