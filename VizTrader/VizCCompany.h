//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCCOMPANY_H__
#define __VIZCCOMPANY_H__
//-----------------------------------------------------------------------------
#include <QString>
#include <QByteArray>
#include <QDataStream>
#include <QVector>
#include <QDate>

#include "VizCSplit.h"
#include "VizTypes.h"
#include "VizCSecurity.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace core
{
//-----------------------------------------------------------------------------
class VizCCompany
{
public:
	VizCCompany();
	VizCCompany(const QString& name, const QString& wholeName, const QString& partialSecuritySymbol, const QString& cnpj, const QDate& issuerCreation, const QString& currentStatus, const QDate& infoUpdated, const u32& ccvm, const QVector<VizCSplit>& splits = QVector<VizCSplit>(), const QVector<VizCSecurity>& securities = QVector<VizCSecurity>());
	VizCCompany(const VizCCompany& other);
	void set(s32 companyId, const QString& name, const QString& wholeName, const QString& partialSecuritySymbol, const QString& cnpj, const QDate& issuerCreation, const QString& currentStatus, const QDate& infoUpdated, const u32& ccvm, const QVector<VizCSplit>& splits = QVector<VizCSplit>(), const QVector<VizCSecurity>& securities = QVector<VizCSecurity>());
	void set(const VizCCompany& other);
	void get(s32 companyId, QString& name, QString& wholeName, QString& partialSecuritySymbol, QString& cnpj, QDate& issuerCreation, QString& currentStatus, QDate& infoUpdated, u32& ccvm, QVector<VizCSplit>& splits, QVector<VizCSecurity>& securities) const;
	void get(VizCCompany& other) const;

	u16 crc() const;
	void mergeAttributes(const VizCCompany& other);

	VizCCompany& operator=(const VizCCompany& other);
	bool operator==(const VizCCompany& other) const;
	bool operator!=(const VizCCompany& other) const;
	bool operator<(const VizCCompany& other) const;
	
	s32 CompanyId;
	QString Name;
	QString WholeName;
	QString PartialSecuritySymbol;
	QString CNPJ;
	QDate IssuerCreation;
	QString CurrentStatus;
	QDate InfoUpdated;
	u32 CCVM;

	QVector<VizCSplit> Splits;
	QVector<VizCSecurity> Securities;	
};
//-----------------------------------------------------------------------------
static inline QDataStream& operator>>(QDataStream& in, VizCCompany& company)
{
	//talvez colocar o >> Securities tambem 

	in
		>> company.CompanyId
		>> company.Name
		>> company.WholeName
		>> company.PartialSecuritySymbol
		>> company.CNPJ
		>> company.IssuerCreation
		>> company.CurrentStatus
		>> company.InfoUpdated
		>> company.CCVM
		>> company.Splits;

	return in;	
}
//-----------------------------------------------------------------------------
static inline QDataStream& operator<<(QDataStream& out, const VizCCompany& company)
{
	//talvez colocar o << Securities tambem

	out
		<< company.CompanyId
		<< company.Name
		<< company.WholeName
		<< company.PartialSecuritySymbol
		<< company.CNPJ
		<< company.IssuerCreation
		<< company.CurrentStatus
		<< company.InfoUpdated
		<< company.CCVM
		<< company.Splits;	

	return out;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
