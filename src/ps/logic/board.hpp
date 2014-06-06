#ifndef PS_LOGIC_BOARD_HPP
#define PS_LOGIC_BOARD_HPP

#include "point.hpp"
#include "direction.hpp"
#include "edge.hpp"

#include <vector>
#include <QtCore/QIntegerForSize>

namespace ps {
namespace logic {

class Move;

enum class Player : quint8
{
	One = 0,
	Two = 1
};

/**
 * Returns the other player.
 */
Player operator !(Player p);

/**
 * Contains the game state, which consists of:
 *     - board size,
 *     - ball position,
 *     - visited edges,
 *     - current player,
 *     - the winner if the game has finished.
 */
class Board
{
public:
	/**
	 * Initializes an empty board, with the ball on the center.
	 * 
	 * @param width Width of the board. Must be even.
	 * @param height Height of the board, excluding the gates. Must be even.
	 */
	Board(quint8 width, quint8 height);

	quint8 width() const;
	quint8 height() const;
	quint8 halfWidth() const;
	quint8 halfHeight() const;

	/**
	 * Has the game finished?
	 * 
	 * There are two possible win conditions:
	 *     - the ball entered a gate,
	 *     - a player cannot make a move and so he loses the game.
	 */
	bool isFinished() const;

	/**
	 * The player who has to make the next move.
	 */
	Player currentPlayer() const;

	/**
	 * The winner.
	 * @warning Call only when isFinished().
	 */
	Player winner() const;

	/**
	 * @returns the position of the ball.
	 */
	Point ball() const;

	/**
	 * Sets the position of the ball. 
	 * 
	 * The new position must be inside the board.
	 */
	void setBall(Point p);

	/**
	 * Is the point inside the board (board includes the edge and gates).
	 */
	bool isPointInside(Point p) const;

	/**
	 * Is the point outside the board.
	 * 
	 * @see isInside
	 */
	bool isPointOutside(Point p) const;

	/**
	 * Is the edge inside the board (includes the gates).
	 * 
	 * The edge is inside if both its start and end point are inside.
	 */
	bool isEdgeInside(Edge p) const;

	/**
	 * Is the edge outside the board.
	 * 
	 * @see isEdgeInside
	 */
	bool isEdgeOutside(Edge e) const;

	/**
	 * Was the edge visited?
	 */
	bool edge(Edge edge) const;

	/**
	 * Sets whether the edge was visited.
	 */
	void setEdge(Edge edge, bool value) const;

	/**
	 * Checks if the move does not violate any rules.
	 */
	bool isMoveValid(const Move& move) const;

	/**
	 * Checks if the move is valid and the current player cannot extend it.
	 */
	bool isMoveComplete(const Move& move) const;

	/**
	 * Makes a move.
	 * 
	 * The move has to be valid, but does not have to be complete. The current player does not change after an 
	 * incomplete move.
	 */
	void makeMove(const Move& move);

	/**
	 * Reverts a move.
	 * 
	 * For any valid move:
	 * @code
	 * board.makeMove(move);
	 * board.revertMove(move);
	 * @endcode
	 * should have no effect on the board.
	 * 
	 * @see makeMove
	 */
	void revertMove(const Move& move);

private:
	Point ball_;
	Player currentPlayer_;
	quint8 width_, height_;
	std::vector<bool> edges;
};

} // namespace logic
} // namespace ps

#endif // PS_LOGIC_BOARD_HPP
