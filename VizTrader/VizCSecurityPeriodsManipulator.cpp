//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCSecurityPeriodsManipulator.h"

#include <QDebug>
#include <limits>
//-----------------------------------------------------------------------------
namespace viz
{
namespace core
{
//-----------------------------------------------------------------------------
VizCSecurityPeriodsManipulator* VizCSecurityPeriodsManipulator::Instance = 0;
//-----------------------------------------------------------------------------
VizCSecurityPeriodsManipulator* VizCSecurityPeriodsManipulator::instance()
{
	if(!Instance)
	{
		Instance = new VizCSecurityPeriodsManipulator();
	}
	return Instance;
}
//-----------------------------------------------------------------------------
VizCSecurityPeriodsManipulator::VizCSecurityPeriodsManipulator()
{
}
//-----------------------------------------------------------------------------
QVector<VizCSecurityPeriod> VizCSecurityPeriodsManipulator::adjustSecurityPeriodsSplits(QVector<core::VizCSplit>& splits, const QVector<VizCSecurityPeriod>& periods)
{
	
	if (splits.isEmpty()) return periods;

	QVector<VizCSecurityPeriod> out(periods);

	s32 size = periods.size();	

	foreach (const core::VizCSplit& split, splits)
	{
		double adjustFactor = static_cast<double>(split.QuantityAfter) / static_cast<double>(split.QuantityBefore);

		for (int i = 0; i < size; i++)
		{
			if (out.at(i).ExchangeDate < split.Date)
			{
				out[i].AveragePrice *= adjustFactor;
				out[i].BestBuyOffer *= adjustFactor;
				out[i].BestSellOffer*= adjustFactor;
				out[i].ClosePrice   *= adjustFactor;
				out[i].CurrentValue *= adjustFactor;
				out[i].MaxPrice     *= adjustFactor;
				out[i].MinPrice     *= adjustFactor;
				out[i].OpenPrice    *= adjustFactor;
				out[i].TotalVolume  *= adjustFactor;
//				out[i].Variation    *= adjustFactor;
				out[i].StrikePrice  *= adjustFactor;
			}
			else
			{
				break;
			}

		}
	}

	for (int i = 0; i < size; i++)
	{
		out[i].AveragePrice /= out[i].QuotationFactor;
		out[i].BestBuyOffer /= out[i].QuotationFactor;
		out[i].BestSellOffer/= out[i].QuotationFactor;
		out[i].ClosePrice   /= out[i].QuotationFactor;
		out[i].CurrentValue /= out[i].QuotationFactor;
		out[i].MaxPrice     /= out[i].QuotationFactor;
		out[i].MinPrice     /= out[i].QuotationFactor;
		out[i].OpenPrice    /= out[i].QuotationFactor;
		out[i].TotalVolume  /= out[i].QuotationFactor;
//		out[i].Variation    /= out[i].QuotationFactor;
		out[i].StrikePrice  /= out[i].QuotationFactor;
	}


	return out;
}
//-----------------------------------------------------------------------------
static void convertPeriodicityIntevalToDaily(const QVector<VizCSecurityPeriod>& periodsDaily, QVector<VizCTradePeriod>& out)
{
	if(periodsDaily.isEmpty()) return;

	s32 size = periodsDaily.size();	

	for(int i = 0; i < size; i++)
	{
		VizCTradePeriod p;

		/*p.OpenPrice = periodsDaily[i].OpenPrice;
		p.TimeStamp = periodsDaily[i].CloseTimeStamp;
		p.ClosePrice = periodsDaily[i].ClosePrice;
		p.MinPrice = periodsDaily[i].MinPrice;
		p.MaxPrice = periodsDaily[i].MaxPrice;
		p.TotalVolume = periodsDaily[i].TotalVolume;
		*/

		p.set(periodsDaily[i].OpenPrice, periodsDaily[i].ClosePrice, periodsDaily[i].MaxPrice, periodsDaily[i].MinPrice, periodsDaily[i].TotalVolume, QDateTime(periodsDaily[i].ExchangeDate));

		out.append(p);

	}


}
//-----------------------------------------------------------------------------
static void convertPeriodicityIntevalToWeeakly(const QVector<VizCSecurityPeriod>& periodsDaily, QVector<VizCTradePeriod>& out)
{
	if(periodsDaily.isEmpty()) return;

	VizCSecurityPeriod first = periodsDaily[0];
	f32 MaxPrice = first.MaxPrice;
	f32 MinPrice = first.MinPrice;
	f32 TotalVolume = first.TotalVolume;
	s32 yearOpen;
	s32 weekOpen = first.ExchangeDate.weekNumber(&yearOpen);

	s32 size = periodsDaily.size();
	for(int i = 1; i < size; i++)
	{
		bool isEnd = i == size - 1;
		s32 week = periodsDaily[i].ExchangeDate.weekNumber(&yearOpen);
		if(week != weekOpen || isEnd)
		{
			weekOpen = week;

			VizCTradePeriod p;
			if(isEnd)
			{
				if(periodsDaily[i].MaxPrice > MaxPrice)
					MaxPrice = periodsDaily[i].MaxPrice;
				if(periodsDaily[i].MinPrice < MinPrice)
					MinPrice = periodsDaily[i].MinPrice;
				TotalVolume += periodsDaily[i].TotalVolume;


				/*p.OpenPrice = first.OpenPrice;
				p.TimeStamp = periodsDaily[i].CloseTimeStamp;
				p.ClosePrice = periodsDaily[i].ClosePrice;
				p.MinPrice = MinPrice;
				p.MaxPrice = MaxPrice;
				p.TotalVolume = TotalVolume;
				*/

				p.set(first.OpenPrice, periodsDaily[i].ClosePrice, MaxPrice, MinPrice, TotalVolume, QDateTime(periodsDaily[i].ExchangeDate));

			}
			else
			{
				/*p.TimeStamp = periodsDaily[i-1].CloseTimeStamp;
				p.OpenPrice = first.OpenPrice;
				p.ClosePrice = periodsDaily[i-1].ClosePrice;
				p.MinPrice = MinPrice;
				p.MaxPrice = MaxPrice;
				p.TotalVolume = TotalVolume;
				*/
				
				p.set(first.OpenPrice, periodsDaily[i-1].ClosePrice, MaxPrice, MinPrice, TotalVolume, QDateTime(periodsDaily[i-1].ExchangeDate));

			}

			first = periodsDaily[i];
			MaxPrice = std::numeric_limits<f32>::min();
			MinPrice = std::numeric_limits<f32>::max();
			TotalVolume = 0;

			//qDebug() << p.CloseTimeStamp << p.OpenPrice << p.ClosePrice << p.MaxPrice << p.MinPrice << p.TotalVolume;

			out.append(p);
		}
		if(periodsDaily[i].MaxPrice > MaxPrice)
			MaxPrice = periodsDaily[i].MaxPrice;
		if(periodsDaily[i].MinPrice < MinPrice)
			MinPrice = periodsDaily[i].MinPrice;
		TotalVolume += periodsDaily[i].TotalVolume;
	}

}
//-----------------------------------------------------------------------------
static void convertPeriodicityIntevalToMonthly(const QVector<VizCSecurityPeriod>& periodsDaily, QVector<VizCTradePeriod>& out)
{
	if(periodsDaily.isEmpty()) return;
	
	VizCSecurityPeriod first = periodsDaily[0];
	f32 MaxPrice = first.MaxPrice;
	f32 MinPrice = first.MinPrice;
	f32 TotalVolume = first.TotalVolume;
	s32 monthOpen = first.ExchangeDate.month();

	s32 size = periodsDaily.size();
	for(int i = 1; i < size; i++)
	{
		bool isEnd = i == size - 1;
		s32 month = periodsDaily[i].ExchangeDate.month();

		if(month != monthOpen || isEnd)
		{
			VizCTradePeriod p;
			if(isEnd)
			{
				if(periodsDaily[i].MaxPrice > MaxPrice)
					MaxPrice = periodsDaily[i].MaxPrice;
				if(periodsDaily[i].MinPrice < MinPrice)
					MinPrice = periodsDaily[i].MinPrice;
				TotalVolume += periodsDaily[i].TotalVolume;

				/*
				p.TimeStamp = periodsDaily[i].CloseTimeStamp;
				p.OpenPrice = first.OpenPrice;
				p.ClosePrice = periodsDaily[i].ClosePrice;
				p.MinPrice = MinPrice;
				p.MaxPrice = MaxPrice;
				p.TotalVolume = TotalVolume;
				*/
				p.set(first.OpenPrice, periodsDaily[i].ClosePrice, MaxPrice, MinPrice, TotalVolume, QDateTime(periodsDaily[i].ExchangeDate));

			}
			else
			{
				/*p.TimeStamp = periodsDaily[i-1].CloseTimeStamp;
				p.OpenPrice = first.OpenPrice;
				p.ClosePrice = periodsDaily[i-1].ClosePrice;
				p.MinPrice = MinPrice;
				p.MaxPrice = MaxPrice;
				p.TotalVolume = TotalVolume;
				*/
				p.set(first.OpenPrice, periodsDaily[i-1].ClosePrice, MaxPrice, MinPrice, TotalVolume, QDateTime(periodsDaily[i-1].ExchangeDate));

			}

			first = periodsDaily[i];
			MaxPrice = std::numeric_limits<f32>::min();
			MinPrice = std::numeric_limits<f32>::max();
			TotalVolume = 0;

			monthOpen = month;

			out.append(p);
		}
		if(periodsDaily[i].MaxPrice > MaxPrice)
			MaxPrice = periodsDaily[i].MaxPrice;
		if(periodsDaily[i].MinPrice < MinPrice)
			MinPrice = periodsDaily[i].MinPrice;
		TotalVolume += periodsDaily[i].TotalVolume;
	}

}
//-----------------------------------------------------------------------------
static void convertPeriodicityIntevalToQuarterly(const QVector<VizCSecurityPeriod>& periodsDaily, QVector<VizCTradePeriod>& out)
{
	if(periodsDaily.isEmpty()) return;

	VizCSecurityPeriod first = periodsDaily[0];
	f32 MaxPrice = first.MaxPrice;
	f32 MinPrice = first.MinPrice;
	f32 TotalVolume = first.TotalVolume;
	s32 monthOpen = first.ExchangeDate.month();
	s32 Quarterly = 0;
	bool flagFirst = true;
	s32 size = periodsDaily.size();
	for(int i = 1; i < size; i++)
	{
		bool isEnd = i == size - 1;
		s32 month = periodsDaily[i].ExchangeDate.month();
		if(month != monthOpen)
		{
			monthOpen = month;
			Quarterly++;
		}

		//int n = flagFirst ? 1 : 3;
		int n = flagFirst ? 0 : 2;

		if(Quarterly == n || isEnd)
		{
			flagFirst = false;
			Quarterly = 0;

			VizCTradePeriod p;
			if(isEnd)
			{
				if(periodsDaily[i].MaxPrice > MaxPrice)
					MaxPrice = periodsDaily[i].MaxPrice;
				if(periodsDaily[i].MinPrice < MinPrice)
					MinPrice = periodsDaily[i].MinPrice;
				TotalVolume += periodsDaily[i].TotalVolume;

				/*p.TimeStamp = periodsDaily[i].CloseTimeStamp;
				p.OpenPrice = first.OpenPrice;
				p.ClosePrice = periodsDaily[i].ClosePrice;
				p.MinPrice = MinPrice;
				p.MaxPrice = MaxPrice;
				p.TotalVolume = TotalVolume;
				*/
				
				p.set(first.OpenPrice, periodsDaily[i].ClosePrice, MaxPrice, MinPrice, TotalVolume, QDateTime(periodsDaily[i].ExchangeDate));

			}
			else
			{
				/*
				p.TimeStamp = periodsDaily[i-1].CloseTimeStamp;
				p.OpenPrice = first.OpenPrice;
				p.ClosePrice = periodsDaily[i-1].ClosePrice;
				p.MinPrice = MinPrice;
				p.MaxPrice = MaxPrice;
				p.TotalVolume = TotalVolume;
				*/

				p.set(first.OpenPrice, periodsDaily[i-1].ClosePrice, MaxPrice, MinPrice, TotalVolume, QDateTime(periodsDaily[i-1].ExchangeDate));

			}

			first = periodsDaily[i];
			MaxPrice = std::numeric_limits<f32>::min();
			MinPrice = std::numeric_limits<f32>::max();
			TotalVolume = 0;

			out.append(p);
		}
		if(periodsDaily[i].MaxPrice > MaxPrice)
			MaxPrice = periodsDaily[i].MaxPrice;
		if(periodsDaily[i].MinPrice < MinPrice)
			MinPrice = periodsDaily[i].MinPrice;
		TotalVolume += periodsDaily[i].TotalVolume;
	}
}
//-----------------------------------------------------------------------------
static void convertPeriodicityIntevalToYearly(const QVector<VizCSecurityPeriod>& periodsDaily, QVector<VizCTradePeriod>& out)
{
	if(periodsDaily.isEmpty()) return;

	VizCSecurityPeriod first = periodsDaily[0];
	f32 MaxPrice = first.MaxPrice;
	f32 MinPrice = first.MinPrice;
	f32 TotalVolume = first.TotalVolume;
	s32 yearOpen = first.ExchangeDate.year();
	s32 size = periodsDaily.size();
	for(int i = 1; i < size; i++)
	{
		s32 year = periodsDaily[i].ExchangeDate.year();
		bool isEnd = i == size - 1;
		if(year != yearOpen || isEnd)
		{
			yearOpen = year;

			VizCTradePeriod p;
			if(isEnd)
			{
				if(periodsDaily[i].MaxPrice > MaxPrice)
					MaxPrice = periodsDaily[i].MaxPrice;
				if(periodsDaily[i].MinPrice < MinPrice)
					MinPrice = periodsDaily[i].MinPrice;
				TotalVolume += periodsDaily[i].TotalVolume;

				/*p.TimeStamp = periodsDaily[i].CloseTimeStamp;
				p.OpenPrice = first.OpenPrice;
				p.ClosePrice = periodsDaily[i].ClosePrice;
				p.MinPrice = MinPrice;
				p.MaxPrice = MaxPrice;
				p.TotalVolume = TotalVolume;
				*/

				p.set(first.OpenPrice, periodsDaily[i].ClosePrice, MaxPrice, MinPrice, TotalVolume, QDateTime(periodsDaily[i].ExchangeDate));

			}
			else
			{
				/*
				p.TimeStamp = periodsDaily[i-1].CloseTimeStamp;
				p.OpenPrice = first.OpenPrice;
				p.ClosePrice = periodsDaily[i-1].ClosePrice;
				p.MinPrice = MinPrice;
				p.MaxPrice = MaxPrice;
				p.TotalVolume = TotalVolume;
				*/

				p.set(first.OpenPrice, periodsDaily[i-1].ClosePrice, MaxPrice, MinPrice, TotalVolume, QDateTime(periodsDaily[i-1].ExchangeDate));

			}


			first = periodsDaily[i];
			MaxPrice = std::numeric_limits<f32>::min();
			MinPrice = std::numeric_limits<f32>::max();
			TotalVolume = 0;

			out.append(p);
		}
		if(periodsDaily[i].MaxPrice > MaxPrice)
			MaxPrice = periodsDaily[i].MaxPrice;
		if(periodsDaily[i].MinPrice < MinPrice)
			MinPrice = periodsDaily[i].MinPrice;
		TotalVolume += periodsDaily[i].TotalVolume;
	}

}
//-----------------------------------------------------------------------------
static QVector<VizCSecurityPeriod> ajustRage(const QVector<VizCSecurityPeriod>& periodsDaily, const QDateTime& startDate, const QDateTime& endDate)
{
	if(periodsDaily.isEmpty() || startDate.isNull() || endDate.isNull() || endDate > startDate)
	{
		return periodsDaily;
	}
	
	QVector<VizCSecurityPeriod> out;

	foreach(VizCSecurityPeriod p, periodsDaily)
	{
		if(QDateTime(p.ExchangeDate) >= startDate && QDateTime(p.ExchangeDate) <= startDate)
		{
			out.append(p);
		}
	}

	return out;


}
//-----------------------------------------------------------------------------
QVector<VizCTradePeriod> VizCSecurityPeriodsManipulator::convertPeriodicityInteval(const QVector<VizCSecurityPeriod>& periodsDaily, viz::VizESecurityPeriodicity securityPeriodicity, const QDateTime& startDate, const QDateTime& endDate)
{
	QVector<VizCTradePeriod> out;
	QVector<VizCSecurityPeriod> ajustedRange = ajustRage(periodsDaily, startDate, endDate);

	switch(securityPeriodicity)
	{
		case viz::VESP_DAILY:
		{
			convertPeriodicityIntevalToDaily(ajustedRange, out);
			break;
		}
		case viz::VESP_WEEKLY:
		{
			convertPeriodicityIntevalToWeeakly(ajustedRange, out);
			break;
		}
		case viz::VESP_MONTHLY:
		{
			convertPeriodicityIntevalToMonthly(ajustedRange, out);
			break;
		}
		case viz::VESP_QUARTERLY:
		{
			convertPeriodicityIntevalToQuarterly(ajustedRange, out);
			break;
		}
		case viz::VESP_YEARLY:
		{
			convertPeriodicityIntevalToYearly(ajustedRange, out);
			break;
		}
	};
	return out;
}
//-----------------------------------------------------------------------------
QVector<VizCTradePeriod> VizCSecurityPeriodsManipulator::generatePreviewPeriods(const QVector<VizCSecurityPeriod>& periodsDaily)
{
	const s32 Size = 100;

	QVector<VizCTradePeriod> p;

	//implementar
	Q_ASSERT(false);

	return p;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
