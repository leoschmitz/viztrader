//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCSPLIT_H__
#define __VIZCSPLIT_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include <QDataStream>
#include <QDateTime>
//-----------------------------------------------------------------------------
namespace viz
{
namespace core
{
//-----------------------------------------------------------------------------
class VizCSplit
{
public:
	VizCSplit();
	VizCSplit(u64 quantityBefore, u64 quantityAfter, f64 valueAfter, f64 valueBefore, const QDate& date);
	VizCSplit(const VizCSplit& other);
	~VizCSplit();

	u16 crc() const;
	void set(s32 splitId, u64 quantityBefore, u64 quantityAfter, f64 valueAfter, f64 valueBefore, const QDate& date);
	void get(s32& splitId, u64& quantityBefore, u64& quantityAfter, f64& valueAfter, f64& valueBefore, QDate& date) const;
	void set(const VizCSplit& other);
	void get(VizCSplit& other) const;
	VizCSplit& operator=(const VizCSplit& other);

	s32 SplitId;
	u64 QuantityBefore;
	u64 QuantityAfter;
	f64 ValueAfter;
	f64 ValueBefore;
	QDate Date;
};
//-----------------------------------------------------------------------------
static inline QDataStream& operator>>(QDataStream& in, VizCSplit& split)
{
	in
		>> split.QuantityBefore
		>> split.QuantityAfter
		>> split.ValueAfter
		>> split.ValueBefore
		>> split.Date;
	return in;	
}
//-----------------------------------------------------------------------------
static inline QDataStream& operator<<(QDataStream& out, const VizCSplit& split)
{
	out
		<< split.QuantityBefore
		<< split.QuantityAfter
		<< split.ValueAfter
		<< split.ValueBefore
		<< split.Date;
	return out;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
