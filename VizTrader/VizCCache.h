//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCCACHE_H__
#define __VIZCCACHE_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include "VizCCompany.h"
#include "VizCFile.h"
#include "VizCTP.h"

#include <QString>
#include <QByteArray>
#include <QMap>
#include <QVector>

#include <QReadWriteLock>
#include <QWriteLocker>
//-----------------------------------------------------------------------------
namespace viz
{
namespace io
{
//-----------------------------------------------------------------------------
typedef struct CacheElement 
{
	u32 BytesCost;
	core::VizCCompany Company;

	CacheElement(const core::VizCCompany& company, const u32& bytesCost) 
		: BytesCost(bytesCost) 
	{
 		Company = company;
	};
	CacheElement() : BytesCost(0) {};

	CacheElement& operator=(const CacheElement& other)
	{
		BytesCost = other.BytesCost;

		Company = other.Company;

		return *this;

	};

} VizSCacheElement;
//-----------------------------------------------------------------------------
class VizCCache : public QObject 
{
	Q_OBJECT
private:
	QReadWriteLock Lock;

	u32 MaximumSizeInKB;
	u32 MemoryUsedInKB;

	QList<QString> RemoveList;	

	QMap<QString, s32> SearchThroughSymbol;	
	QMap<QString, VizSCacheElement> CompanyToCache;
	QSet<QString> DummySecurity;

	void addElementSize(u32 byteSize);
	void subElementSize(u32 byteSize);

	void checkBufferSize();
	void removeSecurityByFIFO();	

	void removeDummySecurity(const QString& symbol);
public:	
	const QVector<core::VizCSecurityPeriod>& securityPeriodsDaily(const QString& securitySymbol);
	
	core::VizCSecurity& security(const QString& symbol);
	const QList<VizSCacheElement>& elements();

	bool hasDummy(const QString& symbol);	
	void insertDummySecurity(const QString& symbol);
	
	bool contains(const QString& symbol);
	bool insertSecurity(const core::VizCCompany& company, const s32& elementSize);
	void removeSecurity(const QString symbol);
	bool appendSecurityPeriods(const QString& symbol, const QVector<core::VizCSecurityPeriod>& periods);	
	
	u32 evaluateApproximateSizeInKB(const core::VizCSecurity& security);
	void setMaximumSizeInKB(const u32& maxSizeKB);		
	u32 sizeInKB();
	u32 maximumSizeInKB();

	VizCCache(QObject* parent = 0);
	~VizCCache();

private slots:
	void updateCacheSize();

signals:
	void cacheChanged(const QString& symbol, const bool& input);
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
