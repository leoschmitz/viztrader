//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCFILEH__
#define __VIZCFILEH__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include "VizCCompany.h" 

#include <QObject>
#include <QDir>
#include <QVector>
#include <QMap>
#include <QFile>
#include <QSet>
#include <QFileInfo>
#include <QString>
//-----------------------------------------------------------------------------
namespace viz
{
namespace io
{
//-----------------------------------------------------------------------------
// Essa classe lida com 3 extensões de arquivo
//  1) vzc - vizcompany: contém informação meta da companhia
//	2) vzh - vizhistory: contém informação histórica do ativo
//	3) vzr - vizrealtime: contém informação intraday do ativo
//
// VZC
// ---
// (QString) Version
// (core::VizCCompany) Company
// QVector<core::VizCSecurity> Securities
// (u16) crc 16 do arquivo
//
// VZH
// ---
// (core::VizCSecurityPeriod) period 1
// (u16) crc period 1
// (core::VizCSecurityPeriod) period 2
// (u16) crc period 2
// ...
// (core::VizCSecurityPeriod) period n
// (u16) crc period n
//
// VZR
// ---
// (core::VizCRealTimePeriod) period 1
// (u16) crc period 1
// (core::VizCRealTimePeriod) period 2
// (u16) crc period 2
// ...
// (core::VizCRealTimePeriod) period n
// (u16) crc period n
//////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
class VizCFile: public QObject
{	
	Q_OBJECT

public:
	VizCFile();
	~VizCFile();
	
	bool containsSymbol(const QString id) const;
	void loadTradeList(QVector<QStringList>& tradeList);
	bool loadTrade(const QString& symbol, core::VizCCompany &company, bool intraday = false);
	bool appendDaily(const QString& symbol, const core::VizCCompany& company);	

	static void setExecutableDirectory();
	static void setDataDirectory();	
	static void setDeletedDirectory();
private:
	QList<QString> localSecurityIds();
	QMap<QString, QFileInfo> AvailableSecurities;
	QSet<QString> RealTimeSecurities, DailySecurities;

	void checkAndRepairTradeFiles();	

	bool loadCompany(QFileInfo fileInfo, core::VizCCompany& in);	
	bool loadSecurity(const QString& symbol, core::VizCCompany& in);	
	bool loadSecurityPeriods(const QString& symbol, QVector<core::VizCSecurityPeriod>& in);		
	
	QFileInfo securityFileInfo(QString symbol) const;
	
	bool saveCompany(const core::VizCCompany& company);
	bool saveDailyPeriods(const QString& symbol, const QVector<core::VizCSecurityPeriod>& histo);
	bool updateSecurityFile(const QString& symbol, const QVector<core::VizCSecurityPeriod>& periods);	

	bool deleteInvalidFile(QFile& file);	
	bool deleteInvalidFile(const QFileInfo& fileInfo);

signals:
 	void percentageReady(QString symbol, int percentage);
	void error(const QString& fileError);
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif