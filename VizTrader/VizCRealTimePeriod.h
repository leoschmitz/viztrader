//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCREALTIMEPERIOD_H__
#define __VIZCREALTIMEPERIOD_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"

#include <QByteArray>
#include <QDateTime>
#include <QString>
//-----------------------------------------------------------------------------
namespace viz
{
namespace core
{
//-----------------------------------------------------------------------------
class VizCRealTimePeriod
{
public:
	VizCRealTimePeriod();
	VizCRealTimePeriod(s32 realTimePeriodId, u64 quantityTrades, f64 tradePrice, u64 quantityTradesAccumInDay, f64 highestPrice, f64 lowestPrice, VizEMarketSegment marketSegment, const QString& periodOfSettlementInDays, const QDateTime& tradeDateTime, f64 variation);
	VizCRealTimePeriod(const VizCRealTimePeriod& other);

	void set(s32 realTimePeriodId, u64 quantityTrades, f64 tradePrice, u64 quantityTradesAccumInDay, f64 highestPrice, f64 lowestPrice, VizEMarketSegment marketSegment, const QString& periodOfSettlementInDays, const QDateTime& tradeDateTime, f64 variation);
	void get(s32& realTimePeriodId, u64& quantityTrades, f64& tradePrice, u64& quantityTradesAccumInDay, f64& highestPrice, f64& lowestPrice, VizEMarketSegment& marketSegment, QString& periodOfSettlementInDays, QDateTime& tradeDateTime, f64& variation) const;
	void set(const VizCRealTimePeriod& other);
	void get(VizCRealTimePeriod& other) const;

	bool isValid() const;
	u16 crc() const;

	bool operator!=(const VizCRealTimePeriod& other) const;
	bool operator==(const VizCRealTimePeriod& other) const;
	bool operator<(const VizCRealTimePeriod& other);
	VizCRealTimePeriod& operator=(const VizCRealTimePeriod& other);

	s32 RealTimePeriodId;
	u64 QuantityTrades;
	f64 TradePrice;
	u64 QuantityTradesAccumInDay;
	f64 HighestPrice;
	f64 LowestPrice;
	VizEMarketSegment MarketSegment;
	QString PeriodOfSettlementInDays;
	QDateTime TradeDateTime;	
	f64 Variation;
};
//-----------------------------------------------------------------------------
static inline QDataStream& operator>>(QDataStream& in, VizCRealTimePeriod& realTimePeriod)
{
	in
		>> realTimePeriod.RealTimePeriodId
		>> realTimePeriod.QuantityTrades
		>> realTimePeriod.TradePrice
		>> realTimePeriod.QuantityTradesAccumInDay
		>> realTimePeriod.HighestPrice
		>> realTimePeriod.LowestPrice
		>> realTimePeriod.MarketSegment
		>> realTimePeriod.PeriodOfSettlementInDays
		>> realTimePeriod.TradeDateTime
		>> realTimePeriod.Variation;		

	return in;
}
//-----------------------------------------------------------------------------
static inline QDataStream& operator<<(QDataStream& out, const VizCRealTimePeriod& realTimePeriod)
{
	out
		<< realTimePeriod.RealTimePeriodId
		<< realTimePeriod.QuantityTrades
		<< realTimePeriod.TradePrice
		<< realTimePeriod.QuantityTradesAccumInDay
		<< realTimePeriod.HighestPrice
		<< realTimePeriod.LowestPrice
		<< realTimePeriod.MarketSegment
		<< realTimePeriod.PeriodOfSettlementInDays
		<< realTimePeriod.TradeDateTime
		<< realTimePeriod.Variation;

	return out;
} 
//-----------------------------------------------------------------------------
static u32 approximateSizeOfARealTimePeriod()
{
	return	sizeof(s32) + // RealTimePeriodId;
		sizeof(u64) + // QuantityTrades;
		sizeof(f64) + // TradePrice;
		sizeof(u64) + // QuantityTradesAccumInDay;
		sizeof(f64) + // HighestPrice;
		sizeof(f64) + //LowestPrice;
		sizeof(VizEMarketSegment) + // MarketSegment;
		sizeof(QString("1234")) +  // PeriodOfSettlementInDays;
		sizeof(QDateTime) + // TradeDateTime
		sizeof(f64);  // Variation;			
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
