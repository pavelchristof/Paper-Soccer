#include "board.hpp"

namespace ps {
namespace logic {

Board::Board(uint width, uint height)
	: ball_(0, 0)
	, currentPlayer_(Player::One)
	, winner_(None{})
	, width_(width)
	, height_(height)
	, shape(std::make_tuple(
		std::make_pair(-(halfWidth() + 1), halfWidth() + 1), 
		std::make_pair(-(halfHeight() + 2), halfHeight() + 2), 
		std::make_pair(0, 3)
	  ))
	, edges(shape.size(), false)
{
	Q_ASSERT(width_ % 2 == 0);
	Q_ASSERT(height_ % 2 == 0);

	// 012.....halfWidth()
	// -
	//  |______  <- halfHeight()
	//         |
	//         |
	//         |
	//         |

	int hw = halfWidth();
	int hh = halfHeight();

	// Draw a rectangle.
	for (int row = -hh; row < hh; ++row) {
		setEdgeVisited({{hw, row}, North}, true);
		setEdgeVisited({{-hw, row}, North}, true);
	}
	for (int col = -hw; col < hw; ++col) {
		setEdgeVisited({{col, hh}, East}, true);
		setEdgeVisited({{col, -hh}, East}, true);
	}

	// Make a hole for the gates.
	setEdgeVisited({{0, hh}, East}, false);
	setEdgeVisited({{0, hh}, West}, false);
	setEdgeVisited({{0, -hh}, East}, false);
	setEdgeVisited({{0, -hh}, West}, false);

	// Draw the gates.
	setEdgeVisited({{0, hh + 1}, East}, true);
	setEdgeVisited({{0, hh + 1}, West}, true);
	setEdgeVisited({{0, -(hh + 1)}, East}, true);
	setEdgeVisited({{0, -(hh + 1)}, West}, true);
	setEdgeVisited({{1, hh}, North}, true);
	setEdgeVisited({{-1, hh}, North}, true);
	setEdgeVisited({{1, -hh}, South}, true);
	setEdgeVisited({{-1, -hh}, South}, true);
}

uint Board::width() const
{
	return width_;
}

uint Board::height() const
{
	return height_;
}

uint Board::halfWidth() const
{
	return width_ >> 1;
}

uint Board::halfHeight() const
{
	return height_ >> 1;
}

Maybe<Player> Board::currentPlayer() const
{
	if (winner_.isSome()) {
		return None{};
	} else {
		return currentPlayer_;
	}
}

Maybe<Player> Board::winner() const
{
	return winner_;
}

Point Board::ball() const
{
	return ball_;
}

bool Board::isEdgeVisited(Edge edge) const
{
	return edges[edgeIndex(edge)];
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

bool Board::isMoveValid(Direction dir) const
{
	Edge edge{ball(), dir};
	return isEdgeInside(edge) && !isEdgeVisited(edge);
}

void Board::setBall(Point p)
{
	Q_ASSERT(isPointInside(p));
	ball_ = p;
}

void Board::setCurrentPlayer(Player player)
{
	currentPlayer_ = player;
}

void Board::setEdgeVisited(Edge edge, bool value)
{
	edges[edgeIndex(edge)] = value;
}

void Board::makeMove(Direction dir)
{
	Edge edge = {ball(), dir};

	Q_ASSERT(isMoveValid(dir));
	Q_ASSERT(isEdgeInside(edge));
	Q_ASSERT(!isEdgeVisited(edge));
	
	setEdgeVisited(edge, true);
	setBall(edge.end());
	checkGates();
}

void Board::revertMove(Direction dir)
{

}

void Board::endTurn()
{

}

size_t Board::edgeIndex(Edge edge) const
{
	edge.normalize();
	return shape.map(std::make_tuple(
		edge.start().x, 
		edge.start().y, 
		static_cast<quint8>(edge.direction())
	));
}

void Board::checkGates()
{
	if (ball().y == halfHeight() + 1) {
		winner_ = Player::Two;
	} else if (ball().y == -(halfHeight() + 1)) {
		winner_ = Player::One;
	}
}

void Board::checkWinConditions()
{
	if (!isAnyMoveAvailable()) {
		// checkWinConditions happens after a move, so we have 
		winner_ = !currentPlayer();
	}
}

bool Board::isAnyMoveAvailable() const
{
	for (Direction dir : directions()) {
		if (isMoveValid(dir)) {
			return true;
		}
	}
	return false;
}

} // namespace logic
} // namespace ps
