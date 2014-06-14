#include "welcomecontroller.hpp"
#include "../application.hpp"
#include "../views/mainwindow.hpp"
#include "../views/welcomeview.hpp"
#include "../models/recentlysaved.hpp"

namespace ps {

void WelcomeController::setup(Application* app)
{
	WelcomeView* view = app->welcomeView();

	connect(view->newGameButton(), &QPushButton::clicked, app, &Application::newGame);
	connect(view->loadGameButton(), &QPushButton::clicked, [app] () { app->loadGame(); });
	connect(view->recentlySavedList(), &QListView::activated, [app] (const QModelIndex& index) {
		QString filePath = app->recentlySaved()->data(index, Qt::DisplayRole).toString();
		app->loadGame(filePath);
	});
}

void WelcomeController::activate(Application* app)
{
	app->welcomeView()->recentlySavedList()->setModel(app->recentlySaved());
	app->setActiveView(app->welcomeView());
}

void WelcomeController::deactivate(Application* app)
{
	app->welcomeView()->recentlySavedList()->setModel(nullptr);
}

} // namespace ps
