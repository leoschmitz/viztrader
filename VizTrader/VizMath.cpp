//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 Visualitica Soluções em Visualização LTDA. All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizMath.h"
#include <cmath>
#include <limits>
#include <QCryptographicHash>
//-----------------------------------------------------------------------------
namespace viz
{
namespace core
{
//-----------------------------------------------------------------------------
QString md5(const QString& plain)
{
	QCryptographicHash md5(QCryptographicHash::Md5);
	md5.addData(plain.toLatin1());
	QString out(md5.result().toHex());
	return out;
}
//-----------------------------------------------------------------------------
bool intersectsRayWithLineSegment(const QLineF& ray, const QLineF& line, QPointF& point)
{
	QPointF tmp;
	QLineF::IntersectType it = line.intersect(ray, &tmp);

	if(it == QLineF::BoundedIntersection)
	{
		point = tmp;
		return true;
	}
	if(it == QLineF::UnboundedIntersection)
	{
		f64 maxX = core::max(line.p1().x(), line.p2().x());
		f64 minX = core::min(line.p1().x(), line.p2().x());
		f64 maxY = core::max(line.p1().y(), line.p2().y());
		f64 minY = core::min(line.p1().y(), line.p2().y());

		if((tmp.x() > maxX || tmp.x() < minX) || tmp.y() > maxY || tmp.y() < minY)
		{
			return false;
		}

		point = tmp;
		return true;
	}

	return false;
}
//-----------------------------------------------------------------------------
bool intersectsLineWithRect(const QLineF& line, const QRectF& rect)
{

	if(rect.contains(line.p1()) || rect.contains(line.p2()))
	{
		return true;
	}

	QLineF top_line = QLineF(rect.topLeft(), rect.topRight());
	QLineF bottom_line = QLineF(rect.bottomLeft(), rect.bottomRight());
	QLineF left_line = QLineF(rect.topLeft(), rect.bottomLeft());
	QLineF right_line = QLineF(rect.topRight(), rect.bottomRight());

	QPointF temp_point;
	//QLineF line(p1, p2);

	if(QLineF::BoundedIntersection == line.intersect(top_line, &temp_point))
	{
		return true;
	}
	if(QLineF::BoundedIntersection == line.intersect(bottom_line, &temp_point))
	{
		return true;
	}
	if(QLineF::BoundedIntersection == line.intersect(left_line, &temp_point))
	{
		return true;
	}
	if(QLineF::BoundedIntersection == line.intersect(right_line, &temp_point))
	{
		return true;
	}

	return false;

}
//-----------------------------------------------------------------------------
bool intersectsRectWithValidRect(const QRectF& rectangle, const QRectF& validRectangle)
{
	Q_ASSERT(validRectangle.isValid());

	if(rectangle.isValid())
	{
		return rectangle.intersects(validRectangle);
	}

	QPointF p1 = rectangle.topLeft();
	QPointF p2 = p1 + QPointF(rectangle.width(), 0);
	return core::intersectsLineWithRect(QLineF(p1, p2), validRectangle);
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
