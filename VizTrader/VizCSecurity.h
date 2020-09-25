//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCSECURITY_H__
#define __VIZCSECURITY_H__
//-----------------------------------------------------------------------------
#include "VizCSecurityPeriod.h"
#include "VizCRealTimePeriod.h"
#include "VizTypes.h"


#include <QByteArray>
#include <QDateTime>
#include <QString>
#include <QVector>
#include <QPair>
#include <QMap>
//-----------------------------------------------------------------------------
namespace viz
{
namespace core
{
//-----------------------------------------------------------------------------
class VizCSecurity
{
public:
	VizCSecurity();
	VizCSecurity
	(
		const QString& isin, const QString& isinObject,
		const QString& symbol, const VizESecurityBDI bdi, const QString& bdiDescription,
		VizEMarketType marketType, const QString& marketDescription, 
		s32 serialNumber, const QString& securityType,
		s32 distributionNumber, const QDate& dueDate, f64 price,
		const QString& optionStyle, s32 currecyType, const QString& currecyTypeDescription,
		const QString& protection, const QDate& negotiableStart, const QDate& negotiableEnd, const QDate& infoUpdated,
		bool negotiable = false,
		const QVector<VizCSecurityPeriod>& securityPeriods = QVector<VizCSecurityPeriod>(),
		const QVector<VizCRealTimePeriod>& realTimePeriods = QVector<VizCRealTimePeriod>()
	);
	VizCSecurity(const VizCSecurity& other);
	void set
	(
		s32 securityId, 
		const QString& isin, const QString& isinObject,
		const QString& symbol, const VizESecurityBDI bdi, const QString& bdiDescription,
		VizEMarketType marketType, const QString& marketDescription, 
		s32 serialNumber, const QString& securityType,
		s32 distributionNumber, const QDate& dueDate, f64 price,
		const QString& optionStyle, s32 currecyType, const QString& currecyTypeDescription,
		const QString& protection, const QDate& negotiableStart, const QDate& negotiableEnd, const QDate& infoUpdated,
		bool negotiable = false,
		const QVector<VizCSecurityPeriod>& securityPeriods = QVector<VizCSecurityPeriod>(),
		const QVector<VizCRealTimePeriod>& realTimePeriods = QVector<VizCRealTimePeriod>()
	);
	void get
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
	) const;

	void set(const VizCSecurity& other);
	void get(VizCSecurity& other) const;

	void mergeAttributes(const VizCSecurity& other);

	VizCSecurity& operator=(const VizCSecurity& other);
	bool operator==(const VizCSecurity& other) const;
	bool operator!=(const VizCSecurity& other) const;
	bool operator<(const VizCSecurity& other) const;
	u16 crc() const;

	
	s32 SecurityId;
	QString ISIN; // International Securities Identifying Number
	QString ISINObject;
	QString Symbol;
	VizESecurityBDI Bdi;
	QString BdiDescription;
	s32 DistributionNumber;
	VizEMarketType MarketType;
	QString MarketDescription;
	s32 SerialNumber;
	QString SecurityType;
	QDate DueDate;
	f64 Price;
	QString OptionStyle;
	s32 CurrecyType;
	QString CurrecyTypeDescription;
	QString Protection;
	bool Negotiable;
	QDate NegotiableStart;
	QDate NegotiableEnd;
	QDate InfoUpdated;

	QVector<VizCSecurityPeriod> SecurityPeriods;
	QVector<VizCRealTimePeriod> RealTimePeriods;
};
//-----------------------------------------------------------------------------
static inline QDataStream& operator>>(QDataStream& in, VizCSecurity& security)
{
	in
		>> security.SecurityId
		>> security.ISIN
		>> security.ISINObject
		>> security.Symbol
		>> security.Bdi
		>> security.BdiDescription
		>> security.MarketType
		>> security.MarketDescription
		>> security.DistributionNumber
		>> security.SerialNumber
		>> security.SecurityType
		>> security.DueDate
		>> security.Price
		>> security.OptionStyle
		>> security.CurrecyType
		>> security.CurrecyTypeDescription
		>> security.Protection
		>> security.Negotiable
		>> security.NegotiableStart
		>> security.NegotiableEnd;

	return in;	
}
//-----------------------------------------------------------------------------
static inline QDataStream& operator<<(QDataStream& out, const VizCSecurity& security)
{	
	out
		<< security.SecurityId
		<< security.ISIN
		<< security.ISINObject
		<< security.Symbol
		<< security.Bdi
		<< security.BdiDescription
		<< security.MarketType
		<< security.MarketDescription
		<< security.DistributionNumber
		<< security.SerialNumber
		<< security.SecurityType
		<< security.DueDate
		<< security.Price
		<< security.OptionStyle
		<< security.CurrecyType
		<< security.CurrecyTypeDescription
		<< security.Protection
		<< security.Negotiable
		<< security.NegotiableStart
		<< security.NegotiableEnd;

	return out;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
