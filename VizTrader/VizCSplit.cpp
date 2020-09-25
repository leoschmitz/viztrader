//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCSplit.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace core
{
//-----------------------------------------------------------------------------
VizCSplit::VizCSplit() : SplitId(-1), QuantityBefore(0), QuantityAfter(0), ValueAfter(0.0), ValueBefore(0.0), Date(QDate())
{
}
//-----------------------------------------------------------------------------
VizCSplit::VizCSplit(u64 quantityBefore, u64 quantityAfter, f64 valueAfter, f64 valueBefore, const QDate& date) : QuantityBefore(quantityBefore), QuantityAfter(quantityAfter), ValueAfter(valueAfter), Date(date)
{
}
//-----------------------------------------------------------------------------
VizCSplit::VizCSplit(const VizCSplit& other)
{
	set(other);
}
//-----------------------------------------------------------------------------
VizCSplit::~VizCSplit()
{
}
//-----------------------------------------------------------------------------
u16 VizCSplit::crc() const
{
	QString toChar(QString::number(SplitId));

	toChar.append(QString::number(QuantityBefore));
	toChar.append(QString::number(QuantityAfter));
	toChar.append(QString::number(ValueAfter));
	toChar.append(QString::number(ValueBefore));
	toChar.append(Date.toString(Qt::ISODate));

	return qChecksum(toChar.toLatin1(), toChar.size());
}
//-----------------------------------------------------------------------------
void VizCSplit::set(s32 splitId, u64 quantityBefore, u64 quantityAfter, f64 valueAfter, f64 valueBefore, const QDate& date)
{
	SplitId = splitId;
	QuantityBefore = quantityBefore;
	QuantityAfter = quantityAfter;
	ValueAfter = valueAfter;
	ValueBefore = valueBefore;
	Date = date;
}
//-----------------------------------------------------------------------------
void VizCSplit::get(s32& splitId, u64& quantityBefore, u64& quantityAfter, f64& valueAfter, f64& valueBefore, QDate& date) const
{
	splitId = SplitId;
	quantityBefore = QuantityBefore;
	quantityAfter = QuantityAfter;
	date = Date;
	valueAfter = ValueAfter;
}
//-----------------------------------------------------------------------------
void VizCSplit::set(const VizCSplit& other)
{
	set(other.SplitId, other.QuantityBefore, other.QuantityAfter, other.ValueAfter, other.ValueBefore, other.Date);
}
//-----------------------------------------------------------------------------
void VizCSplit::get(VizCSplit& other) const
{
	get(other.SplitId, other.QuantityBefore, other.QuantityAfter, other.ValueAfter, other.ValueBefore, other.Date);
}
//-----------------------------------------------------------------------------
VizCSplit& VizCSplit::operator=(const VizCSplit& other)
{
	set(other);
	return *this;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
