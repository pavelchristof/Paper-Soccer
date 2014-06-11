#include "direction.hpp"

namespace ps {

Direction directions[8] = {NorthWest, North, NorthEast, East, SouthEast, South, SouthWest, West};

Direction opposite(Direction dir)
{
	quint8 asInt = (static_cast<quint8>(dir) + 4) % 8;
	return static_cast<Direction>(asInt);
}

QPoint dirToPoint(Direction dir)
{
	qint8 x;

	switch (dir) {
		case NorthWest:
		case SouthWest:
		case West:
			x = -1;
			break;

		case North:
		case South:
			x = 0;
			break;

		case NorthEast:
		case East:
		case SouthEast:
			x = 1;
			break;

	}

	qint8 y;

	switch (dir) {
		case SouthEast:
		case South:
		case SouthWest:
			y = -1;
			break;

		case East:
		case West:
			y = 0;
			break;

		case NorthWest:
		case North:
		case NorthEast:
			y = 1;
			break;
	}

	return {x, y};
}

}
