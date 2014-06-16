#ifndef PS_VIEWS_HISTORYVIEWITEM_HPP
#define PS_VIEWS_HISTORYVIEWITEM_HPP

#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QLabel>

namespace ps
{

class HistoryView;
class BoardView;

/**
 * A history item, displays a miniature board.
 * 
 * Inherits QAbstractButton for easy clicked() signal.
 */
class HistoryViewItem : public QAbstractButton
{
	Q_OBJECT

public:
	/**
	 * Creates a not focused history item with an empty board view.
	 */
	HistoryViewItem(QWidget* parent = nullptr, Qt::WindowFlags f = 0);

	bool isFocused() const;
	void setFocused(bool isFocused);

	BoardView* boardView();
	const BoardView* boardView() const;

protected:
	void paintEvent(QPaintEvent* e) override;

private:
	bool isFocused_;
	BoardView* boardView_;
	QLabel* label;
};

} // namespace ps

#endif // PS_VIEWS_HISTORYVIEWITEM_HPP
