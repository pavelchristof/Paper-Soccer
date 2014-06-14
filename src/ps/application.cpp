#include "application.hpp"
#include "views/mainwindow.hpp"
#include "views/gameview.hpp"
#include "views/welcomeview.hpp"
#include "views/gameconfigview.hpp"
#include "models/recentlysaved.hpp"
#include "models/gameconfig.hpp"
#include "models/history.hpp"
#include "controllers/controller.hpp"
#include "controllers/welcomecontroller.hpp"
#include "controllers/gameconfigcontroller.hpp"
#include "controllers/gamecontroller.hpp"

#include <QtWidgets/QFileDialog>

namespace ps {

Application::Application()
	: mainWindow_(new MainWindow)
	, activeController_(nullptr)
	, welcomeController_(new WelcomeController)
	, gameConfigController_(new GameConfigController)
	, gameController_(new GameController)
	, welcomeView_(new WelcomeView)
	, gameConfigView_(new GameConfigView)
	, gameView_(new GameView)
	, recentlySaved_(new RecentlySaved)
	, gameConfig_(new GameConfig)
	, history_(new History)
{
	// Setup models.
	recentlySaved_->load();

	// Setup views.
	mainWindow()->stackedLayout()->addWidget(welcomeView_);
	mainWindow()->stackedLayout()->addWidget(gameConfigView_);
	mainWindow()->stackedLayout()->addWidget(gameView_);
	mainWindow_->show();

	// Setup controllers.
	welcomeController_->setup(this);
	gameConfigController_->setup(this);
	gameController_->setup(this);

	setActiveController(welcomeController());
}

Application::~Application()
{
	// delete controllers
	setActiveController(nullptr);
	delete welcomeController_;
	delete gameConfigController_;
	delete gameController_;

	// delete models
	recentlySaved_->save();
	delete recentlySaved_;
	delete gameConfig_;
	delete history_;

	delete mainWindow_;
}

Controller* Application::activeController()
{
	return activeController_;
}

void Application::setActiveController(Controller* controller)
{
	if (activeController_ != nullptr) {
		activeController_->deactivate(this);
	}
	activeController_ = controller;
	if (activeController_ != nullptr) {
		activeController_->activate(this);
	}
}

WelcomeController* Application::welcomeController()
{
	return welcomeController_;
}

GameConfigController* Application::gameConfigController()
{
	return gameConfigController_;
}

GameController* Application::gameController()
{
	return gameController_;
}

QWidget* Application::activeView()
{
	return mainWindow()->stackedLayout()->currentWidget();
}

void Application::setActiveView(QWidget* view)
{
	mainWindow()->stackedLayout()->setCurrentWidget(view);
}

MainWindow* Application::mainWindow()
{
	return mainWindow_;
}

WelcomeView* Application::welcomeView()
{
	return welcomeView_;
}

GameConfigView* Application::gameConfigView()
{
	return gameConfigView_;
}

GameView* Application::gameView()
{
	return gameView_;
}

RecentlySaved* Application::recentlySaved()
{
	return recentlySaved_;
}

GameConfig* Application::gameConfig()
{
	return gameConfig_;
}

History* Application::history()
{
	return history_;
}

void Application::newGame()
{
	setActiveController(gameConfigController());
}

void Application::loadGame()
{
	QString fileName = QFileDialog::getOpenFileName(mainWindow(), tr("Load game"), QString(), 
													"Paper Soccer save files (*.pss)");
	if (!fileName.isEmpty()) {
		loadGame(fileName);
	}
}

void Application::loadGame(const QString& fileName)
{
}

void Application::saveGame()
{
	QString fileName = QFileDialog::getSaveFileName(mainWindow(), tr("Save game"), QString(), 
													"Paper Soccer save files (*.pss)");
	if (!fileName.isEmpty()) {
		loadGame(fileName);
	}
}

void Application::saveGame(const QString&)
{

}

} // namespace ps
