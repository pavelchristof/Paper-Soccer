#include "direction.hpp"

namespace ps {

Direction directions[8] = {NorthWest, North, NorthEast, East, SouthEast, South, SouthWest, West};
QPoint asPoint[8] = {{-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}};

Direction opposite(Direction dir)
{
	quint8 asInt = (static_cast<quint8>(dir) + 4) % 8;
	return static_cast<Direction>(asInt);
}

QPoint dirToPoint(Direction dir)
{
	return asPoint[static_cast<quint8>(dir)];
}

Maybe<Direction> pointToDir(QPoint point)
{
	for (quint8 i = 0; i < 8; ++i) {
		if (asPoint[i] == point) {
			return static_cast<Direction>(i);
		}
	}
	return none;
}

QDataStream& operator<<(QDataStream& stream, Direction dir)
{
	return stream << static_cast<quint8>(dir);
}

QDataStream& operator>>(QDataStream& stream, Direction& dir)
{
	quint8 asInt;
	stream >> asInt;
	if (asInt > 7) {
		stream.setStatus(QDataStream::ReadCorruptData);
		return stream;
	}
	dir = static_cast<Direction>(asInt);
	return stream;
}

} // namespace ps
