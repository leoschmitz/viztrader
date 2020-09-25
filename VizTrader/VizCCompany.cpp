//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCCompany.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace core
{
//-----------------------------------------------------------------------------
VizCCompany::VizCCompany() : CompanyId(-1), Name(""), WholeName(""), PartialSecuritySymbol(""), CNPJ(""), IssuerCreation(QDate()), CurrentStatus(""), InfoUpdated(QDate()), CCVM(0)
{
}
//-----------------------------------------------------------------------------
VizCCompany::VizCCompany(const QString& name, const QString& wholeName, const QString& partialSecuritySymbol, const QString& cnpj, const QDate& issuerCreation, const QString& currentStatus, const QDate& infoUpdated, const u32& ccvm, const QVector<VizCSplit>& splits, const QVector<VizCSecurity>& securities) : CompanyId(-1), Name(name), WholeName(wholeName), PartialSecuritySymbol(partialSecuritySymbol), CNPJ(cnpj), IssuerCreation(issuerCreation), CurrentStatus(currentStatus), Securities(securities), InfoUpdated(infoUpdated), CCVM(ccvm), Splits(splits)
{
}
//-----------------------------------------------------------------------------
VizCCompany::VizCCompany(const VizCCompany& other)
{
	set(other);
}
//-----------------------------------------------------------------------------
void VizCCompany::set(s32 companyId, const QString& name, const QString& wholeName, const QString& partialSecuritySymbol, const QString& cnpj, const QDate& issuerCreation, const QString& currentStatus, const QDate& infoUpdated, const u32& ccvm, const QVector<VizCSplit>& splits, const QVector<VizCSecurity>& securities)
{
	CompanyId = companyId;
	Name = name;
	WholeName = wholeName;
	PartialSecuritySymbol = partialSecuritySymbol;
	CNPJ = cnpj;
	IssuerCreation = issuerCreation;
	CurrentStatus = currentStatus;
	Securities = securities;
	Splits = splits;
	InfoUpdated = infoUpdated;
	CCVM = ccvm;
}
//-----------------------------------------------------------------------------
void VizCCompany::get(s32 companyId, QString& name, QString& wholeName, QString& partialSecuritySymbol, QString& cnpj, QDate& issuerCreation, QString& currentStatus, QDate& infoUpdated, u32& ccvm, QVector<VizCSplit>& splits, QVector<VizCSecurity>& securities) const
{
	companyId = CompanyId;
	name = Name;
	wholeName = WholeName;
	partialSecuritySymbol = PartialSecuritySymbol;
	cnpj = CNPJ;
	issuerCreation = IssuerCreation;
	currentStatus = CurrentStatus;
	securities = Securities;
	splits = Splits;
	infoUpdated = InfoUpdated;
	ccvm = CCVM;
}
//-----------------------------------------------------------------------------
void VizCCompany::set(const VizCCompany& other)
{
	set(other.CompanyId, other.Name, other.WholeName, other.PartialSecuritySymbol, other.CNPJ, other.IssuerCreation, other.CurrentStatus, other.InfoUpdated, other.CCVM, other.Splits, other.Securities);
}
//-----------------------------------------------------------------------------
void VizCCompany::get(VizCCompany& other) const
{
	get(other.CompanyId, other.Name, other.WholeName, other.PartialSecuritySymbol, other.CNPJ, other.IssuerCreation, other.CurrentStatus, other.InfoUpdated, other.CCVM, other.Splits, other.Securities);
}
//-----------------------------------------------------------------------------
// Não checa se os atributos são conflitantes.
// A prioridade é da propria classe se houver conflito.
void VizCCompany::mergeAttributes(const VizCCompany& other)
{	
	CompanyId = (CompanyId == -1) ? other.CompanyId : CompanyId;
	Name = (Name.isEmpty()) ? other.Name : Name;
	WholeName = (WholeName.isEmpty()) ? other.WholeName : WholeName;
	PartialSecuritySymbol = (PartialSecuritySymbol.isEmpty()) ? other.PartialSecuritySymbol : PartialSecuritySymbol;
	CNPJ = (CNPJ.isEmpty()) ? other.CNPJ : CNPJ;			
	IssuerCreation = IssuerCreation.isNull() ? other.IssuerCreation : IssuerCreation;
	CurrentStatus = (CurrentStatus.isEmpty()) ? other.CurrentStatus : CurrentStatus;
	InfoUpdated = other.InfoUpdated > InfoUpdated ? other.InfoUpdated : InfoUpdated;
	CCVM = (CCVM == 0) ? other.CCVM : CCVM;
}
//-----------------------------------------------------------------------------
VizCCompany& VizCCompany::operator=(const VizCCompany& other)
{
	set(other);
	return *this;
}		
//-----------------------------------------------------------------------------
bool VizCCompany::operator==(const VizCCompany& other) const
{
	return (crc() == other.crc());
}
//-----------------------------------------------------------------------------
bool VizCCompany::operator!=(const VizCCompany& other) const
{
	return crc() != other.crc();
}
//-----------------------------------------------------------------------------
bool VizCCompany::operator<(const VizCCompany& other) const
{
	return PartialSecuritySymbol < other.PartialSecuritySymbol;
}
//-----------------------------------------------------------------------------
quint16 VizCCompany::crc() const
{
	QString toChar(QString::number(CompanyId));
	toChar.append(Name);
	toChar.append(WholeName);
	toChar.append(PartialSecuritySymbol);
	toChar.append(CNPJ);
	toChar.append(IssuerCreation.toString(Qt::ISODate));
	toChar.append(CurrentStatus);
	toChar.append(InfoUpdated.toString(Qt::ISODate));
	toChar.append(QString::number(CCVM));
	return qChecksum(toChar.toLatin1(), toChar.size());
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
