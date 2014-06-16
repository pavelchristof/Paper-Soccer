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

const QVector<Board*>& History::boards() const
{
	return boards_;
}

Board* History::focusedBoard()
{
	return focus_.mapOr<Board*>([this] (int i) { return boards_[i]; }, nullptr);
}

const Board* History::focusedBoard() const
{
	return focus_.mapOr<const Board*>([this] (int i) { return boards_[i]; }, nullptr);
}

Maybe<int> History::focusedIndex() const
{
	return focus_;
}

void History::setFocusedIndex(Maybe<int> i)
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

Board* History::boardAt(int i)
{
	return boards_[i];
}

const Board* History::boardAt(int i) const
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
	int newSize = focus_.mapOr<int>([] (int i) { return i + 1; }, 0);
	while (size() > newSize) {
		emit popping();
		boards_.pop_back();
	}
}

int History::size() const
{
	return boards_.size();
}

QDataStream& operator <<(QDataStream& stream, const History& history)
{
	// Save the focus.
	stream << history.focus_;

	// Save the boards.
	stream << history.boards_.size();
	for (Board* board : history.boards_) {
		stream << *board;
	}

	return stream;
}

QDataStream& operator >>(QDataStream& stream, History& history)
{
	// Load the focus.
	stream >> history.focus_;

	// Load the boards.
	int size;
	stream >> size;
	history.boards_.clear();
	for (int i = 0; i < size; ++i) {
		Board* board = new Board;
		stream >> *board;
		history.boards_.push_back(board);
	}

	return stream;
}

} // namespace ps
