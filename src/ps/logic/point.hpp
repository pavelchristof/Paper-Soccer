#ifndef PS_LOGIC_POINT_HPP
#define PS_LOGIC_POINT_HPP

#include <QtCore/QIntegerForSize>

namespace ps {
namespace logic {

/**
 * A point on the board. (0, 0) is the board center.
 */
struct Point
{
	// So much optimizations.
	union {
		struct {
			qint8 x, y;
		};
		quint16 packed;
	};

	Point() = default;
	Point(const Point&) = default;
	Point(Point&&) = default;

	Point(qint8 x, qint8 y);
	Point(quint16 packed);

	Point& operator = (const Point&) = default;
	Point& operator = (Point&&) = default;

	bool operator < (const Point& p) const;
	bool operator <= (const Point& p) const;
	bool operator > (const Point& p) const;
	bool operator >= (const Point& p) const;
	bool operator == (const Point& p) const;
	bool operator != (const Point& p) const;
};

} // namespace logic
} // namespace ps

#endif // PS_LOGIC_POINT_HPP
