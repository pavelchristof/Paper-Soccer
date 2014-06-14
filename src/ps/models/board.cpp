#include "board.hpp"

namespace ps {

Player operator!(Player p)
{
	switch (p) {
		case Player::One:
			return Player::Two;

		case Player::Two:
			return Player::One;
	}
	assert(false);
}

Board::Board(QSize size)
	: size_(size)
	, ball_(0, 0)
	, shape(std::make_tuple(
		std::make_pair(-(halfWidth() + 1), halfWidth() + 1), 
		std::make_pair(-(halfHeight() + 1), halfHeight() + 1), 
		std::make_pair(0, 3)))
	, edges(shape.size(), EdgeCategory::Empty)
	, currentPlayer_(Player::One)
	, winner_(none)
{
	Q_ASSERT(width() % 2 == 0);
	Q_ASSERT(height() % 2 == 0);

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
	for (int row = -(hh - 1); row < (hh - 1); ++row) {
		setEdgeCategory({{hw, row}, North}, EdgeCategory::Border);
		setEdgeCategory({{-hw, row}, North}, EdgeCategory::Border);
	}
	for (int col = -hw; col < hw; ++col) {
		setEdgeCategory({{col, hh - 1}, East}, EdgeCategory::Border);
		setEdgeCategory({{col, -(hh - 1)}, East}, EdgeCategory::Border);
	}

	// Make a hole for the gates.
	setEdgeCategory({{0, hh - 1}, East}, EdgeCategory::Empty);
	setEdgeCategory({{0, hh - 1}, West}, EdgeCategory::Empty);
	setEdgeCategory({{0, -(hh - 1)}, East}, EdgeCategory::Empty);
	setEdgeCategory({{0, -(hh - 1)}, West}, EdgeCategory::Empty);

	// Draw the gates.
	setEdgeCategory({{0, hh}, East}, EdgeCategory::Border);
	setEdgeCategory({{0, hh}, West}, EdgeCategory::Border);
	setEdgeCategory({{0, -hh}, East}, EdgeCategory::Border);
	setEdgeCategory({{0, -hh}, West}, EdgeCategory::Border);
	setEdgeCategory({{1, hh}, South}, EdgeCategory::Border);
	setEdgeCategory({{-1, hh}, South}, EdgeCategory::Border);
	setEdgeCategory({{1, -hh}, North}, EdgeCategory::Border);
	setEdgeCategory({{-1, -hh}, North}, EdgeCategory::Border);
}

QSize Board::size() const
{
	return size_;
}

int Board::width() const
{
	return size().width();
}

int Board::height() const
{
	return size().height();
}

int Board::halfWidth() const
{
	return width() / 2;
}

int Board::halfHeight() const
{
	return height() / 2;
}

QPoint Board::ball() const
{
	return ball_;
}

void Board::setBall(QPoint ball)
{
	Q_ASSERT(isPointInside(ball));
	ball_ = ball;
}

bool Board::isPointInside(QPoint point) const
{
	// Outside the rectangle.
	if (abs(point.x()) > halfWidth() || abs(point.y()) > halfHeight())
		return false;
	// On the gates row but not in the gate.
	if (abs(point.x()) > 1 && abs(point.y()) == halfHeight())
		return false;
	return true;
}

std::vector<QPoint> Board::pointsInside() const
{
	std::vector<QPoint> points;
	for (int x = -halfWidth(); x <= halfWidth(); ++x) {
		for (int y = -halfHeight(); y <= halfHeight(); ++y) {
			if (isPointInside({x, y})) {
				points.push_back({x, y});
			}
		}
	}
	return points;
}

int Board::countVisitedEdgesAround(QPoint point) const
{
	int count = 0;
	for (Direction dir : directions) {
		Edge edge{point, dir};
		if (isEdgeVisited(edge)) {
			++count;
		}
	}
	return count;
}

EdgeCategory Board::edgeCategory(Edge edge) const
{
	Q_ASSERT(isEdgeInside(edge));
	return edges[edgeIndex(edge)];
}

void Board::setEdgeCategory(Edge edge, EdgeCategory category)
{
	Q_ASSERT(isEdgeInside(edge));
	edges[edgeIndex(edge)] = category;
}

bool Board::isEdgeInside(Edge edge) const
{
	return isPointInside(edge.start()) && isPointInside(edge.end());
}

bool Board::isEdgeVisited(Edge edge) const
{
	return isEdgeInside(edge) && edgeCategory(edge) != EdgeCategory::Empty;
}

std::vector<Edge> Board::edgesInside() const
{
	std::vector<Edge> edges;
	for (QPoint p : pointsInside()) {
		for (Direction dir : directions) {
			Edge edge{p, dir};
			if (isEdgeInside(edge)) {
				edges.push_back(edge);
			}
		}
	}
	return edges;
}

Player Board::currentPlayer() const
{
	return currentPlayer_;
}

void Board::setCurrentPlayer(Player player)
{
	currentPlayer_ = player;
}

const std::vector<Direction>& Board::currentMove() const
{
	return currentMove_;
}

void Board::setCurrentMove(const std::vector<Direction>& move)
{
	currentMove_ = move;
}

void Board::pushStep(Direction dir)
{
	Q_ASSERT(canStepInDirection(dir));
	Edge edge{ball(), dir};
	setEdgeCategory(edge, EdgeCategory::New);
	currentMove_.push_back(dir);
	setBall(edge.end());
}

void Board::popStep()
{
	Direction dir = currentMove_.back();
	currentMove_.pop_back();

	Edge edge{ball(), opposite(dir)};
	Q_ASSERT(isEdgeInside(edge));
	Q_ASSERT(edgeCategory(edge) == EdgeCategory::New);

	setEdgeCategory(edge, EdgeCategory::Empty);
	setBall(edge.end());
}

void Board::clearCurrentMove()
{
	while (!currentMove_.empty()) {
		popStep();
	}
}

void Board::finishMove()
{
	QPoint p = ball();
	while (!currentMove_.empty()) {
		Direction dir = currentMove_.back();
		currentMove_.pop_back();

		Edge edge{p, opposite(dir)};
		Q_ASSERT(isEdgeInside(edge));
		Q_ASSERT(edgeCategory(edge) == EdgeCategory::New);
		setEdgeCategory(edge, EdgeCategory::Old);

		p = edge.end();
	}

	setCurrentPlayer(!currentPlayer());
}

bool Board::canStepInDirection(Direction dir) const
{
	if (!canPushStep()) {
		return false;
	}

	Edge edge{ball(), dir};
	return isEdgeInside(edge) && !isEdgeVisited(edge);
}

bool Board::canPushStep() const
{
	if (currentMove_.empty())
		return true;

	if (countVisitedEdgesAround(ball()) > 1)
		return true;

	return false;
}

Maybe<Player> Board::winner() const
{
	if (ball().y() == halfHeight() + 1) {
		return Player::Two;
	} else if (ball().y() == -(halfHeight() + 1)) {
		return Player::One;
// 	} if (!isAnyMoveAvailable()) {
// 		return !currentPlayer();
	} else {
		return none;
	}
}

size_t Board::edgeIndex(Edge edge) const
{
	edge.normalize();
	return shape.map(std::make_tuple(
		edge.start().x(), 
		edge.start().y(), 
		static_cast<quint8>(edge.direction())
	));
}

} // namespace ps
