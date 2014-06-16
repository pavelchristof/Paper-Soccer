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

	// Never reaches here.
	return Player::One;
}

QDataStream& operator<<(QDataStream& stream, Player player)
{
	return stream << static_cast<quint8>(player);
}

QDataStream& operator>>(QDataStream& stream, Player& player)
{
	quint8 asInt;
	stream >> asInt;
	if (asInt > 1) {
		stream.setStatus(QDataStream::ReadCorruptData);
		return stream;
	}
	player = static_cast<Player>(asInt);
	return stream;
}

QDataStream& operator <<(QDataStream& stream, EdgeCategory cat)
{
	return stream << static_cast<quint8>(cat);
}

QDataStream& operator >>(QDataStream& stream, EdgeCategory& cat)
{
	quint8 asInt;
	stream >> asInt;
	if (asInt > 3) {
		stream.setStatus(QDataStream::ReadCorruptData);
		return stream;
	}
	cat = static_cast<EdgeCategory>(asInt);
	return stream;
}

Board::Board()
	: size_(0, 0)
	, ball_(0, 0)
	, shape(intervals())
	, currentPlayer_(Player::One)
{
}

Board::Board(QSize size)
	: size_(size.width(), size.height() + 2) // add the gates
	, ball_(0, 0)
	, shape(intervals())
	, edges(shape.size(), EdgeCategory::Empty)
	, currentPlayer_(Player::One)
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

QVector<QPoint> Board::pointsInside() const
{
	QVector<QPoint> points;
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
	edge.normalize();

	if (!isPointInside(edge.start()) || !isPointInside(edge.end())) {
		return false;
	}

	// The above criterium would be enough if the board was convex.
	// Because it is not convex around the gates there are four special cases.
	Edge specialCases[4] = {
		{{-2, halfHeight() - 1}, NorthEast}, 
		{{2, halfHeight() - 1}, NorthWest}, 
		{{-2, -(halfHeight() - 1)}, SouthEast}, 
		{{2, -(halfHeight() - 1)}, SouthWest}
	};
	for (int i = 0; i < 4; ++i) {
		if (edge == specialCases[i].normalized()) {
			return false;
		}
	}

	return true;
}

bool Board::isEdgeVisited(Edge edge) const
{
	return isEdgeInside(edge) && edgeCategory(edge) != EdgeCategory::Empty;
}

QVector<Edge> Board::edgesInside() const
{
	QVector<Edge> edges;
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

const QVector<Direction>& Board::currentMove() const
{
	return currentMove_;
}

void Board::setCurrentMove(const QVector<Direction>& move)
{
	clearCurrentMove();
	for (Direction dir : move) {
		pushStep(dir);
	}
}

void Board::clearCurrentMove()
{
	while (!currentMove_.empty()) {
		popStep();
	}
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

bool Board::canFinishMove() const
{
	if (currentMove_.empty())
		return false;
	
	if (countVisitedEdgesAround(ball()) > 1)
		return false;
	
	return true;
}

bool Board::canStepInDirection(Direction dir) const
{
	if (canFinishMove()) {
		return false;
	}
	
	Edge edge{ball(), dir};
	return isEdgeInside(edge) && !isEdgeVisited(edge);
}

bool Board::canStepTo(QPoint point) const
{
	QPoint diff = point - ball();
	return pointToDir(diff).mapOr<bool>([this] (Direction dir) { 
		return canStepInDirection(dir);
	}, false);
}

bool Board::canPushSomeStep() const
{
	for (Direction dir : directions) {
		if (canStepInDirection(dir)) {
			return true;
		}
	}
	return false;
}

void Board::convertCurrentMoveToOldEdges()
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
}

void Board::finishMove()
{
	Q_ASSERT(canFinishMove());
	Q_ASSERT(winner().isNone());
	convertCurrentMoveToOldEdges();
	setCurrentPlayer(!currentPlayer());
}

void Board::undoConvertCurrentMoveToOldEdges(QVector<Direction>&& move)
{
	currentMove_ = std::move(move);
	QPoint p = ball();
	for (int i = currentMove_.size() - 1; i >= 0; --i) {
		Direction dir = currentMove_[i];

		Edge edge{p, opposite(dir)};
		Q_ASSERT(isEdgeInside(edge));
		Q_ASSERT(edgeCategory(edge) == EdgeCategory::Old);
		setEdgeCategory(edge, EdgeCategory::New);

		p = edge.end();
	}
}

void Board::undoFinishMove(QVector<Direction>&& move)
{
	undoConvertCurrentMoveToOldEdges(std::move(move));
	setCurrentPlayer(!currentPlayer());
}

bool Board::enumerateMoves(std::function<bool (Board&, const QVector<Direction>&)> callback)
{
	if (winner().isSome()) {
		return callback(*this, currentMove());
	} else if (canFinishMove()) {
		QVector<Direction> move = currentMove();
		finishMove();
		bool cont = callback(*this, move);
		undoFinishMove(std::move(move));
		return cont;
	} else {
		for (Direction dir : directions) {
			if (canStepInDirection(dir)) {
				pushStep(dir);
				bool cont = enumerateMoves(callback);
				popStep();
				if (!cont) {
					return false;
				}
			}
		}
	}
	return true;
}

Maybe<Player> Board::winner() const
{
	if (ball().y() == halfHeight()) {
		return Player::Two;
	} else if (ball().y() == -halfHeight()) {
		return Player::One;
	} if (!canFinishMove() && !canPushSomeStep()) {
		return !currentPlayer();
	} else {
		return none;
	}
}

Shape<int, int, quint8>::IntervalsTuple Board::intervals() const
{
	return std::make_tuple(
		std::make_pair(-(halfWidth() + 1), halfWidth() + 1), 
		std::make_pair(-(halfHeight() + 1), halfHeight() + 1), 
		std::make_pair(0, 3)
	);
}

int Board::edgeIndex(Edge edge) const
{
	edge.normalize();
	return shape.map(std::make_tuple(
		edge.start().x(), 
		edge.start().y(), 
		static_cast<quint8>(edge.direction())
	));
}

QDataStream& operator<<(QDataStream& stream, const Board& board)
{
	return stream << board.size_ << board.ball_ << board.edges << board.currentPlayer_ << board.currentMove_;
}

QDataStream& operator>>(QDataStream& stream, Board& board)
{
	board.edges.clear();
	board.currentMove_.clear();
	stream >> board.size_ >> board.ball_ >> board.edges >> board.currentPlayer_ >> board.currentMove_;
	board.shape.setIntervals(board.intervals());

	// Check data integrity.
	// This doesn't check if currentMove doesn't get out of the board and if edge categories are ok.
	// It should be enough to detect some random data corruption, but a specially crafted save file probably can
	// crash the application. However even if this function was 100% correct it would still be trivially easy to
	// crash the program - QVector is saved like this: <item count><item 0><item 1>...
	// Just set item count to MAX_INT and the program will (probably) run OOM when loading that vector.
	// So this function really isn't critical. QDataStream in itself is very fragile...
	if (board.size().width() < 2 || board.size().height() < 2 ||
		board.size().width() % 2 != 0 || board.size().height() % 2 != 0 ||
		(size_t)board.edges.size() != board.shape.size() ||
		!board.isPointInside(board.ball())
	) {
		stream.setStatus(QDataStream::ReadCorruptData);
		return stream;
	}

	return stream;
}

} // namespace ps
