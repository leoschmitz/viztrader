//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCMEMORYUTILS_H__
#define __VIZCMEMORYUTILS_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include "VizCSecurity.h"
#include "VizCSecurityPeriod.h"
#include "VizCRealTimePeriod.h"
#include "VizCSplit.h"
#include "VizCCompany.h"
#include <QString>
#include <QVector>
//-----------------------------------------------------------------------------
namespace viz
{
namespace system
{
//-----------------------------------------------------------------------------
class VizCMemoryUtils
{
public:
	static u64 totalSystemMemory();
	static u64 availableSystemMemory();
	static void formatByteSize(u64 bytes, f64& value, QString& suffix);
	static u32 sizeOfQString(const QString& str);
	static u32 sizeOfSecurityPeriod();
	static u32 sizeOfRealTimePeriod();
	static u32 sizeOfSplit();
	static u32 sizeOfSecurityPeriods(const QVector<core::VizCSecurityPeriod>& securityPeriods);
	static u32 sizeOfRealTimePeriods(const QVector<core::VizCRealTimePeriod>& realTimePeriods);
	static u32 sizeOfSecurity(const core::VizCSecurity& security);
	static u32 sizeOfSecurities(const QVector<core::VizCSecurity>& securities);
	static u32 sizeOfSplits(const QVector<core::VizCSplit>& splits);
	static u32 sizeOfCompany(const core::VizCCompany& company);
	
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
