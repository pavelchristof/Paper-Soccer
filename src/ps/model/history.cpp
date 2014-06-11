#include "history.hpp"

namespace ps {

History::History()
	: focus_(none)
{
}

History::~History()
{
	clear();
}

const std::vector<Board*>& History::boards() const
{
	return boards_;
}

Board* History::focusedBoard()
{
	return focus_.mapOr<Board*>([this] (size_t i) { return boards_[i]; }, nullptr);
}

const Board* History::focusedBoard() const
{
	return focus_.mapOr<const Board*>([this] (size_t i) { return boards_[i]; }, nullptr);
}

Maybe<size_t> History::focusedIndex() const
{
	return focus_;
}

void History::setFocusedIndex(Maybe<size_t> i)
{
	if (focus_ != i) {
		focus_ = i;
		emit focusChanged();
	}
}

void History::focusLast()
{
	if (boards_.size() > 0) {
		setFocusedIndex(boards_.size() - 1);
	} else {
		setFocusedIndex(none);
	}
}

Board* History::boardAt(size_t i)
{
	return boards_[i];
}

const Board* History::boardAt(size_t i) const
{
	return boards_[i];
}

void History::push(const Board& board)
{
	boards_.push_back(new Board{board});
	emit pushed();
}

void History::push(Board&& board)
{
	boards_.push_back(new Board{board});
	emit pushed();
}

void History::pop()
{
	if (boards_.size() == 1) {
		setFocusedIndex(none);
	} else if (focusedIndex() == boards_.size() - 1) {
		setFocusedIndex(boards_.size() - 2);
	}

	emit popping();
	boards_.pop_back();
}

void History::clear()
{
	setFocusedIndex(none);
	while (size() > 0) {
		emit popping();
		boards_.pop_back();
	}
}

void History::clearAfterFocus()
{
	size_t newSize = focus_.mapOr<size_t>([] (size_t i) { return i + 1; }, 0);
	while (size() > newSize) {
		emit popping();
		boards_.pop_back();
	}
}

size_t History::size() const
{
	return boards_.size();
}

} // namespace ps
