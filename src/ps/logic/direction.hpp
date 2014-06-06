#ifndef PS_LOGIC_DIRECTION_HPP
#define PS_LOGIC_DIRECTION_HPP

#include <QtCore/QIntegerForSize>
#include "point.hpp"

namespace ps {
namespace logic {

enum Direction : quint8 {
	NorthWest = 0,
	North = 1,
	NorthEast = 2,
	East = 3,
	SouthEast = 4,
	South = 5,
	SouthWest = 6,
	West = 7
};

// It would be nice if enums could have methods.

Direction opposite(Direction dir);
Point dirToPoint(Direction dir);

}
}

#endif // PS_LOGIC_DIRECTION_HPP
