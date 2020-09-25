//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCSecurity.h"
#include "VizCMemoryUtils.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace core
{
//-----------------------------------------------------------------------------
VizCSecurity::VizCSecurity()
:
SecurityId(-1), ISIN(""), ISINObject(""), Symbol(""),
Bdi(viz::VESB_INVALID), BdiDescription(""), MarketType(viz::VEMT_INVALID), MarketDescription(""),
DistributionNumber(-1), SerialNumber(-1), SecurityType(""),
DueDate(QDate()), Price(0.0), OptionStyle(""), CurrecyType(-1), CurrecyTypeDescription(""),
Protection(""), Negotiable(false), NegotiableStart(QDate()), NegotiableEnd(QDate()), InfoUpdated(QDate())
{
}
//-----------------------------------------------------------------------------
VizCSecurity::VizCSecurity
(
 const QString& isin, const QString& isinObject,
 const QString& symbol, const VizESecurityBDI bdi, const QString& bdiDescription,
 VizEMarketType MarketType, const QString& marketDescription, 
 s32 serialNumber, const QString& securityType,
 s32 distributionNumber,  const QDate& dueDate, f64 price,
 const QString& optionStyle, s32 currecyType, const QString& currecyTypeDescription,
 const QString& protection, const QDate& negotiableStart, const QDate& negotiableEnd, const QDate& infoUpdated,
 bool negotiable,
 const QVector<VizCSecurityPeriod>& securityPeriods,
 const QVector<VizCRealTimePeriod>& realTimePeriods
)
:
SecurityId(-1), ISIN(isin), ISINObject(isinObject), Symbol(symbol),
Bdi(bdi), BdiDescription(bdiDescription), MarketType(MarketType), MarketDescription(marketDescription),
DistributionNumber(distributionNumber), SerialNumber(serialNumber), SecurityType(securityType), DueDate(dueDate), Price(price),
OptionStyle(optionStyle), CurrecyType(currecyType), CurrecyTypeDescription(currecyTypeDescription),
Protection(protection), Negotiable(negotiable), SecurityPeriods(securityPeriods), RealTimePeriods(realTimePeriods), NegotiableStart(negotiableStart), NegotiableEnd(negotiableEnd), InfoUpdated(infoUpdated)
{
}
//-----------------------------------------------------------------------------
VizCSecurity::VizCSecurity(const VizCSecurity& other)
{
	set(other);
}
//-----------------------------------------------------------------------------
void VizCSecurity::set
(
 s32 securityId,
 const QString& isin, const QString& isinObject,
 const QString& symbol, const VizESecurityBDI bdi, const QString& bdiDescription,
 VizEMarketType marketType, const QString& marketDescription, 
 s32 serialNumber, const QString& securityType,
 s32 distributionNumber, const QDate& dueDate, f64 price,
 const QString& optionStyle, s32 currecyType, const QString& currecyTypeDescription,
 const QString& protection, const QDate& negotiableStart, const QDate& negotiableEnd, const QDate& infoUpdated,
 bool negotiable,
 const QVector<VizCSecurityPeriod>& securityPeriods, 
 const QVector<VizCRealTimePeriod>& realTimePeriods
)
{
	SecurityId = securityId;
	ISIN = isin;
	ISINObject = isinObject;
	Symbol = symbol;
	Bdi = bdi;
	BdiDescription = bdiDescription;
	MarketType = marketType;
	MarketDescription = marketDescription;
	DistributionNumber = distributionNumber;
	SerialNumber = serialNumber;
	SecurityType = securityType;
	DueDate = dueDate;
	Price = price;
	OptionStyle = optionStyle;
	CurrecyType = currecyType;
	CurrecyTypeDescription = currecyTypeDescription;
	Protection = protection;
	Negotiable = negotiable;
	SecurityPeriods = securityPeriods;
	RealTimePeriods = realTimePeriods;
	NegotiableStart = negotiableStart;
	NegotiableEnd = negotiableEnd;
	InfoUpdated = infoUpdated;
}
//-----------------------------------------------------------------------------
void VizCSecurity::get
(
s32& securityId, 
QString& isin, QString& isinObject,
QString& symbol, VizESecurityBDI& bdi, QString& bdiDescription,
VizEMarketType& marketType, QString& marketDescription, 
s32& serialNumber, QString& securityType,
s32& distributionNumber, QDate& dueDate, f64& price,
QString& optionStyle, s32& currecyType, QString& currecyTypeDescription,
QString& protection, QDate& negotiableStart, QDate& negotiableEnd, QDate& infoUpdated,
bool& negotiable,
QVector<VizCSecurityPeriod>& securityPeriods,
QVector<VizCRealTimePeriod>& realTimePeriods
) const
{
	securityId = SecurityId;
	isin = ISIN;
	isinObject = ISINObject;
	symbol = Symbol;
	bdi = Bdi;
	bdiDescription = BdiDescription;
	marketType = MarketType;
	marketDescription = MarketDescription;
	distributionNumber = DistributionNumber;
	serialNumber = SerialNumber;
	securityType = SecurityType;
	dueDate = DueDate;
	price = Price;
	optionStyle = OptionStyle;
	currecyType = CurrecyType;
	currecyTypeDescription = CurrecyTypeDescription;
	protection = Protection;
	negotiable = Negotiable;
	securityPeriods = SecurityPeriods;
	realTimePeriods = RealTimePeriods;
	negotiableStart = NegotiableStart;
	negotiableEnd = NegotiableEnd;
	infoUpdated = InfoUpdated;
}
//-----------------------------------------------------------------------------
void VizCSecurity::set(const VizCSecurity& other)
{
	//set(other.SecurityId, other.ISIN, other.Symbol, other.Bdi, other.TPMerc, other.Type, other.SecurityPeriods);


	set
	(
		other.SecurityId,
		other.ISIN,
		other.ISINObject,
		other.Symbol,
		other.Bdi,
		other.BdiDescription,
		other.MarketType,
		other.MarketDescription,
		other.SerialNumber,
		other.SecurityType,
		other.DistributionNumber,
		other.DueDate,
		other.Price,
		other.OptionStyle,
		other.CurrecyType,
		other.CurrecyTypeDescription,
		other.Protection,
		other.NegotiableStart,
		other.NegotiableEnd,
		other.InfoUpdated,
		other.Negotiable,
		other.SecurityPeriods,
		other.RealTimePeriods
	);

}
//-----------------------------------------------------------------------------
void VizCSecurity::get(VizCSecurity& other) const
{
	//get(other.SecurityId, other.ISIN, other.Symbol, other.Bdi, other.TPMerc, other.Type, other.SecurityPeriods);

	get
	(
		other.SecurityId,
		other.ISIN,
		other.ISINObject,
		other.Symbol,
		other.Bdi,
		other.BdiDescription,
		other.MarketType,
		other.MarketDescription,
		other.SerialNumber,
		other.SecurityType,
		other.DistributionNumber,
		other.DueDate,
		other.Price,
		other.OptionStyle,
		other.CurrecyType,
		other.CurrecyTypeDescription,
		other.Protection,
		other.NegotiableStart,
		other.NegotiableEnd,
		other.InfoUpdated,
		other.Negotiable,
		other.SecurityPeriods,
		other.RealTimePeriods
	);
}
//-----------------------------------------------------------------------------
void VizCSecurity::mergeAttributes(const VizCSecurity& other)
{
	SecurityId = (SecurityId == -1) ? other.SecurityId : SecurityId;
	ISIN = (ISIN.isEmpty()) ? other.ISIN : ISIN; // International Securities Identifying Number
	ISINObject = (ISINObject.isEmpty()) ? other.ISINObject : ISINObject;
	Symbol = (Symbol.isEmpty()) ? other.Symbol : Symbol;
	Bdi = (Bdi == viz::VESB_INVALID) ? other.Bdi : Bdi;
	BdiDescription = (BdiDescription.isEmpty()) ? other.BdiDescription : BdiDescription;
	MarketType = (MarketType == viz::VEMT_INVALID) ? other.MarketType : MarketType;
	MarketDescription = (MarketDescription.isEmpty()) ? other.MarketDescription : MarketDescription;
	DistributionNumber = (DistributionNumber == -1) ? other.DistributionNumber : DistributionNumber;
	SerialNumber = (SerialNumber == -1) ? other.SerialNumber : SerialNumber;
	SecurityType = (SecurityType.isEmpty()) ? other.SecurityType : SecurityType;	
	DueDate = (DueDate.isNull()) ? other.DueDate : DueDate;	
	Price = (Price == 0.0) ? other.Price : Price;
	OptionStyle = (OptionStyle.isEmpty()) ? other.OptionStyle : OptionStyle;
	CurrecyType = (CurrecyType == -1) ? other.CurrecyType : CurrecyType;
	CurrecyTypeDescription = (CurrecyTypeDescription.isEmpty()) ? other.CurrecyTypeDescription : CurrecyTypeDescription;	
	Protection = (Protection.isEmpty()) ? other.Protection : Protection;	
	Negotiable &= other.Negotiable; 

	// Casos que exigem cuidado para sincronização no servidor
	NegotiableStart = (NegotiableEnd > NegotiableStart) ? 
		(other.NegotiableStart > NegotiableStart ? other.NegotiableStart : NegotiableStart) :
		NegotiableStart;
	NegotiableEnd = other.NegotiableEnd > NegotiableEnd ? other.NegotiableEnd : NegotiableEnd;
	InfoUpdated = other.InfoUpdated > InfoUpdated ? other.InfoUpdated : InfoUpdated;

}
//-----------------------------------------------------------------------------
VizCSecurity& VizCSecurity::operator=(const VizCSecurity& other)
{
	set(other);
	return *this;
}		
//-----------------------------------------------------------------------------
bool VizCSecurity::operator==(const VizCSecurity& other) const
{
	return (crc() == other.crc());
}
//-----------------------------------------------------------------------------
bool VizCSecurity::operator!=(const VizCSecurity& other) const
{
	return crc() != other.crc();
}
//-----------------------------------------------------------------------------
bool VizCSecurity::operator<(const VizCSecurity& other) const
{
	return Symbol < other.Symbol;
}
//-----------------------------------------------------------------------------
u16 VizCSecurity::crc() const
{
	QString toChar(QString::number(SecurityId));
	toChar.append(ISIN);
	toChar.append(ISINObject);
	toChar.append(Symbol);
	toChar.append(QString::number(Bdi));
	toChar.append(BdiDescription);
	toChar.append(QString::number(MarketType));
	toChar.append(MarketDescription);
	toChar.append(QString::number(DistributionNumber));
	toChar.append(QString::number(SerialNumber));
	toChar.append(SecurityType);
	toChar.append(DueDate.toString(Qt::ISODate));
	toChar.append(QString::number(Price, 'g', 2));
	toChar.append(OptionStyle);
	toChar.append(QString::number(CurrecyType));
	toChar.append(CurrecyTypeDescription);
	toChar.append(Protection);

	toChar.append(NegotiableStart.toString(Qt::ISODate));
	toChar.append(NegotiableEnd.toString(Qt::ISODate));
	toChar.append(InfoUpdated.toString(Qt::ISODate));

	return static_cast<u16>(qChecksum(toChar.toLatin1(), toChar.size()));
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
