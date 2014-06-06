#include "point.hpp"

namespace ps {
namespace logic {

Point::Point(qint8 x, qint8 y)
	: x(x)
	, y(y)
{
}

Point::Point(quint16 packed)
	: packed(packed)
{
}

bool Point::operator <(const Point& p) const
{
	return packed < p.packed;
}

bool Point::operator <=(const Point& p) const
{
	return packed <= p.packed;
}

bool Point::operator >(const Point& p) const
{
	return packed > p.packed;
}

bool Point::operator >=(const Point& p) const
{
	return packed >= p.packed;
}

bool Point::operator ==(const Point& p) const
{
	return packed == p.packed;
}

bool Point::operator !=(const Point& p) const
{
	return packed != p.packed;
}

} // namespace logic
} // namespace ps
