#ifndef PS_MODEL_EDGE_HPP
#define PS_MODEL_EDGE_HPP

#include "direction.hpp"

namespace ps {

/**
 * An edge on the board, starting in some point and going in some direction.
 */
class Edge
{
public:
	/**
	 * Creates an edge starting in point @a p and going in direction @a dir.
	 */
	Edge(QPoint p, Direction dir);

	Edge(const Edge&) = default;
	Edge& operator =(const Edge&) = default;

	QPoint start() const;
	void setStart(QPoint p);

	QPoint end() const;
	void setEnd(QPoint p);

	Direction direction() const;
	void setDirection(Direction dir);

	/**
	 * Modifies the direction to one of:
	 *     - NorthWest, 
	 *     - North, 
	 *     - NorthEast, 
	 *     - East.
	 * The starting point is adjusted so that the edge is still the same.
	 */
	void normalize();

	/**
	 * Immutable version of normalize.
	 * @see normalize
	 */
	Edge normalized() const;

	/**
	 * Compares two edges are equal (up to normalization).
	 */
	//@{
	bool operator ==(const Edge& edge) const;
	bool operator !=(const Edge& edge) const;
	//@}

private:
	QPoint start_;
	Direction direction_;
};

} // namespace ps

#endif // PS_MODEL_EDGE_HPP
