#ifndef PS_MODELS_BOARD_HPP
#define PS_MODELS_BOARD_HPP

#include "../maybe.hpp"
#include "../shape.hpp"
#include "direction.hpp"
#include "edge.hpp"

#include <QtCore/QVector>
#include <QtCore/QPoint>
#include <QtCore/QSize>
#include <QtCore/QDataStream>

namespace ps {

class Move;

enum class Player : quint8
{
	One = 0,
	Two = 1
};

QDataStream& operator <<(QDataStream& stream, Player player);
QDataStream& operator >>(QDataStream& stream, Player& player);

/**
 * Returns the other player.
 */
Player operator !(Player p);

enum class EdgeCategory : quint8
{
	/**
	 * The edge is a part of the border.
	 */
	Border = 0,

	/**
	 * The edge was not filled.
	 */
	Empty = 1,

	/**
	 * The edge was visited before this turn.
	 */
	Old = 2,

	/**
	 * The edge was visited during this turn.
	 */
	New = 3
};

QDataStream& operator <<(QDataStream& stream, EdgeCategory cat);
QDataStream& operator >>(QDataStream& stream, EdgeCategory& cat);

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
	 * Initializes an empty board, with no edges at all and size 0x0.
	 */
	Board();

	/**
	 * Initializes an empty board, with the ball on the center.
	 * 
	 * @param size The size of the board excluding gates. Both width and height must be even.
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
	QVector<QPoint> pointsInside() const;

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
	QVector<Edge> edgesInside() const;

	// Current player.

	Player currentPlayer() const;
	void setCurrentPlayer(Player player);

	// Current move.

	const QVector<Direction>& currentMove() const;
	void setCurrentMove(const QVector<Direction>& move);
	void clearCurrentMove();

	void pushStep(Direction dir);
	void popStep();

	/**
	 * The user can finish move if:
	 *     - the move is not empty
	 *     - the ball is not surrounded by more then one edge
	 */
	bool canFinishMove() const;
	bool canStepInDirection(Direction dir) const;
	bool canStepTo(QPoint point) const;
	bool canPushSomeStep() const;

	/**
	 * Converts edges from the current move to old edges and clears the current move.
	 * @returns the old move.
	 */
	QVector<Direction> convertCurrentMoveToOldEdges();
	QVector<Direction> finishMove();

private:
	void undoConvertCurrentMoveToOldEdges(QVector<Direction>&& move);
	void undoFinishMove(QVector<Direction>&& move);

public:

	/**
	 * Generates all possible boards after this move has been finished.
	 * 
	 * The callback gets the board after the move was finished and the move itself.
	 * The callback can break enumeration early by returning false.
	 * The function modifies the board during enumeration, but undos all modifications before it exits.
	 * 
	 * @returns Whether the enumeration completed without interruption.
	 */
	bool enumerateMoves(std::function<bool (Board&, const QVector<Direction>&)> callback);

	/**
	 * Returns the winner, if there is one.
	 * 
	 * This method checks the ball position and whether the current player can 
	 * finish the move or push an additional segment.
	 */
	Maybe<Player> winner() const;

private:
	Shape<int, int, quint8>::IntervalsTuple intervals() const;
	int edgeIndex(Edge edge) const;

	QSize size_;
	QPoint ball_;
	Shape<int, int, quint8> shape;
	QVector<EdgeCategory> edges;
	Player currentPlayer_;
	QVector<Direction> currentMove_;

	friend QDataStream& operator <<(QDataStream& stream, const Board& board);
	friend QDataStream& operator >>(QDataStream& stream, Board& board);
};

QDataStream& operator <<(QDataStream& stream, const Board& board);
QDataStream& operator >>(QDataStream& stream, Board& board);

} // namespace ps

#endif // PS_MODELS_BOARD_HPP
