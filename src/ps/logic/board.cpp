#include "board.hpp"

namespace ps {
namespace logic {



Board::Board(quint8 width, quint8 height)
	: ball_(0, 0)
	, currentPlayer_(Player::One)
	, width_(width)
	, height_(height)
{
	Q_ASSERT(width_ % 2 == 0);
	Q_ASSERT(height_ % 2 == 0);
}

bool Board::isFinished() const
{

}

Player Board::winner() const
{

}

quint8 Board::width() const
{
	return width_;
}

quint8 Board::height() const
{
	return height_;
}

quint8 Board::halfWidth() const
{
	return width_ >> 1;
}

quint8 Board::halfHeight() const
{
	return height_ >> 1;
}

Point Board::ball() const
{
	return ball_;
}

void Board::setBall(Point p)
{
	Q_ASSERT(isPointInside(p));
	ball_ = p;
}

bool Board::isPointInside(Point p) const
{
	// Outside the rectangle containing gates.
	if (abs(p.x) >= halfWidth() || abs(p.y) >= halfHeight() + 1)
		return false;
	// On the gates row but not in the gate.
	if (abs(p.x) >= 2 && abs(p.y) == halfHeight() + 1)
		return false;
	return true;
}

bool Board::isPointOutside(Point p) const
{
	return !isPointInside(p);
}

bool Board::isEdgeInside(Edge p) const
{
	return isPointInside(p.start()) && isPointInside(p.end());
}

bool Board::isEdgeOutside(Edge e) const
{
	return !isEdgeInside(e);
}

bool Board::isMoveValid(const Move& move) const
{
	
}

} // namespace logic
} // namespace ps
