#ifndef PS_MODELS_DIRECTION_HPP
#define PS_MODELS_DIRECTION_HPP

#include "../maybe.hpp"

#include <QtCore/QIntegerForSize>
#include <QtCore/QPoint>
#include <QtCore/QDataStream>

namespace ps {

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

extern Direction directions[8];
Direction opposite(Direction dir);
QPoint dirToPoint(Direction dir);
Maybe<Direction> pointToDir(QPoint point);

QDataStream& operator <<(QDataStream& stream, Direction dir);
QDataStream& operator >>(QDataStream& stream, Direction& dir);

}

#endif // PS_MODELS_DIRECTION_HPP
