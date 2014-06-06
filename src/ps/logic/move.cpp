#include "move.hpp"

#include <QtCore/QDebug>

namespace ps {
namespace logic {

size_t Move::size() const
{
	return bits.size() / bitsPerElement;
}

void Move::resize(size_t n)
{
	bits.resize(n * bitsPerElement);
}

void Move::resize(size_t n, Direction val)
{
	resize(n);
	for (size_t i = 0; i < n; ++i)
		set(i, val);
}

bool Move::empty() const
{
	return bits.empty();
}

Move::Reference::operator Direction() const
{
	return move.get(index);
}

Move::Reference& Move::Reference::operator =(Direction dir)
{
	move.set(index, dir);
	return *this;
}

Move::Reference& Move::Reference::operator=(const Move::Reference& ref)
{
	return *this =(Direction(ref));
}

Move::Reference::Reference(Move& move, int index)
	: move(move)
	, index(index)
{
}

Move::Reference Move::operator[](size_t i)
{
	return Reference(*this, i);
}

Direction Move::operator[](size_t i) const
{
	return get(i);
}

Move::Reference Move::front()
{
	return Reference(*this, 0);
}

Direction Move::front() const
{
	return get(0);
}

Move::Reference Move::back()
{
	return Reference(*this, size() - 1);
}

Direction Move::back() const
{
	return get(size() - 1);
}

static constexpr size_t firstBit(size_t i)
{
	return i * Move::bitsPerElement;
}

static constexpr size_t lastBit(size_t i)
{
	return (i + 1) * Move::bitsPerElement - 1;
}

Direction Move::get(size_t i) const
{
	Q_ASSERT(i < size());
	quint8 encoding = 0;
	for (size_t j = lastBit(i); j >= firstBit(i); --j) {
		encoding = (encoding << 1) + (quint8)bits[j];
	}
	return (Direction)encoding;
}

void Move::set(size_t i, Direction val)
{
	Q_ASSERT(i < size());
	quint8 encoding = (quint8)val;
	for (int j = firstBit(i); j <= lastBit(i); ++j) {
		bits[j] = bool(encoding & 1);
		encoding >>= 1;
	}
}

void Move::push_back(Direction val)
{
	resize(size() + 1);
	set(size() - 1, val);
}

void Move::pop_back()
{
	Q_ASSERT(size() > 0);
	resize(size() - 1);
}

void Move::clear()
{
	bits.clear();
}

} // namespace logic
} // namespace ps
