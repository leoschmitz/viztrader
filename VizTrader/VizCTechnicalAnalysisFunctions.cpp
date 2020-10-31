//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCTechnicalAnalysisFunctions.h"
#include <QDebug>
#include "ta_libc.h"
#include "ta_defs.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace core
{
//-----------------------------------------------------------------------------
VizCTechnicalAnalysisFunctions* VizCTechnicalAnalysisFunctions::Instance = 0;
//-----------------------------------------------------------------------------
VizCTechnicalAnalysisFunctions* VizCTechnicalAnalysisFunctions::instance()
{
	//qDebug() << TA_GetVersionString();

	if(!Instance)
	{
		Instance = new VizCTechnicalAnalysisFunctions();
	}
	return Instance;
}
//-----------------------------------------------------------------------------
VizCTechnicalAnalysisFunctions::VizCTechnicalAnalysisFunctions()
{
}
//-----------------------------------------------------------------------------
QVector<VizCTradePeriod> VizCTechnicalAnalysisFunctions::movingAverage(const QVector<VizCTradePeriod>& in, s32 timePeriod, viz::VizEPriceField priceField, viz::VizEMovingAverageMethod movingAverageMethod)
{
	QVector<VizCTradePeriod> out;

	if(in.empty() || (timePeriod >= in.size()))
	{
		return out;
	}
	
	Q_ASSERT(!in.empty());
	Q_ASSERT(timePeriod > 0 && timePeriod < in.size());

	TA_MAType optInMAType;
	switch(movingAverageMethod)
	{
		case viz::VEMAM_SIMPLE:
			optInMAType = TA_MAType_SMA;
			break;
		case viz::VEMAM_EXPONENTIAL:
			optInMAType = TA_MAType_EMA;
			break;
		case viz::VEMAM_WEIGHTED:
			optInMAType = TA_MAType_WMA;
			break;
		case viz::VEMAM_DOUBLE_EXPONENTIAL:
			optInMAType = TA_MAType_DEMA;
			break;
		case viz::VEMAM_TRIPLE_EXPONENTIAL:
			optInMAType = TA_MAType_TEMA;
			break;
		case viz::VEMAM_TRIANGULAR:
			optInMAType = TA_MAType_TRIMA;
			break;
		case viz::VEMAM_KAYFMAN:
			optInMAType = TA_MAType_KAMA;
			break;
		case viz::VEMAM_MESA:
			optInMAType = TA_MAType_MAMA;
			break;
		case viz::VEMAM_T3:
			optInMAType = TA_MAType_T3;
			break;
	};

	s32 size = in.size();
	f64* inPtr = new f64[size];
	f64* outPtr = new f64[size + 1];

	switch(priceField)
	{
		case viz::VEPF_OPEN_PRICE:
		{
			for(s32 i = 0; i < size; i++)
			{
				inPtr[i] = in[i].OpenPrice;
			}
			break;
		}
		case viz::VEPF_CLOSE_PRICE:
		{
			for(s32 i = 0; i < size; i++)
			{
				inPtr[i] = in[i].ClosePrice;
			}
			break;
		}
		case viz::VEPF_MAX_PRICE:
		{
			for(s32 i = 0; i < size; i++)
			{
				inPtr[i] = in[i].MaxPrice;
			}
			break;
		}
		case viz::VEPF_MIN_PRICE:
		{
			for(s32 i = 0; i < size; i++)
			{
				inPtr[i] = in[i].MinPrice;
			}
			break;
		}
	};

	out.reserve(size + 1);
	out.resize(size + 1);

	s32 outBegIdx, outNBElement;
	TA_RetCode ok = TA_MA(0, in.size(), inPtr, timePeriod, optInMAType, &outBegIdx, &outNBElement, outPtr);

	Q_ASSERT(ok == TA_SUCCESS);

	for(s32 i = 0; i < outNBElement - 1; i++)
	{
		VizCTradePeriod ip(outPtr[i], in[i + outBegIdx].TimeStamp);
		out[i] = ip;
	}

	delete [] inPtr;
	delete [] outPtr;

	return out;
}
//-----------------------------------------------------------------------------
QVector<VizCTradePeriod> VizCTechnicalAnalysisFunctions::relativeStrengthIndex(const QVector<VizCTradePeriod>& in, s32 timePeriod, viz::VizEPriceField priceField)
{
	QVector<VizCTradePeriod> out;

	if(in.empty() || (timePeriod >= in.size()))
	{
		return out;
	}


	Q_ASSERT(!in.empty());
	Q_ASSERT(timePeriod > 0 && timePeriod < in.size());

	s32 size = in.size();
	f64* inPtr = new f64[size];
	f64* outPtr = new f64[size + 1];

	switch(priceField)
	{
		case viz::VEPF_OPEN_PRICE:
		{
			for(s32 i = 0; i < size; i++)
			{
				inPtr[i] = in[i].OpenPrice;
			}
			break;
		}
		case viz::VEPF_CLOSE_PRICE:
		{
			for(s32 i = 0; i < size; i++)
			{
				inPtr[i] = in[i].ClosePrice;
			}
			break;
		}
		case viz::VEPF_MAX_PRICE:
		{
			for(s32 i = 0; i < size; i++)
			{
				inPtr[i] = in[i].MaxPrice;
			}
			break;
		}
		case viz::VEPF_MIN_PRICE:
		{
			for(s32 i = 0; i < size; i++)
			{
				inPtr[i] = in[i].MinPrice;
			}
			break;
		}
	};

	out.reserve(size + 1);
	out.resize(size + 1);

	s32 outBegIdx, outNBElement;
	TA_RetCode ok = TA_RSI(0, in.size(), inPtr, timePeriod, &outBegIdx, &outNBElement, outPtr);

	Q_ASSERT(ok == TA_SUCCESS);


	for(s32 i = 0; i < outNBElement - 1; i++)
	{
		VizCTradePeriod ip(outPtr[i], in[i + outBegIdx].TimeStamp);
		out[i] = ip;
	}

	delete [] inPtr;
	delete [] outPtr;

	return out;
}
//-----------------------------------------------------------------------------
QVector<VizCTradePeriod> VizCTechnicalAnalysisFunctions::volume(const QVector<VizCTradePeriod>& in)
{
	Q_ASSERT(!in.empty());


	QVector<VizCTradePeriod> out;
	s32 size = in.size();
	out.reserve(size);
	out.resize(size);

	for(s32 i = 0; i < in.size(); i++)
	{
		const VizCTradePeriod& sp = in[i];
		VizCTradePeriod ip(sp.TotalVolume, sp.TimeStamp);
		out[i] = ip;
	}

	return out;

}
//-----------------------------------------------------------------------------
QVector<VizCTradePeriod> VizCTechnicalAnalysisFunctions::averageDirectionalMovementIndex(const QVector<VizCTradePeriod>& in, s32 timePeriod)
{
	QVector<VizCTradePeriod> out;

	if(in.empty() || (timePeriod >= in.size()))
	{
		return out;
	}


	Q_ASSERT(!in.empty());
	Q_ASSERT(timePeriod > 0 && timePeriod < in.size());

	s32 size = in.size();
	f64* inHighPtr = new f64[size];
	f64* inLowPtr = new f64[size];
	f64* inClosePtr = new f64[size];
	f64* outPtr = new f64[size + 1];

	for(s32 i = 0; i < size; i++)
	{
		inHighPtr[i] = in[i].MaxPrice;
	}
	for(s32 i = 0; i < size; i++)
	{
		inLowPtr[i] = in[i].MinPrice;
	}
	for(s32 i = 0; i < size; i++)
	{
		inClosePtr[i] = in[i].ClosePrice;
	}

	out.reserve(size + 1);
	out.resize(size + 1);

	s32 outBegIdx, outNBElement;
	TA_RetCode ok = TA_ADX(0, size, inHighPtr, inLowPtr, inClosePtr, timePeriod, &outBegIdx, &outNBElement, outPtr);

	Q_ASSERT(ok == TA_SUCCESS);

	for(s32 i = 0; i < outNBElement - 1; i++)
	{
		VizCTradePeriod ip(outPtr[i], in[i + outBegIdx].TimeStamp);
		out[i] = ip;
	}

	delete [] inHighPtr;
	delete [] inLowPtr;
	delete [] inClosePtr;
	delete [] outPtr;

	return out;
}
//-----------------------------------------------------------------------------
QVector<VizCTradePeriod> VizCTechnicalAnalysisFunctions::minusDirectionalIndicator(const QVector<VizCTradePeriod>& in, s32 timePeriod)
{
	QVector<VizCTradePeriod> out;

	if(in.empty() || (timePeriod >= in.size()))
	{
		return out;
	}


	Q_ASSERT(!in.empty());
	Q_ASSERT(timePeriod > 0 && timePeriod < in.size());

	s32 size = in.size();
	f64* inHighPtr = new f64[size];
	f64* inLowPtr = new f64[size];
	f64* inClosePtr = new f64[size];
	f64* outPtr = new f64[size + 1];

	for(s32 i = 0; i < size; i++)
	{
		inHighPtr[i] = in[i].MaxPrice;
	}
	for(s32 i = 0; i < size; i++)
	{
		inLowPtr[i] = in[i].MinPrice;
	}
	for(s32 i = 0; i < size; i++)
	{
		inClosePtr[i] = in[i].ClosePrice;
	}

	out.reserve(size + 1);
	out.resize(size + 1);

	s32 outBegIdx, outNBElement;
	TA_RetCode ok = TA_MINUS_DI(0, size, inHighPtr, inLowPtr, inClosePtr, timePeriod, &outBegIdx, &outNBElement, outPtr);

	Q_ASSERT(ok == TA_SUCCESS);

	for(s32 i = 0; i < outNBElement - 1; i++)
	{
		VizCTradePeriod ip(outPtr[i], in[i + outBegIdx].TimeStamp);
		out[i] = ip;
	}

	delete [] inHighPtr;
	delete [] inLowPtr;
	delete [] inClosePtr;
	delete [] outPtr;

	return out;
}
//-----------------------------------------------------------------------------
QVector<VizCTradePeriod> VizCTechnicalAnalysisFunctions::plusDirectionalIndicator(const QVector<VizCTradePeriod>& in, s32 timePeriod)
{
	QVector<VizCTradePeriod> out;

	if(in.empty() || (timePeriod >= in.size()))
	{
		return out;
	}


	Q_ASSERT(!in.empty());
	Q_ASSERT(timePeriod > 0 && timePeriod < in.size());

	s32 size = in.size();
	f64* inHighPtr = new f64[size];
	f64* inLowPtr = new f64[size];
	f64* inClosePtr = new f64[size];
	f64* outPtr = new f64[size + 1];

	for(s32 i = 0; i < size; i++)
	{
		inHighPtr[i] = in[i].MaxPrice;
	}
	for(s32 i = 0; i < size; i++)
	{
		inLowPtr[i] = in[i].MinPrice;
	}
	for(s32 i = 0; i < size; i++)
	{
		inClosePtr[i] = in[i].ClosePrice;
	}

	out.reserve(size + 1);
	out.resize(size + 1);

	s32 outBegIdx, outNBElement;
	TA_RetCode ok = TA_PLUS_DI(0, size, inHighPtr, inLowPtr, inClosePtr, timePeriod, &outBegIdx, &outNBElement, outPtr);

	Q_ASSERT(ok == TA_SUCCESS);

	for(s32 i = 0; i < outNBElement - 1; i++)
	{
		VizCTradePeriod ip(outPtr[i], in[i + outBegIdx].TimeStamp);
		out[i] = ip;
	}

	delete [] inHighPtr;
	delete [] inLowPtr;
	delete [] inClosePtr;
	delete [] outPtr;

	return out;
}
/*//-----------------------------------------------------------------------------
void VizCTechnicalAnalysisFunctions::bollingerBands(const QVector<VizCTradePeriod>& in, s32 timePeriod, f64 standardDeviationUp, f64 standardDeviationDown, viz::VizEPriceField priceField, viz::VizEMovingAverageMethod movingAverageMethod, QVector<VizCTradePeriod>& upperBand, QVector<VizCTradePeriod>& middleBand, QVector<VizCTradePeriod>& lowerBand)
{
	upperBand.clear();
	middleBand.clear();
	lowerBand.clear();

	if(in.empty() || (timePeriod >= in.size()))
	{
		return;
	}

	Q_ASSERT(!in.empty());
	Q_ASSERT(timePeriod > 0 && timePeriod < in.size());

	TA_MAType optInMAType;
	switch(movingAverageMethod)
	{
		case viz::VEMAM_SIMPLE:
			optInMAType = TA_MAType_SMA;
			break;
		case viz::VEMAM_EXPONENTIAL:
			optInMAType = TA_MAType_EMA;
			break;
		case viz::VEMAM_WEIGHTED:
			optInMAType = TA_MAType_WMA;
			break;
		case viz::VEMAM_DOUBLE_EXPONENTIAL:
			optInMAType = TA_MAType_DEMA;
			break;
		case viz::VEMAM_TRIPLE_EXPONENTIAL:
			optInMAType = TA_MAType_TEMA;
			break;
		case viz::VEMAM_TRIANGULAR:
			optInMAType = TA_MAType_TRIMA;
			break;
		case viz::VEMAM_KAYFMAN:
			optInMAType = TA_MAType_KAMA;
			break;
		case viz::VEMAM_MESA:
			optInMAType = TA_MAType_MAMA;
			break;
		case viz::VEMAM_T3:
			optInMAType = TA_MAType_T3;
			break;
	};

	s32 size = in.size();
	f64* inPtr = new f64[size];
	f64* outRealUpperBand = new f64[size + 1];
	f64* outRealMiddleBand = new f64[size + 1];
	f64* outRealLowerBand = new f64[size + 1];


	switch(priceField)
	{
		case viz::VEPF_OPEN_PRICE:
		{
			for(s32 i = 0; i < size; i++)
			{
				inPtr[i] = in[i].OpenPrice;
			}
			break;
		}
		case viz::VEPF_CLOSE_PRICE:
		{
			for(s32 i = 0; i < size; i++)
			{
				inPtr[i] = in[i].ClosePrice;
			}
			break;
		}
		case viz::VEPF_MAX_PRICE:
		{
			for(s32 i = 0; i < size; i++)
			{
				inPtr[i] = in[i].MaxPrice;
			}
			break;
		}
		case viz::VEPF_MIN_PRICE:
		{
			for(s32 i = 0; i < size; i++)
			{
				inPtr[i] = in[i].MinPrice;
			}
			break;
		}
	};

	s32 outBegIdx, outNBElement;
	TA_RetCode ok = TA_BBANDS(0, in.size(), inPtr, timePeriod, standardDeviationUp, standardDeviationDown, optInMAType, &outBegIdx, &outNBElement, outRealUpperBand, outRealMiddleBand, outRealLowerBand);

	Q_ASSERT(ok == TA_SUCCESS);
	
	for(s32 i = 0; i < outNBElement - 1; i++)
	{
		VizCTradePeriod ipUpper(outRealUpperBand[i], in[i + outBegIdx].TimeStamp);
		upperBand << ipUpper;
		VizCTradePeriod ipMiddle(outRealMiddleBand[i], in[i + outBegIdx].TimeStamp);
		middleBand << ipMiddle;
		VizCTradePeriod ipLower(outRealLowerBand[i], in[i + outBegIdx].TimeStamp);
		lowerBand << ipLower;
	}

	delete [] inPtr;
	delete [] outRealUpperBand;
	delete [] outRealMiddleBand;
	delete [] outRealLowerBand;
}
*/
//-----------------------------------------------------------------------------
QVector<VizCTradePeriod> VizCTechnicalAnalysisFunctions::middleBollingerBand(const QVector<VizCTradePeriod>& in, s32 timePeriod, viz::VizEPriceField priceField, viz::VizEMovingAverageMethod movingAverageMethod)
{
	QVector<VizCTradePeriod> out;

	if(in.empty() || (timePeriod >= in.size()))
	{
		return out;
	}


	Q_ASSERT(!in.empty());
	Q_ASSERT(timePeriod > 0 && timePeriod < in.size());

	TA_MAType optInMAType;
	switch(movingAverageMethod)
	{
		case viz::VEMAM_SIMPLE:
		optInMAType = TA_MAType_SMA;
		break;
		case viz::VEMAM_EXPONENTIAL:
		optInMAType = TA_MAType_EMA;
		break;
		case viz::VEMAM_WEIGHTED:
		optInMAType = TA_MAType_WMA;
		break;
		case viz::VEMAM_DOUBLE_EXPONENTIAL:
		optInMAType = TA_MAType_DEMA;
		break;
		case viz::VEMAM_TRIPLE_EXPONENTIAL:
		optInMAType = TA_MAType_TEMA;
		break;
		case viz::VEMAM_TRIANGULAR:
		optInMAType = TA_MAType_TRIMA;
		break;
		case viz::VEMAM_KAYFMAN:
		optInMAType = TA_MAType_KAMA;
		break;
		case viz::VEMAM_MESA:
		optInMAType = TA_MAType_MAMA;
		break;
		case viz::VEMAM_T3:
		optInMAType = TA_MAType_T3;
		break;
	};

	s32 size = in.size();
	f64* inPtr = new f64[size];
	f64* outRealUpperBand = new f64[size + 1];
	f64* outRealMiddleBand = new f64[size + 1];
	f64* outRealLowerBand = new f64[size + 1];


	switch(priceField)
	{
		case viz::VEPF_OPEN_PRICE:
		{
			for(s32 i = 0; i < size; i++)
			{
				inPtr[i] = in[i].OpenPrice;
			}
			break;
		}
		case viz::VEPF_CLOSE_PRICE:
		{
			for(s32 i = 0; i < size; i++)
			{
				inPtr[i] = in[i].ClosePrice;
			}
			break;
		}
		case viz::VEPF_MAX_PRICE:
		{
			for(s32 i = 0; i < size; i++)
			{
				inPtr[i] = in[i].MaxPrice;
			}
			break;
		}
		case viz::VEPF_MIN_PRICE:
		{
			for(s32 i = 0; i < size; i++)
			{
				inPtr[i] = in[i].MinPrice;
			}
			break;
		}
	};

	out.reserve(size + 1);
	out.resize(size + 1);

	s32 outBegIdx, outNBElement;
	TA_RetCode ok = TA_BBANDS(0, in.size(), inPtr, timePeriod, 0.0, 0.0, optInMAType, &outBegIdx, &outNBElement, outRealUpperBand, outRealMiddleBand, outRealLowerBand);

	Q_ASSERT(ok == TA_SUCCESS);

	for(s32 i = 0; i < outNBElement - 1; i++)
	{
		VizCTradePeriod ipMiddle(outRealMiddleBand[i], in[i + outBegIdx].TimeStamp);
		out[i] = ipMiddle;
	}

	delete [] inPtr;
	delete [] outRealUpperBand;
	delete [] outRealMiddleBand;
	delete [] outRealLowerBand;

	return out;
}
//-----------------------------------------------------------------------------
QVector<VizCTradePeriod> VizCTechnicalAnalysisFunctions::upperBollingerBand(const QVector<VizCTradePeriod>& in, s32 timePeriod, f64 standardDeviation, viz::VizEPriceField priceField, viz::VizEMovingAverageMethod movingAverageMethod)
{
	QVector<VizCTradePeriod> out;

	if(in.empty() || (timePeriod >= in.size()))
	{
		return out;
	}


	Q_ASSERT(!in.empty());
	Q_ASSERT(timePeriod > 0 && timePeriod < in.size());

	TA_MAType optInMAType;
	switch(movingAverageMethod)
	{
		case viz::VEMAM_SIMPLE:
		optInMAType = TA_MAType_SMA;
		break;
		case viz::VEMAM_EXPONENTIAL:
		optInMAType = TA_MAType_EMA;
		break;
		case viz::VEMAM_WEIGHTED:
		optInMAType = TA_MAType_WMA;
		break;
		case viz::VEMAM_DOUBLE_EXPONENTIAL:
		optInMAType = TA_MAType_DEMA;
		break;
		case viz::VEMAM_TRIPLE_EXPONENTIAL:
		optInMAType = TA_MAType_TEMA;
		break;
		case viz::VEMAM_TRIANGULAR:
		optInMAType = TA_MAType_TRIMA;
		break;
		case viz::VEMAM_KAYFMAN:
		optInMAType = TA_MAType_KAMA;
		break;
		case viz::VEMAM_MESA:
		optInMAType = TA_MAType_MAMA;
		break;
		case viz::VEMAM_T3:
		optInMAType = TA_MAType_T3;
		break;
	};

	s32 size = in.size();
	f64* inPtr = new f64[size];
	f64* outRealUpperBand = new f64[size + 1];
	f64* outRealMiddleBand = new f64[size + 1];
	f64* outRealLowerBand = new f64[size + 1];


	switch(priceField)
	{
		case viz::VEPF_OPEN_PRICE:
		{
			for(s32 i = 0; i < size; i++)
			{
				inPtr[i] = in[i].OpenPrice;
			}
			break;
		}
		case viz::VEPF_CLOSE_PRICE:
		{
			for(s32 i = 0; i < size; i++)
			{
				inPtr[i] = in[i].ClosePrice;
			}
			break;
		}
		case viz::VEPF_MAX_PRICE:
		{
			for(s32 i = 0; i < size; i++)
			{
				inPtr[i] = in[i].MaxPrice;
			}
			break;
		}
		case viz::VEPF_MIN_PRICE:
		{
			for(s32 i = 0; i < size; i++)
			{
				inPtr[i] = in[i].MinPrice;
			}
			break;
		}
	};

	out.reserve(size + 1);
	out.resize(size + 1);

	s32 outBegIdx, outNBElement;
	TA_RetCode ok = TA_BBANDS(0, in.size(), inPtr, timePeriod, standardDeviation, 0.0, optInMAType, &outBegIdx, &outNBElement, outRealUpperBand, outRealMiddleBand, outRealLowerBand);

	Q_ASSERT(ok == TA_SUCCESS);

	for(s32 i = 0; i < outNBElement - 1; i++)
	{
		VizCTradePeriod ipUp(outRealUpperBand[i], in[i + outBegIdx].TimeStamp);
		out[i] = ipUp;
	}

	delete [] inPtr;
	delete [] outRealUpperBand;
	delete [] outRealMiddleBand;
	delete [] outRealLowerBand;

	return out;
}
//-----------------------------------------------------------------------------
QVector<VizCTradePeriod> VizCTechnicalAnalysisFunctions::lowerBollingerBand(const QVector<VizCTradePeriod>& in, s32 timePeriod, f64 standardDeviation, viz::VizEPriceField priceField, viz::VizEMovingAverageMethod movingAverageMethod)
{
	QVector<VizCTradePeriod> out;

	if(in.empty() || (timePeriod >= in.size()))
	{
		return out;
	}


	Q_ASSERT(!in.empty());
	Q_ASSERT(timePeriod > 0 && timePeriod < in.size());

	TA_MAType optInMAType;
	switch(movingAverageMethod)
	{
		case viz::VEMAM_SIMPLE:
		optInMAType = TA_MAType_SMA;
		break;
		case viz::VEMAM_EXPONENTIAL:
		optInMAType = TA_MAType_EMA;
		break;
		case viz::VEMAM_WEIGHTED:
		optInMAType = TA_MAType_WMA;
		break;
		case viz::VEMAM_DOUBLE_EXPONENTIAL:
		optInMAType = TA_MAType_DEMA;
		break;
		case viz::VEMAM_TRIPLE_EXPONENTIAL:
		optInMAType = TA_MAType_TEMA;
		break;
		case viz::VEMAM_TRIANGULAR:
		optInMAType = TA_MAType_TRIMA;
		break;
		case viz::VEMAM_KAYFMAN:
		optInMAType = TA_MAType_KAMA;
		break;
		case viz::VEMAM_MESA:
		optInMAType = TA_MAType_MAMA;
		break;
		case viz::VEMAM_T3:
		optInMAType = TA_MAType_T3;
		break;
	};

	s32 size = in.size();
	f64* inPtr = new f64[size];
	f64* outRealUpperBand = new f64[size + 1];
	f64* outRealMiddleBand = new f64[size + 1];
	f64* outRealLowerBand = new f64[size + 1];


	switch(priceField)
	{
		case viz::VEPF_OPEN_PRICE:
		{
			for(s32 i = 0; i < size; i++)
			{
				inPtr[i] = in[i].OpenPrice;
			}
			break;
		}
		case viz::VEPF_CLOSE_PRICE:
		{
			for(s32 i = 0; i < size; i++)
			{
				inPtr[i] = in[i].ClosePrice;
			}
			break;
		}
		case viz::VEPF_MAX_PRICE:
		{
			for(s32 i = 0; i < size; i++)
			{
				inPtr[i] = in[i].MaxPrice;
			}
			break;
		}
		case viz::VEPF_MIN_PRICE:
		{
			for(s32 i = 0; i < size; i++)
			{
				inPtr[i] = in[i].MinPrice;
			}
			break;
		}
	};

	out.reserve(size + 1);
	out.resize(size + 1);

	s32 outBegIdx, outNBElement;
	TA_RetCode ok = TA_BBANDS(0, in.size(), inPtr, timePeriod, 0.0, standardDeviation, optInMAType, &outBegIdx, &outNBElement, outRealUpperBand, outRealMiddleBand, outRealLowerBand);

	Q_ASSERT(ok == TA_SUCCESS);

	for(s32 i = 0; i < outNBElement - 1; i++)
	{
		VizCTradePeriod ipLower(outRealLowerBand[i], in[i + outBegIdx].TimeStamp);
		out[i] = ipLower;
	}

	delete [] inPtr;
	delete [] outRealUpperBand;
	delete [] outRealMiddleBand;
	delete [] outRealLowerBand;

	return out;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
