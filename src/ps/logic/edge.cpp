#include "edge.hpp"

namespace ps {
namespace logic {

Edge::Edge(Point p, Direction dir)
	: start_(p)
	, direction_(dir)
{
}

Point Edge::start() const
{
	return start_;
}

void Edge::setStart(Point p)
{
	start_ = p;
}

Point Edge::end() const
{
	Point end = dirToPoint(direction());
	end.x += start_.x;
	end.y += start_.y;
	return end;
}

void Edge::setEnd(Point p)
{
	Point start = dirToPoint(opposite(direction()));
	start.x += p.x;
	start.y += p.y;
	setStart(start);
}

Direction Edge::direction() const
{
	return direction_;
}

void Edge::setDirection(Direction dir)
{
	direction_ = dir;
}

void Edge::normalize()
{
	quint8 dirAsInt = static_cast<quint8>(direction());
	if (dirAsInt >= 4) {
		setStart(end());
		setDirection(opposite(direction()));
	}
}

Edge Edge::normalized() const
{
	Edge edge{*this};
	edge.normalize();
	return edge;
}

bool Edge::operator==(const Edge& edge) const
{
	Edge a{normalized()};
	Edge b{edge.normalized()};
	return a.start() == b.start() && a.direction() == b.direction();
}

bool Edge::operator!=(const Edge& edge) const
{
	return !(*this == edge);
}

} // namespace logic
} // namespace ps
