//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCRealTimePeriod.h"
#include "VizMath.h"
#include "VizCMemoryUtils.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace core
{
//-----------------------------------------------------------------------------
VizCRealTimePeriod::VizCRealTimePeriod() : RealTimePeriodId(-1), QuantityTrades(0), TradePrice(0.0), QuantityTradesAccumInDay(0), HighestPrice(0.0),	LowestPrice(0.0), MarketSegment(VEMS_INVALID), PeriodOfSettlementInDays(""), TradeDateTime(QDateTime()), Variation(0.0)
{
}
//-----------------------------------------------------------------------------
VizCRealTimePeriod::VizCRealTimePeriod(s32 realTimePeriodId, u64 quantityTrades, f64 tradePrice, u64 quantityTradesAccumInDay, f64 highestPrice, f64 lowestPrice, VizEMarketSegment marketSegment, const QString& periodOfSettlementInDays, const QDateTime& tradeDateTime, f64 variation) 	: RealTimePeriodId(realTimePeriodId), QuantityTrades(quantityTrades), TradePrice(tradePrice), QuantityTradesAccumInDay(quantityTradesAccumInDay), HighestPrice(highestPrice), LowestPrice(lowestPrice), MarketSegment(marketSegment), PeriodOfSettlementInDays(periodOfSettlementInDays), TradeDateTime(tradeDateTime), Variation(variation)
{
}
//-----------------------------------------------------------------------------
VizCRealTimePeriod::VizCRealTimePeriod(const VizCRealTimePeriod& other)
{
	set(other);
}
//-----------------------------------------------------------------------------
void VizCRealTimePeriod::set(s32 realTimePeriodId, u64 quantityTrades, f64 tradePrice, u64 quantityTradesAccumInDay, f64 highestPrice, f64 lowestPrice, VizEMarketSegment marketSegment, const QString& periodOfSettlementInDays, const QDateTime& tradeDateTime, f64 variation)
{
	RealTimePeriodId = realTimePeriodId;
	QuantityTrades = quantityTrades;
	TradePrice = tradePrice; 
	QuantityTradesAccumInDay = quantityTradesAccumInDay;
	HighestPrice = highestPrice;
	LowestPrice = lowestPrice;
	MarketSegment = marketSegment;
	PeriodOfSettlementInDays = periodOfSettlementInDays;
	TradeDateTime = tradeDateTime;	
	Variation = variation;
}
//-----------------------------------------------------------------------------
void VizCRealTimePeriod::get(s32& realTimePeriodId, u64& quantityTrades, f64& tradePrice, u64& quantityTradesAccumInDay, f64& highestPrice, f64& lowestPrice, VizEMarketSegment& marketSegment, QString& periodOfSettlementInDays, QDateTime& tradeDateTime, f64& variation) const
{
	realTimePeriodId = RealTimePeriodId;
	quantityTrades = QuantityTrades;
	tradePrice = TradePrice; 
	quantityTradesAccumInDay = QuantityTradesAccumInDay;
	highestPrice = HighestPrice;
	lowestPrice = LowestPrice;
	marketSegment = MarketSegment;
	periodOfSettlementInDays = PeriodOfSettlementInDays;
	tradeDateTime = TradeDateTime;	
	variation = Variation;
}
//-----------------------------------------------------------------------------
void VizCRealTimePeriod::set(const VizCRealTimePeriod& other)
{
	set(other.RealTimePeriodId, other.QuantityTrades, other.TradePrice, other.QuantityTradesAccumInDay, other.HighestPrice, other.LowestPrice, other.MarketSegment, other.PeriodOfSettlementInDays, other.TradeDateTime, other.Variation);
}
//-----------------------------------------------------------------------------
void VizCRealTimePeriod::get(VizCRealTimePeriod& other) const
{
	get(other.RealTimePeriodId, other.QuantityTrades, other.TradePrice, other.QuantityTradesAccumInDay, other.HighestPrice, other.LowestPrice, other.MarketSegment, other.PeriodOfSettlementInDays, other.TradeDateTime, other.Variation);
}
//-----------------------------------------------------------------------------
u16 VizCRealTimePeriod::crc() const
{
	QString toChar(QString::number(RealTimePeriodId));
	toChar.append(QString::number(QuantityTrades));
	toChar.append(QString::number(TradePrice));
	toChar.append(QString::number(QuantityTradesAccumInDay));
	toChar.append(QString::number(HighestPrice));
	toChar.append(QString::number(LowestPrice));
	toChar.append(QString::number(static_cast<int>(MarketSegment)));
	toChar.append(PeriodOfSettlementInDays);
	toChar.append(TradeDateTime.toString(Qt::ISODate));
	toChar.append(QString::number(Variation));
	
	return static_cast<u16>(qChecksum(toChar.toLatin1(), toChar.size()));
}
//-----------------------------------------------------------------------------
bool VizCRealTimePeriod::isValid() const
{
	return RealTimePeriodId != -1;
}
//-----------------------------------------------------------------------------
bool VizCRealTimePeriod::operator==(const VizCRealTimePeriod& other) const
{
	return (crc() == other.crc());
}
//-----------------------------------------------------------------------------
bool VizCRealTimePeriod::operator!=(const VizCRealTimePeriod& other) const
{
	return (crc() != other.crc());
}
//-----------------------------------------------------------------------------
bool VizCRealTimePeriod::operator<(const VizCRealTimePeriod& other) 
{
	return (TradeDateTime < other.TradeDateTime);
}
//-----------------------------------------------------------------------------
VizCRealTimePeriod& VizCRealTimePeriod::operator=(const VizCRealTimePeriod& other)
{
	set(other);
	return *this;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
