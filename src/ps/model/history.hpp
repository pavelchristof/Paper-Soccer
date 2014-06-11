#ifndef PS_HISTORY_HPP
#define PS_HISTORY_HPP

#include "../maybe.hpp"
#include "board.hpp"

#include <QtCore/QObject>

namespace ps
{

class History : public QObject
{
	Q_OBJECT

public:
	/**
	 * Creates an empty history with no focus.
	 */
	History();

	History(const History&) = default;
	History(History&&) = default;
	History& operator =(const History&) = default;
	History& operator =(History&&) = default;

	const std::vector<Board>& boards() const;

	/**
	 * The focused board.
	 */
	Maybe<const Board&> focusedBoard() const;

	/**
	 * The focused entry index.
	 */
	Maybe<size_t> focusedIndex() const;

	/**
	 * Sets the focused entry index.
	 */
	void setFocusedIndex(Maybe<size_t> i);

	/**
	 * Focuses the last entry or None if the history is empty.
	 */
	void focusLast();

	const Board& boardAt(size_t i) const;
	void setBoardAt(size_t i, const Board& board);

	/**
	 * Adds a new board as the last entry.
	 */
	void push(const Board& board);

	/**
	 * Removes the last history entry, if that board was focused sets the 
	 * focus to the previous board or None if there are no more boards.
	 */
	void pop();

	/**
	 * Removes all boards, sets focus to None.
	 */
	void clear();

	/**
	 * Removes all entries after the current focus or if the focus is None removes everything.
	 */
	void clearAfterFocus();

	/**
	 * The number of entries.
	 */
	size_t size() const;

signals:
	void focusChanged();
	void boardModified(size_t i);
	void pushed();
	void popped();

private:
	Maybe<size_t> focus_;
	std::vector<Board> boards_;
};

} // namespace ps

#endif // PS_HISTORY_HPP
