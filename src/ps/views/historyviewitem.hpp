#ifndef PS_VIEWS_HISTORYVIEWITEM_HPP
#define PS_VIEWS_HISTORYVIEWITEM_HPP

#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QStackedLayout>

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
	explicit HistoryViewItem(QWidget* parent = nullptr, Qt::WindowFlags f = nullptr);

	bool isFocused() const;
	void setFocused(bool isFocused);

	BoardView* boardView();
	const BoardView* boardView() const;
	void setBoardView(BoardView* boardView);

protected:
	void paintEvent(QPaintEvent* e) override;

private:
	bool isFocused_;
	QStackedLayout* layout;
	BoardView* boardView_;
};

} // namespace ps

#endif // PS_VIEWS_HISTORYVIEWITEM_HPP
