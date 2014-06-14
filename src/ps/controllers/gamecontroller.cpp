#include "gamecontroller.hpp"
#include "../application.hpp"
#include "../views/gameview.hpp"
#include "../views/historyview.hpp"
#include "../views/boardview.hpp"
#include "../models/history.hpp"

namespace ps {

void GameController::setup(Application* app)
{
}

void GameController::activate(Application* app)
{
	GameView* view = app->gameView();

	view->historyView()->setHistory(app->history());
	view->boardView()->setBoard(app->history()->focusedBoard());

	app->setActiveView(view);
}

void GameController::deactivate(Application* app)
{
	GameView* view = app->gameView();

	view->historyView()->setHistory(nullptr);
	view->boardView()->setBoard(nullptr);
}

} // namespace ps
