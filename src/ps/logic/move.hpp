#ifndef PS_LOGIC_MOVE_HPP
#define PS_LOGIC_MOVE_HPP

#include "point.hpp"
#include "direction.hpp"

#include <vector>

namespace ps {
namespace logic {

/**
 * A sequence of directions. Implements a subset of std::vector interface.
 * 
 * The container is implemented as a bit vector. Each direction uses 3 bits.
 */
class Move
{
public:
	// Constants.
	static const size_t bitsPerElement = 3;

	// Constructor and assignment.

	Move() = default;
	Move(const Move&) = default;
	Move(Move&&) = default;

	Move& operator =(const Move&) = default;
	Move& operator =(Move&&) = default;

	// Capacity.

	size_t size() const;
	void resize(size_t n);
	void resize(size_t n, Direction val);
	bool empty() const;

	// Element access.

	/**
	 * Accesses individual bits with an interface that emulates a reference to a Direction.
	 */
	class Reference
	{
	public:
		operator Direction() const;
		Reference& operator =(Direction dir);
		Reference& operator =(const Reference& ref);

	private:
		Reference(ps::logic::Move& move, int index);

		Move& move;
		int index;

		friend Move;
	};

	Reference operator [](size_t i);
	Direction operator [](size_t i) const;

	Reference front();
	Direction front() const;

	Reference back();
	Direction back() const;

	Direction get(size_t i) const;
	void set(size_t i, Direction val);

	// Modifiers.

	void push_back(Direction val);
	void pop_back();
	void clear();

private:
	std::vector<bool> bits;
};

} // namespace logic
} // namespace ps

#endif // PS_LOGIC_MOVE_HPP
