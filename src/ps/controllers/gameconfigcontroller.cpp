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
	GameConfigView* view = app->gameConfigView();

	connect(view, &GameConfigView::cancelClicked, [app] () {
		app->setActiveController(new WelcomeController);
	});

	connect(view, &GameConfigView::startGameClicked, [app] () {
		app->history()->clear();
		app->history()->push(Board(app->gameConfig()->size()));
		app->history()->setFocusedIndex(0);
		app->setActiveController(app->gameController());
	});
}

void GameConfigController::activate(Application* app)
{
	app->gameConfigView()->setConfig(app->gameConfig());
	app->setActiveView(app->gameConfigView());
}

void GameConfigController::deactivate(Application* app)
{
	app->gameConfigView()->setConfig(nullptr);
}

} // namespace ps
