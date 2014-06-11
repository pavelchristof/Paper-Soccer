#include "history.hpp"

namespace ps {

History::History()
	: focus_(none)
{
}

const std::vector<Board>& History::boards() const
{
	return boards_;
}

Maybe<const Board&> History::focusedBoard() const
{
	return focus_.map<const Board&>([this] (size_t i) -> const Board& { return boards_[i]; });	
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

const Board& History::boardAt(size_t i) const
{
	return boards_[i];
}

void History::setBoardAt(size_t i, const Board& board)
{
	boards_[i] = board;
	emit boardModified(i);
}

void History::push(const Board& board)
{
	boards_.push_back(board);
	emit pushed();
}

void History::pop()
{
	if (boards_.size() == 1) {
		setFocusedIndex(none);
	} else if (focusedIndex() == boards_.size() - 1) {
		setFocusedIndex(boards_.size() - 2);
	}

	boards_.pop_back();
	emit popped();
}

void History::clear()
{
	setFocusedIndex(none);
	while (size() > 0) {
		boards_.pop_back();
		emit popped();
	}
}

void History::clearAfterFocus()
{
	size_t newSize = focus_.mapOr<size_t>([] (size_t i) { return i + 1; }, 0);
	while (size() > newSize) {
		boards_.pop_back();
		emit popped();
	}
}

size_t History::size() const
{
	return boards_.size();
}

} // namespace ps
