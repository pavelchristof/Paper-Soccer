#ifndef PS_VIEWS_GAMEVIEW_HPP
#define PS_VIEWS_GAMEVIEW_HPP

#include <QtWidgets/QWidget>

namespace ps
{

class HistoryView;
class BoardView;

/**
 * Shows the focused board in the center, some buttons to the left and the history to the right.
 */
class GameView : public QWidget
{
public:
	explicit GameView(QWidget* parent = nullptr, Qt::WindowFlags f = nullptr);

	BoardView* boardView();
	const BoardView* boardView() const;

	HistoryView* historyView();
	const HistoryView* historyView() const;

private:
	BoardView* boardView_;
	HistoryView* historyView_;
};

} // namespace ps

#endif // PS_VIEWS_GAMEVIEW_HPP
