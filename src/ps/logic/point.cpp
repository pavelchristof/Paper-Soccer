#include "point.hpp"

namespace ps {
namespace logic {

Point::Point(int x, int y)
	: x(x)
	, y(y)
{
}

bool Point::operator <(const Point& p) const
{
	if (x != p.x)
		return x < p.x;
	return y < p.y;
}

bool Point::operator <=(const Point& p) const
{
	if (x != p.x)
		return x <= p.x;
	return y <= p.y;
}

bool Point::operator >(const Point& p) const
{
	if (x != p.x)
		return x > p.x;
	return y > p.y;
}

bool Point::operator >=(const Point& p) const
{
	if (x != p.x)
		return x >= p.x;
	return y >= p.y;
}

bool Point::operator ==(const Point& p) const
{
	return x == p.x && y == p.y;
}

bool Point::operator !=(const Point& p) const
{
	return x != p.x || y != p.y;
}

} // namespace logic
} // namespace ps
