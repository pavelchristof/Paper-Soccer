#include "gameconfigcontroller.hpp"
#include "welcomecontroller.hpp"
#include "gamecontroller.hpp"
#include "../application.hpp"
#include "../views/gameconfigview.hpp"
#include "../models/history.hpp"
#include "../models/gameconfig.hpp"

namespace ps {

void GameConfigController::setup(Application* app)
{
	this->app = app;
	this->view = app->gameConfigView();

	connect(view, &GameConfigView::cancelClicked, [app] () {
		app->setActiveController(app->welcomeController());
	});

	connect(view, &GameConfigView::startGameClicked, [app] () {
		app->history()->push(Board(app->gameConfig()->size()));
		app->history()->setFocusedIndex(0);
		app->setActiveController(app->gameController());
	});
}

void GameConfigController::activate()
{
	view->setConfig(app->gameConfig());
	app->setActiveView(view);
	app->newGameAction()->setEnabled(false);
	app->loadGameAction()->setEnabled(true);
	app->saveGameAction()->setEnabled(false);
	app->quitAction()->setEnabled(true);
	app->history()->clear();
}

void GameConfigController::deactivate()
{
	view->setConfig(nullptr);
}

} // namespace ps
