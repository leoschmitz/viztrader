//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZMATH_H__
#define __VIZMATH_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include <QPoint>
#include <QLine>
#include <QRect>
#include <cmath>
#include <limits>
//-----------------------------------------------------------------------------
namespace viz
{
namespace core
{
//-----------------------------------------------------------------------------
const f64 ROUNDING_ERROR_32 = 0.000001f;
const f64 ROUNDING_ERROR_64 = 0.00000001f;
#ifdef PI
#undef PI
#endif
const f32 PI                = 3.14159265359f;
const f32 RECIPROCAL_PI     = 1.0f/PI;
const f32 HALF_PI           = PI/2.0f;
#ifdef PI64
#undef PI64
#endif
const f64 PI64              = 3.1415926535897932384626433832795028841971693993751;
const f64 RECIPROCAL_PI64   = 1.0/PI64;
const f32 DEGTORAD          = PI / 180.0f;
const f32 RADTODEG          = 180.0f / PI;
const f64 DEGTORAD64        = PI64 / 180.0;
const f64 RADTODEG64        = 180.0 / PI64;
//-----------------------------------------------------------------------------
inline f32 radToDeg(f32 radians)
{
	return RADTODEG * radians;
}
//-----------------------------------------------------------------------------
inline f64 radToDeg(f64 radians)
{
	return RADTODEG64 * radians;
}
//-----------------------------------------------------------------------------
inline f32 degToRad(f32 degrees)
{
	return DEGTORAD * degrees;
}
//-----------------------------------------------------------------------------
inline f64 degToRad(f64 degrees)
{
	return DEGTORAD64 * degrees;
}
//-----------------------------------------------------------------------------
inline bool equals(const f64 a, const f64 b, const f64 tolerance = ROUNDING_ERROR_64)
{
	return (a + tolerance >= b) && (a - tolerance <= b);
}
//-----------------------------------------------------------------------------
inline const f64& min(const f64& a, const f64& b)
{
	return a < b ? a : b;
}
//-----------------------------------------------------------------------------
inline const f64& min(const f64& a, const f64& b, const f64& c)
{
	return a < b ? min(a, c) : min(b, c);
}
//-----------------------------------------------------------------------------
inline const f64& max(const f64& a, const f64& b)
{
	return a < b ? b : a;
}
//-----------------------------------------------------------------------------
inline const f64& max(const f64& a, const f64& b, const f64& c)
{
	return a < b ? max(b, c) : max(a, c);
}
//-----------------------------------------------------------------------------
inline long long clampLog10ll(const long long x)
{
	long double d = static_cast<long double>(x);
	//d = (d > 1.0) ? log10(d) : 0.0;
	d = (d > 0.0000001) ? log10(d) : 0.0;
	return static_cast<long long>(d);
} 
//-----------------------------------------------------------------------------
inline f64 clampLog10f(const f64 x)
{
	//return (x > 1.0) ? log10(x) : 0.0;
	return (x > 0.0000001) ? log10(x) : 0.0;
}
//-----------------------------------------------------------------------------
inline f64 pow10f(const f64 x)
{
	return pow(10.0, x);
}
//-----------------------------------------------------------------------------
inline f64 fabs(const f64 x)
{
	return abs(x);
}
//-----------------------------------------------------------------------------
inline f64 isInRange(const f64 x, const f64 a, const f64 b)
{
	return x < a ? false : (x > b ? false : true);
}
//-----------------------------------------------------------------------------
inline f64 clampInRange(const f64 x, const f64 a, const f64 b)
{
	return x < a ? a : (x > b ? b : x);
}
//-----------------------------------------------------------------------------
QString md5(const QString& plain);
//-----------------------------------------------------------------------------
bool intersectsRayWithLineSegment(const QLineF& ray, const QLineF& line, QPointF& point);
//-----------------------------------------------------------------------------
bool intersectsLineWithRect(const QLineF& line, const QRectF& rect);
//-----------------------------------------------------------------------------
bool intersectsRectWithValidRect(const QRectF& rectangle, const QRectF& validRectangle);
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
