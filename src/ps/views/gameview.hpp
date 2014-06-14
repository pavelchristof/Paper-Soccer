#ifndef PS_VIEWS_GAMEVIEW_HPP
#define PS_VIEWS_GAMEVIEW_HPP

#include <QtWidgets/QWidget>

namespace ps
{

class PlayerSwitch;
class HistoryView;
class BoardView;

/**
 * Shows the focused board in the center, some buttons to the left and the history to the right.
 */
class GameView : public QWidget
{
	Q_OBJECT

public:
	explicit GameView(QWidget* parent = nullptr, Qt::WindowFlags f = 0);

	BoardView* boardView();
	const BoardView* boardView() const;

	HistoryView* historyView();
	const HistoryView* historyView() const;

	PlayerSwitch* playerSwitch();
	const PlayerSwitch* playerSwitch() const;

private:
	BoardView* boardView_;
	HistoryView* historyView_;
	PlayerSwitch* playerSwitch_;
};

} // namespace ps

#endif // PS_VIEWS_GAMEVIEW_HPP
