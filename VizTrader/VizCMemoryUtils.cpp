//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCMemoryUtils.h"
#include <QFile>
#include <QTextStream>
#include <QTime>
#include <QDebug>
#ifdef Q_OS_WIN
#include <windows.h>
#endif
//-----------------------------------------------------------------------------
namespace viz
{
namespace system
{
//-----------------------------------------------------------------------------
void VizCMemoryUtils::formatByteSize(u64 bytes, f64& value, QString& suffix)
{

/*
	const int MB = 1048576; // A megabyte is 1,048,576 bytes
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof (statex);
	GlobalMemoryStatusEx (&statex);

	qDebug() << statex.dwMemoryLoad << " percent of memory is in use." << endl;
	qDebug() << endl;
	qDebug() << "There are " << statex.ullTotalPhys/MB << " total megabytes of physical memory." << endl;
	qDebug() << "There are " << statex.ullAvailPhys/MB << " free megabytes of physical memory." << endl;
	qDebug() << endl;
	qDebug() << "There are " << statex.ullTotalPageFile/MB << " total megabytes of paging file." << endl;
	qDebug() << "There are " << statex.ullAvailPageFile/MB << " free megabytes of paging file." << endl;
	qDebug() << endl;
	qDebug() << "There are " << statex.ullTotalVirtual/MB << " total megabytes of virtual memory." << endl;
	qDebug() << "There are " << statex.ullAvailVirtual/MB << " free megabytes of virtual memory." << endl;
	qDebug() << endl;
	qDebug() << "There are " << statex.ullAvailExtendedVirtual/MB << " free megabytes of virtual memory." << endl;
*/

	const f64 B = 1.0; //byte
	const f64 KB = 1024 * B; //kilobyte
	const f64 MB = 1024 * KB; //megabyte
	const f64 GB = 1024 * MB; //gigabyte

	if(bytes >= GB)
	{
		value = bytes / GB;
		suffix = "GB";
	}
	else
	if(bytes >= MB)
	{
		value = bytes / MB;
		suffix = "MB";
	}
	else
	if(bytes >= KB)
	{
		value = bytes / KB;
		suffix = "KB";
	}
	else
	{
		value = bytes;
		suffix = "bytes";
	}

}
//-----------------------------------------------------------------------------
u64 VizCMemoryUtils::totalSystemMemory()
{

#if defined(Q_OS_WIN)

	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof (statex);
	GlobalMemoryStatusEx (&statex);
	return statex.ullTotalPhys;

#elif defined(Q_OS_LINUX)
	// if /proc/meminfo doesn't exist, return 128MB
	QFile memFile( "/proc/meminfo" );
	if(!memFile.open(QIODevice::ReadOnly))
	{
		return 134217728;
	}

	// read /proc/meminfo and sum up the contents of 'MemFree', 'Buffers'
	// and 'Cached' fields. consider swapped memory as used memory.
	QTextStream readStream( &memFile );
	while(true)
	{
		QString entry = readStream.readLine();
		if(entry.isNull()) break;
		if(entry.startsWith("MemTotal:"))
		{
			return (1024 * entry.section( ' ', -2, -2 ).toInt());
		}
	}
#else
	// outro SO nao implementado ainda
	Q_ASSERT(false);
	return 0;
#endif
}
//-----------------------------------------------------------------------------
u64 VizCMemoryUtils::availableSystemMemory()
{
#if defined(Q_OS_WIN)

	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof (statex);
	GlobalMemoryStatusEx (&statex);
	return statex.ullAvailPhys;

#elif defined(Q_OS_LINUX)
	// if /proc/meminfo doesn't exist, return MEMORY FULL
	QFile memFile("/proc/meminfo");
	if(!memFile.open( QIODevice::ReadOnly))
	{
		return 0;
	}

	// read /proc/meminfo and sum up the contents of 'MemFree', 'Buffers'
	// and 'Cached' fields. consider swapped memory as used memory.
	s64 memoryFree = 0;
	QString entry;
	QTextStream readStream(&memFile);
	while(true)
	{
		entry = readStream.readLine();
		if(entry.isNull() ) break;
		if(entry.startsWith( "MemFree:") ||
			entry.startsWith( "Buffers:") ||
			entry.startsWith( "Cached:") ||
			entry.startsWith( "SwapFree:") )
			memoryFree += entry.section(' ', -2, -2 ).toInt();
		if(entry.startsWith( "SwapTotal:"))
			memoryFree -= entry.section(' ', -2, -2 ).toInt();
	}
	memFile.close();

	return 1024 * memoryFree;
#else
	// outro SO nao implementado ainda
	Q_ASSERT(false);
	return 0;
#endif
}
//-----------------------------------------------------------------------------
u32 VizCMemoryUtils::sizeOfQString(const QString& str)
{
	return sizeof( str ) + sizeof( QChar ) * str.capacity();
}
//-----------------------------------------------------------------------------
u32 VizCMemoryUtils::sizeOfSecurityPeriods(const QVector<core::VizCSecurityPeriod>& securityPeriods)
{
	return sizeof( securityPeriods ) + sizeOfSecurityPeriod() * securityPeriods.capacity();
}
//-----------------------------------------------------------------------------
u32 VizCMemoryUtils::sizeOfRealTimePeriods(const QVector<core::VizCRealTimePeriod>& realTimePeriods)
{
	return sizeof( realTimePeriods ) + sizeOfRealTimePeriod() * realTimePeriods.capacity();
}
//-----------------------------------------------------------------------------
u32 VizCMemoryUtils::sizeOfSecurityPeriod()
{
	u32 size = 0;

	size += sizeof(s32);
	size += sizeOfQString(QString("123456"));
	size += sizeof(f64);
	size += sizeof(f64);
	size += sizeof(f64);
	size += sizeof(f64);
	size += sizeof(f64);
	size += sizeof(u64);
	size += sizeof(f64);
	size += sizeof(VizEContractCorrection);
	size += sizeof(f64);
	size += sizeof(QDate);
	size += sizeof(f64);
	size += sizeof(f64);
	size += sizeof(f64);
	size += sizeof(s32);
	size += sizeof(f64);
	size += sizeOfQString(QString("12"));
	size += sizeOfQString(QString("123"));
	size += sizeof(s32);
	size += sizeof(QDate);

	return size;

}
//-----------------------------------------------------------------------------
u32 VizCMemoryUtils::sizeOfRealTimePeriod()
{
	u32 size = 0;

	size += sizeof(s32);
	size += sizeof(u64);
	size += sizeof(f64);
	size += sizeof(u64);
	size += sizeof(f64);
	size += sizeof(f64);
	size += sizeof(VizEMarketSegment);
	size += sizeOfQString(QString("1234"));
	size += sizeof(QDateTime);
	size += sizeof(f64);
	
	return size;
}
//-----------------------------------------------------------------------------
u32 VizCMemoryUtils::sizeOfSecurity(const core::VizCSecurity& security)
{
	u32 size = 0;

	size += sizeof(security.SecurityId);
	size += sizeOfQString(security.ISIN);
	size += sizeOfQString(security.ISINObject);
	size += sizeOfQString(security.Symbol);
	size += sizeof(security.Bdi);
	size += sizeOfQString(security.BdiDescription);
	size += sizeof(security.DistributionNumber);
	size += sizeof(security.MarketType);
	size += sizeOfQString(security.MarketDescription);
	size += sizeof(security.SerialNumber);
	size += sizeOfQString(security.SecurityType);
	size += sizeof(security.DueDate);
	size += sizeof(security.Price);
	size += sizeOfQString(security.OptionStyle);
	size += sizeof(security.CurrecyType);
	size += sizeOfQString(security.CurrecyTypeDescription);
	size += sizeOfQString(security.Protection);
	size += sizeof(security.Negotiable);
	size += sizeof(security.NegotiableStart);
	size += sizeof(security.NegotiableEnd);
	size += sizeof(security.InfoUpdated);

	size += sizeOfSecurityPeriods(security.SecurityPeriods);
	size += sizeOfRealTimePeriods(security.RealTimePeriods);

	return size;
}
//-----------------------------------------------------------------------------
u32 VizCMemoryUtils::sizeOfSecurities(const QVector<core::VizCSecurity>& securities)
{
	u32 size = 0;

	foreach(core::VizCSecurity security, securities)
	{
		size += sizeOfSecurity(security);
	}
	
	return size;
}
//-----------------------------------------------------------------------------
u32 VizCMemoryUtils::sizeOfSplit()
{
	u32 size = 0;

	size += sizeof(s32);
	size += sizeof(u64);
	size += sizeof(u64);
	size += sizeof(f64);
	size += sizeof(f64);
	size += sizeof(QDate);

	return size;
}
//-----------------------------------------------------------------------------
u32 VizCMemoryUtils::sizeOfSplits(const QVector<core::VizCSplit>& splits)
{
	return sizeof( splits ) + sizeOfSplit() * splits.capacity();
}
//-----------------------------------------------------------------------------
u32 VizCMemoryUtils::sizeOfCompany(const core::VizCCompany& company)
{
	u32 size = 0;

	size += sizeof(company.CompanyId);
	size += sizeOfQString(company.Name);
	size += sizeOfQString(company.WholeName);
	size += sizeOfQString(company.PartialSecuritySymbol);
	size += sizeOfQString(company.CNPJ);
	size += sizeof(company.IssuerCreation);
	size += sizeOfQString(company.CurrentStatus);
	size += sizeof(company.InfoUpdated);
	size += sizeof(company.CCVM);

	size += sizeOfSplits(company.Splits);
	size += sizeOfSecurities(company.Securities);

	return size;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
