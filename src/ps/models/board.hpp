#ifndef PS_MODELS_BOARD_HPP
#define PS_MODELS_BOARD_HPP

#include "../maybe.hpp"
#include "../shape.hpp"
#include "direction.hpp"
#include "edge.hpp"

#include <vector>
#include <QtCore/QPoint>
#include <QtCore/QSize>

namespace ps {

class Move;

enum class Player : qint8
{
	One = 0,
	Two = 1
};

/**
 * Returns the other player.
 */
Player operator !(Player p);

enum class EdgeCategory
{
	/**
	 * The edge is a part of the border.
	 */
	Border,

	/**
	 * The edge was not filled.
	 */
	Empty,

	/**
	 * The edge was visited before this turn.
	 */
	Old,

	/**
	 * The edge was visited during this turn.
	 */
	New
};

/**
 * Contains the game state, which consists of:
 *     - board size,
 *     - ball position,
 *     - current player,
 *     - edge types,
 *     - the current players partial move.
 */
class Board
{
public:
	/**
	 * Initializes an empty board, with the ball on the center.
	 * 
	 * @param size The size of the board including gates. Both width and height must be even.
	 */
	Board(QSize size);

	Board(const Board&) = default;
	Board(Board&&) = default;
	Board& operator =(const Board&) = default;
	Board& operator =(Board&&) = default;

	// Board size.

	QSize size() const;
	int width() const;
	int height() const;
	int halfWidth() const;
	int halfHeight() const;

	// Ball.

	QPoint ball() const;
	void setBall(QPoint ball);

	// Points.

	/**
	 * Is the point inside the board (including the border)?
	 */
	bool isPointInside(QPoint point) const;

	/**
	 * @returns a vector of all points inside the board.
	 */
	std::vector<QPoint> pointsInside() const;

	int countVisitedEdgesAround(QPoint point) const;

	// Edges.

	EdgeCategory edgeCategory(Edge edge) const;
	void setEdgeCategory(Edge edge, EdgeCategory category);

	/**
	 * Is an edge inside the board (including the border)?
	 */
	bool isEdgeInside(Edge edge) const;

	/**
	 * A visited edge is in category Border, Old or New.
	 */
	bool isEdgeVisited(Edge edge) const;

	/**
	 * @returns a vector of all visited edges.
	 */
	std::vector<Edge> edgesInside() const;

	// Current player.

	Player currentPlayer() const;
	void setCurrentPlayer(Player player);

	// Current move.

	const std::vector<Direction>& currentMove() const;
	void setCurrentMove(const std::vector<Direction>& move);

	void pushStep(Direction dir);
	void popStep();
	void clearCurrentMove();
	void finishMove();

	/**
	 * Checks if you can move in this direction.
	 */
	bool canStepInDirection(Direction dir) const;

	/**
	 * Checks if the current player can add another segment to the move.
	 */
	bool canPushStep() const;

	// Winner.

	Maybe<Player> winner() const;

private:
	size_t edgeIndex(Edge edge) const;

	QSize size_;
	QPoint ball_;
	//TODO: replace quint8 with Direction
	Shape<int, int, quint8> shape;
	std::vector<EdgeCategory> edges;
	Player currentPlayer_;
	std::vector<Direction> currentMove_;
	Maybe<Player> winner_;
};

} // namespace ps

#endif // PS_MODELS_BOARD_HPP
