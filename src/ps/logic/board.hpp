#ifndef PS_LOGIC_BOARD_HPP
#define PS_LOGIC_BOARD_HPP

#include "../maybe.hpp"
#include "../shape.hpp"
#include "point.hpp"
#include "direction.hpp"
#include "edge.hpp"

#include <vector>
#include <QtCore/QIntegerForSize>

namespace ps {
namespace logic {

class Move;

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
	Board(uint width, uint height);

	//------------------
	// Board properties.

	/**
	 * The width of the board.
	 */
	uint width() const;

	/**
	 * The height of the board, excluding the gates.
	 */
	uint height() const;

	/**
	 * Half of the width of the board.
	 */
	uint halfWidth() const;

	/**
	 * half of the height of the board, excluding the gates.
	 */
	uint halfHeight() const;

	/**
	 * The player who is currently making a move, or None if the game ended.
	 */
	Maybe<Player> currentPlayer() const;

	/**
	 * The winner if the game has finished.
	 * 
	 * There are two possible win conditions:
	 *     - the ball entered a gate,
	 *     - a player cannot make a move and so he loses the game.
	 */
	Maybe<Player> winner() const;

	/**
	 * @returns the position of the ball.
	 */
	Point ball() const;

	/**
	 * Was the edge visited?
	 */
	bool isEdgeVisited(Edge edge) const;

	//----------------------------
	// Read-only helper functions.

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
	 * Is a move valid?
	 */
	bool isMoveValid(Direction dir) const;

	//------------------------------
	// Low level board manipulation.

	/**
	 * Sets the position of the ball. 
	 * 
	 * The new position must be inside the board.
	 */
	void setBall(Point p);

	/**
	 * Sets the current player.
	 */
	void setCurrentPlayer(Player player);

	/**
	 * Sets whether the edge was visited.
	 */
	void setEdgeVisited(ps::logic::Edge edge, bool value);

	//-------------------------------
	// High level board manipulation.

	void makeMove(const Move& move);

	void revertMove(const Move& move);

	/**
	 * Makes a move in some direction.
	 */
	void makeMove(Direction dir);

	/**
	 * Reverts a move.
	 */
	void revertMove(Direction dir);

	/**
	 * Ends the current player's turn.
	 */
	void endTurn();

private:
	size_t edgeIndex(Edge edge) const;
	void checkGates();
	void checkWinConditions();
	bool isAnyMoveAvailable() const;

	Point ball_;
	Player currentPlayer_;
	Maybe<Player> winner_;
	uint width_, height_;
	Shape<int, int, quint8> shape;
	std::vector<bool> edges;
};

} // namespace logic
} // namespace ps

#endif // PS_LOGIC_BOARD_HPP
