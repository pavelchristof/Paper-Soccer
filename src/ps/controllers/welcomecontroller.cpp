#include "welcomecontroller.hpp"
#include "../application.hpp"
#include "../views/welcomeview.hpp"
#include "../models/recentlysaved.hpp"

namespace ps {

void WelcomeController::setup(Application* app)
{
	this->app = app;
	this->view = app->welcomeView();

	connect(view->newGameButton(), &QPushButton::clicked, app, &Application::newGame);
	connect(view->loadGameButton(), &QPushButton::clicked, [app] () { app->loadGame(); });
	connect(view->recentlySavedList(), &QListView::activated, [app] (const QModelIndex& index) {
		QString filePath = app->recentlySaved()->data(index, Qt::DisplayRole).toString();
		app->loadGame(filePath);
	});
}

void WelcomeController::activate()
{
	view->recentlySavedList()->setModel(app->recentlySaved());
	app->setActiveView(view);
	app->newGameAction()->setEnabled(true);
	app->loadGameAction()->setEnabled(true);
	app->saveGameAction()->setEnabled(false);
	app->quitAction()->setEnabled(true);
}

void WelcomeController::deactivate()
{
	view->recentlySavedList()->setModel(nullptr);
}

} // namespace ps
