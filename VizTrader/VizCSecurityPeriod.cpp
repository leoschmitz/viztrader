//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCSecurityPeriod.h"
#include "VizMath.h"
#include "VizCMemoryUtils.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace core
{
//-----------------------------------------------------------------------------
VizCSecurityPeriod::VizCSecurityPeriod() : SecurityPeriodId(-1), EspeciEvent(""), OpenPrice(0.0), ClosePrice(0.0), MaxPrice(0.0), MinPrice(0.0), AveragePrice(0.0), QuantityTrades(0), INDOPC(viz::VECC_INVALID), TotalVolume(0.0), StrikePrice(0.0), CurrentValue(0.0), BestSellOffer(0.0), BestBuyOffer(0.0), QuotationFactor(0), StrikePricePoints(0.0), DISMES(-1), ForwardMarketTermDays(""), Currency("")
{
}
//-----------------------------------------------------------------------------
VizCSecurityPeriod::VizCSecurityPeriod(const QString& especiEvent, const f64 openPrice, const f64& closePrice, const f64& maxPrice, const f64& averagePrice, const f64& minPrice, const viz::u64& quantityTrades, const f64& totalVolume, const VizEContractCorrection& indopc, const f64& strikePrice, const QDate& maturityDate, const f64& currentValue, const f64& bestSellOffer, const f64& bestBuyOffer, const s32& quotationFactor, const f64& strikePricePoints, const QString& forwardMarketTermDays, const QString& currency, const s32& dismes, const QDate& exchangeDate) : SecurityPeriodId(-1), EspeciEvent(especiEvent), OpenPrice(openPrice), ClosePrice(closePrice), MaxPrice(maxPrice), MinPrice(minPrice), AveragePrice(averagePrice), QuantityTrades(quantityTrades), TotalVolume(totalVolume), StrikePrice(strikePrice), MaturityDate(maturityDate), CurrentValue(currentValue), BestSellOffer(bestSellOffer), BestBuyOffer(bestBuyOffer), INDOPC(indopc), StrikePricePoints(strikePricePoints), ForwardMarketTermDays(forwardMarketTermDays), Currency(currency), QuotationFactor(quotationFactor), DISMES(dismes), ExchangeDate(exchangeDate) 
{
}
//-----------------------------------------------------------------------------
VizCSecurityPeriod::VizCSecurityPeriod(const VizCSecurityPeriod& other)
{
	set(other);
}
//-----------------------------------------------------------------------------
void VizCSecurityPeriod::set(s32 securityPeriodId, const QString& especiEvent, const f64& openPrice, const f64& closePrice, const f64& maxPrice, const f64& minPrice, const f64& averagePrice, const viz::u64& quantityTrades, const f64& totalVolume, const VizEContractCorrection& indopc, const f64& strikePrice, const QDate& maturityDate, const f64& currentValue, const f64& bestSellOffer, const f64& bestBuyOffer, const s32& quotationFactor, const f64& strikePricePoints, const QString& forwardMarketTermDays, const QString& currency, const s32& dismes, const QDate& exchangeDate)
{
	SecurityPeriodId = securityPeriodId;
	EspeciEvent = especiEvent;
	OpenPrice = openPrice;
	ClosePrice = closePrice;
	MaxPrice = maxPrice;
	MinPrice = minPrice;
	AveragePrice = averagePrice;
	QuantityTrades = quantityTrades;
	TotalVolume = totalVolume;
	INDOPC = indopc;
	StrikePrice = strikePrice;
	MaturityDate = maturityDate;
	CurrentValue = currentValue;
	BestSellOffer = bestSellOffer;
	BestBuyOffer = bestBuyOffer;
	QuotationFactor = quotationFactor;
	StrikePricePoints = strikePricePoints;
	ForwardMarketTermDays = forwardMarketTermDays;
	Currency = currency;
	DISMES = dismes;
	ExchangeDate = exchangeDate;
}
//-----------------------------------------------------------------------------
void VizCSecurityPeriod::get(s32& securityPeriodId, QString& especiEvent, f64& openPrice, f64& closePrice, f64& maxPrice, f64& minPrice, f64& averagePrice, viz::u64& quantityTrades, f64& totalVolume, VizEContractCorrection& indopc, f64& strikePrice, QDate& maturityDate, f64& currentValue, f64& bestSellOffer, f64& bestBuyOffer, s32& quotationFactor, f64& strikePricePoints, QString& forwardMarketTermDays, QString& currency, s32& dismes, QDate& exchangeDate) const
{
	securityPeriodId = SecurityPeriodId;
	especiEvent = EspeciEvent;
	openPrice = OpenPrice;
	closePrice = ClosePrice;
	maxPrice = MaxPrice;
	minPrice = MinPrice;
	averagePrice = AveragePrice;
	quantityTrades = QuantityTrades;
	totalVolume = TotalVolume;
	indopc = INDOPC;
	strikePrice = StrikePrice;
	maturityDate = MaturityDate;
	currentValue = CurrentValue;
	bestSellOffer = BestSellOffer;
	bestBuyOffer = BestBuyOffer;
	quotationFactor = QuotationFactor;
	strikePricePoints = StrikePricePoints;
	forwardMarketTermDays = ForwardMarketTermDays;
	currency = Currency;
	dismes = DISMES;
	exchangeDate = ExchangeDate;	
}
//-----------------------------------------------------------------------------
void VizCSecurityPeriod::set(const VizCSecurityPeriod& other)
{
	set(other.SecurityPeriodId, other.EspeciEvent, other.OpenPrice, other.ClosePrice, other.MaxPrice, other.MinPrice, other.AveragePrice, other.QuantityTrades, other.TotalVolume, other.INDOPC, other.StrikePrice, other.MaturityDate, other.CurrentValue, other.BestSellOffer, other.BestBuyOffer, other.QuotationFactor, other.StrikePricePoints, other.ForwardMarketTermDays, other.Currency, other.DISMES, other.ExchangeDate);
}
//-----------------------------------------------------------------------------
void VizCSecurityPeriod::get(VizCSecurityPeriod& other) const
{
	get(other.SecurityPeriodId, other.EspeciEvent, other.OpenPrice, other.ClosePrice, other.MaxPrice, other.MinPrice, other.AveragePrice, other.QuantityTrades, other.TotalVolume, other.INDOPC, other.StrikePrice, other.MaturityDate, other.CurrentValue, other.BestSellOffer, other.BestBuyOffer, other.QuotationFactor, other.StrikePricePoints, other.ForwardMarketTermDays, other.Currency, other.DISMES, other.ExchangeDate);
}
//-----------------------------------------------------------------------------
u16 VizCSecurityPeriod::crc() const
{
	QString toChar(EspeciEvent);
	toChar.append(QString::number(SecurityPeriodId, 'g', 2));
	toChar.append(QString::number(OpenPrice, 'g', 2));
	toChar.append(QString::number(ClosePrice, 'g', 2));
	toChar.append(QString::number(MaxPrice, 'g', 2));
	toChar.append(QString::number(MinPrice, 'g', 2));
	toChar.append(QString::number(AveragePrice, 'g', 2));
	toChar.append(QString::number(QuantityTrades));
	toChar.append(QString::number(TotalVolume, 'g', 2));
	toChar.append(QString::number(static_cast<int>(INDOPC)));
	toChar.append(QString::number(StrikePrice, 'g', 2));
	toChar.append(QString::number(QuotationFactor));
	toChar.append(QString::number(StrikePricePoints, 'g', 6));
	toChar.append(ForwardMarketTermDays);
	toChar.append(Currency);
	toChar.append(QString::number(DISMES));
	toChar.append(ExchangeDate.toString(Qt::ISODate));
	return static_cast<u16>(qChecksum(toChar.toLatin1(), toChar.size()));
}
//-----------------------------------------------------------------------------
bool VizCSecurityPeriod::isValid() const
{
	return SecurityPeriodId != -1;
}
//-----------------------------------------------------------------------------
bool VizCSecurityPeriod::operator==(const VizCSecurityPeriod& other) const
{
	return (crc() == other.crc());
}
//-----------------------------------------------------------------------------
bool VizCSecurityPeriod::operator!=(const VizCSecurityPeriod& other) const
{
	return (crc() != other.crc());
}
//-----------------------------------------------------------------------------
bool VizCSecurityPeriod::operator<(const VizCSecurityPeriod& other) 
{
	return (ExchangeDate < other.ExchangeDate);
}
//-----------------------------------------------------------------------------
VizCSecurityPeriod& VizCSecurityPeriod::operator=(const VizCSecurityPeriod& other)
{
	set(other);
	return *this;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
