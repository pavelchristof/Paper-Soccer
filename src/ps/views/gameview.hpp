#ifndef PS_VIEWS_GAMEVIEW_HPP
#define PS_VIEWS_GAMEVIEW_HPP

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QCommandLinkButton>

class Ui_GameView;

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
	GameView(QWidget* parent = nullptr, Qt::WindowFlags f = 0);

	BoardView* boardView();
	HistoryView* historyView();
	PlayerSwitch* playerSwitch();
	QWidget* hintBox();
	QPushButton* startHintButton();
	QPushButton* stopHintButton();
	QWidget* aiBox();
	QPushButton* startAiButton();
	QPushButton* stopAiButton();
	QCommandLinkButton* editButton();

private:
	Ui_GameView* ui;
};

} // namespace ps

#endif // PS_VIEWS_GAMEVIEW_HPP
