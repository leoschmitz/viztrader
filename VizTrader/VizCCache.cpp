//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCCache.h"
#include "VizCApplicationSettings.h"
#include "VizCMemoryUtils.h"

#include <QDebug>
//-----------------------------------------------------------------------------
namespace viz
{
namespace io
{
static core::VizCSecurity InvalidReturnSecurity = core::VizCSecurity();
//-----------------------------------------------------------------------------
VizCCache::VizCCache(QObject* parent) : QObject(parent), 
	MaximumSizeInKB(128 * 1024), MemoryUsedInKB(0)		
{
	updateCacheSize();

	connect(gui::VizCApplicationSettings::instance(), SIGNAL(settingsChange()), this, SLOT(updateCacheSize()));	
}
//-----------------------------------------------------------------------------
VizCCache::~VizCCache()
{
}
//-----------------------------------------------------------------------------
core::VizCSecurity& VizCCache::security(const QString& symbol)
{
	if (symbol.size() < 5)
		return InvalidReturnSecurity;

	return CompanyToCache[symbol.left(4)].Company.Securities[SearchThroughSymbol.value(symbol)];
}
//-----------------------------------------------------------------------------
void VizCCache::updateCacheSize()
{
	u32 totalSysMemInKb = static_cast<u32>(system::VizCMemoryUtils::totalSystemMemory() / 1024);
	u32 percent = static_cast<u32>(io::VizCSettings::instance()->value("Performance/Memory/MaxPercentOfMemoryToCache", 25).toInt());

	setMaximumSizeInKB(percent * totalSysMemInKb);	
}
//-----------------------------------------------------------------------------
bool VizCCache::appendSecurityPeriods(const QString& symbol, const QVector<core::VizCSecurityPeriod>& periods)
{
	return false;
}
//-----------------------------------------------------------------------------
const QList<VizSCacheElement>& VizCCache::elements()
{
	Q_ASSERT(false);
	return CompanyToCache.values();
}
//-----------------------------------------------------------------------------
bool VizCCache::contains(const QString& symbol)
{
	if (symbol.size() < 5)
		return false;

	if (CompanyToCache.contains(symbol.left(4)))
		return SearchThroughSymbol.contains(symbol);

	return false;
}
//-----------------------------------------------------------------------------
u32 VizCCache::evaluateApproximateSizeInKB(const core::VizCSecurity& security)
{
	u32 approxSize(0);
	for (int i = 0; i < security.SecurityPeriods.size(); i++)
		approxSize += core::approximateSizeOfASinglePeriod();

	return approxSize;
}
//-----------------------------------------------------------------------------
bool VizCCache::hasDummy(const QString& symbol)
{
	return DummySecurity.contains(symbol);
}
//-----------------------------------------------------------------------------
void VizCCache::removeDummySecurity(const QString& symbol)
{
	if (hasDummy(symbol))
		DummySecurity.remove(symbol);
}
//-----------------------------------------------------------------------------
void VizCCache::insertDummySecurity(const QString& symbol)
{
	QWriteLocker lock(&Lock);

	qDebug() << "Insert Dummy " << symbol;
	DummySecurity.insert(symbol);

	if (CompanyToCache.contains(symbol.left(4)))
	{
		SearchThroughSymbol.insert(symbol, 
			CompanyToCache.value(symbol.left(4)).Company.Securities.size());		
	}
	else
	{
		SearchThroughSymbol.insert(symbol, 0);
		CompanyToCache.insert(symbol.left(4), VizSCacheElement());			
		CompanyToCache[symbol.left(4)].Company.PartialSecuritySymbol = symbol.left(4);		
	}

	CompanyToCache[symbol.left(4)].Company.Securities.append(core::VizCSecurity());
	CompanyToCache[symbol.left(4)].Company.Securities[SearchThroughSymbol.value(symbol)].Symbol = symbol;	
}
//-----------------------------------------------------------------------------
void VizCCache::removeSecurity(const QString symbol)
{
	// Testes de sanidade
	if (symbol.size() < 5)
		return;
	if (!contains(symbol))
		return;	

	//QWriteLocker lock(&Lock);
	Lock.lockForWrite();

	qDebug() << "Remove " << symbol;

	if (CompanyToCache[symbol.left(4)].Company.Securities.size() < 2)
	{
		VizSCacheElement& element = CompanyToCache.take(symbol.left(4));
		subElementSize(element.BytesCost);
	}
	else 
	{
		subElementSize(evaluateApproximateSizeInKB(
			CompanyToCache[symbol.left(4)].Company.Securities.at(SearchThroughSymbol.value(symbol))));		
	
		CompanyToCache[symbol.left(4)].Company.Securities.remove(SearchThroughSymbol.value(symbol));
	}
	
	SearchThroughSymbol.remove(symbol);	
	
	RemoveList.removeOne(symbol);	

	Lock.unlock();

	emit cacheChanged(symbol, false);	
}
//-----------------------------------------------------------------------------
void VizCCache::removeSecurityByFIFO()
{
	while ((MemoryUsedInKB > MaximumSizeInKB) && !RemoveList.isEmpty())
	{		
		removeSecurity(RemoveList.first());
	}
}
//-----------------------------------------------------------------------------
void VizCCache::checkBufferSize()
{
	if (MemoryUsedInKB < MaximumSizeInKB)
		return;

	removeSecurityByFIFO();
}
//-----------------------------------------------------------------------------
bool VizCCache::insertSecurity(const core::VizCCompany& company, const s32& elementSize)
{
	//QWriteLocker lock(&Lock);

	

	const core::VizCSecurity& security = company.Securities.at(0);
	const QString& symbol = company.Securities.at(0).Symbol;

	if (hasDummy(symbol))
	{	
		Lock.lockForWrite();

		if (CompanyToCache.contains(company.PartialSecuritySymbol))
		{
 			if (SearchThroughSymbol.contains(symbol))
			{
				CompanyToCache[company.PartialSecuritySymbol].Company.mergeAttributes(company);
				CompanyToCache[company.PartialSecuritySymbol].Company.Securities.remove(SearchThroughSymbol.value(symbol));
			}
			
			CompanyToCache[company.PartialSecuritySymbol].Company.Securities.append(security);
			SearchThroughSymbol.insert(symbol, 
				CompanyToCache.value(company.PartialSecuritySymbol).Company.Securities.size() - 1);

			CompanyToCache[company.PartialSecuritySymbol].BytesCost += elementSize;		
		}
		else
		{
			SearchThroughSymbol.insert(symbol, 0);
			CompanyToCache.insert(company.PartialSecuritySymbol, VizSCacheElement(company, elementSize));
		}

		removeDummySecurity(symbol);

		RemoveList.push_back(symbol);

		addElementSize(elementSize);

		checkBufferSize();

		Lock.unlock();

		emit cacheChanged(symbol, true);	
	}

	return true;
}
//-----------------------------------------------------------------------------
void VizCCache::addElementSize(u32 byteSize)
{
	MemoryUsedInKB += (byteSize / 1024);

	qDebug() << MemoryUsedInKB;
}
//-----------------------------------------------------------------------------
void VizCCache::subElementSize(u32 byteSize)
{
	MemoryUsedInKB = ((byteSize / 1024) > MemoryUsedInKB) ? 0 : MemoryUsedInKB - (byteSize / 1024);

	qDebug() << MemoryUsedInKB;
}
//-----------------------------------------------------------------------------
const QVector<core::VizCSecurityPeriod>& VizCCache::securityPeriodsDaily(const QString& securitySymbol)
{
	s32 securityIndex(-1);

	if (!SearchThroughSymbol.contains(securitySymbol))
	{
		insertDummySecurity(securitySymbol);		
	}	

	securityIndex = SearchThroughSymbol.value(securitySymbol);

	return CompanyToCache.value(securitySymbol.left(4)).Company.Securities.at(securityIndex).SecurityPeriods;
}
//-----------------------------------------------------------------------------
void VizCCache::setMaximumSizeInKB(const u32& maxSizeKB)
{
	MaximumSizeInKB = maxSizeKB;	
}
//-----------------------------------------------------------------------------
u32 VizCCache::maximumSizeInKB()
{
	return MaximumSizeInKB;
}
//-----------------------------------------------------------------------------
u32 VizCCache::sizeInKB()
{
	return MemoryUsedInKB;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
