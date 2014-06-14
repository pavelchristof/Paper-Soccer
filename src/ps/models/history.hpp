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

	/**
	 * Clears the history (calls signals!).
	 */
	virtual ~History();

	History(const History&) = default;
	History(History&&) = default;
	History& operator =(const History&) = default;
	History& operator =(History&&) = default;

	/**
	 * @returns the boards.
	 */
	const std::vector<Board*>& boards() const;

	/**
	 * The focused board or nullptr if there is no focus.
	 */
	// @{
	Board* focusedBoard() ;
	const Board* focusedBoard() const;
	// @}

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

	/**
	 * Returns a pointer to the i-th entry.
	 * 
	 * The returned pointer is stable and valid until the board gets popped.
	 * 
	 * @param i the index, must be valid.
	 */
	// @{
	Board* boardAt(size_t i);
	const Board* boardAt(size_t i) const;
	// @}

	/**
	 * Adds a new board as the last entry.
	 */
	// @{
	void push(const Board& board);
	void push(Board&& board);
	// @}

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
	/**
	 * Called after the focus is changed.
	 */
	void focusChanged();

	/**
	 * Called after a board is pushed.
	 */
	void pushed();

	/**
	 * Called before a board is popped.
	 */
	void popping();

private:
	Maybe<size_t> focus_;
	std::vector<Board*> boards_;
};

} // namespace ps

#endif // PS_HISTORY_HPP
