#include "application.hpp"
#include "mainwindow.hpp"
#include "views/gameview.hpp"
#include "views/welcomeview.hpp"
#include "views/gameconfigview.hpp"
#include "views/editorview.hpp"
#include "models/recentlysaved.hpp"
#include "models/gameconfig.hpp"
#include "models/history.hpp"
#include "controllers/controller.hpp"
#include "controllers/welcomecontroller.hpp"
#include "controllers/gameconfigcontroller.hpp"
#include "controllers/gamecontroller.hpp"
#include "controllers/editorcontroller.hpp"

#include <QtWidgets/QFileDialog>
#include <QMessageBox>

namespace ps {

Application::Application()
	: activeController_(nullptr)
	, welcomeController_(new WelcomeController)
	, gameConfigController_(new GameConfigController)
	, gameController_(new GameController)
	, editorController_(new EditorController)
	, welcomeView_(new WelcomeView)
	, gameConfigView_(new GameConfigView)
	, gameView_(new GameView)
	, editorView_(new EditorView)
	, recentlySaved_(new RecentlySaved)
	, gameConfig_(new GameConfig)
	, history_(new History)
{
	// Board must be registered because it used in a queued connection.
	qRegisterMetaType<Board>("Board");

	// Setup actions.
	QStyle* style = QApplication::style();

	newGameAction_ = new QAction(style->standardIcon(QStyle::SP_FileIcon), tr("New game..."), this);
	newGameAction_->setShortcut(QKeySequence::New);
	newGameAction_->setStatusTip(tr("Create a new game"));
	connect(newGameAction_, &QAction::triggered, this, &Application::newGame);

	loadGameAction_ = new QAction(style->standardIcon(QStyle::SP_DialogOpenButton), tr("Load game..."), this);
	loadGameAction_->setShortcut(QKeySequence::Open);
	loadGameAction_->setStatusTip(tr("Load a saved game"));
	connect(loadGameAction_, &QAction::triggered, [this] () { loadGame(); });

	saveGameAction_ = new QAction(style->standardIcon(QStyle::SP_DialogSaveButton), tr("Save game..."), this);
	saveGameAction_->setShortcut(QKeySequence::Save);
	saveGameAction_->setStatusTip(tr("Save the game"));
	connect(saveGameAction_, &QAction::triggered, [this] () { saveGame(); });

	quitAction_ = new QAction(style->standardIcon(QStyle::SP_TitleBarCloseButton), tr("Quit"), this);
	quitAction_->setShortcut(QKeySequence::Quit);
	quitAction_->setStatusTip(tr("Quit the program"));
	connect(quitAction_, &QAction::triggered, this, &Application::quit);

	// Create the main window.
	mainWindow_ = new MainWindow(this);

	// Setup models.
	recentlySaved_->load();

	// Setup views.
	mainWindow()->stackedLayout()->addWidget(welcomeView_);
	mainWindow()->stackedLayout()->addWidget(gameConfigView_);
	mainWindow()->stackedLayout()->addWidget(gameView_);
	mainWindow()->stackedLayout()->addWidget(editorView_);
	mainWindow_->show();

	// Setup controllers.
	welcomeController_->setup(this);
	gameConfigController_->setup(this);
	gameController_->setup(this);
	editorController_->setup(this);

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
		activeController_->deactivate();
	}
	activeController_ = controller;
	if (activeController_ != nullptr) {
		activeController_->activate();
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

EditorController* Application::editorController()
{
	return editorController_;
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

EditorView* Application::editorView()
{
	return editorView_;
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
	if (confirm()) {
		setActiveController(gameConfigController());
	}
}

void Application::loadGame()
{
	if (confirm()) {
		QString fileName = QFileDialog::getOpenFileName(mainWindow(), tr("Load game"), QString(), 
														"Paper Soccer save files (*.pss)");
		if (!fileName.isEmpty()) {
			loadGame(fileName);
		}
	}
}

void Application::loadGame(const QString& fileName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly)) {
		QMessageBox::critical(mainWindow(), tr("Cannot open file"), tr("The save file cannot be opened."));
		return;
	}

	// Disable the controller before we load the data.
	setActiveController(nullptr);

	QDataStream stream(&file);
	stream >> *gameConfig() >> *history();

	if (stream.status() == QDataStream::Ok) {
		setActiveController(gameController());
	} else {
		QMessageBox::critical(mainWindow(), tr("Corrupted save file"), 
							  tr("The save file is invalid and could not be loaded."));

		// Reset the config and history to sane defaults.
		*gameConfig() = GameConfig{};
		history()->clear();

		// Returns to the welcome screen, we cannot just return to the previous active controller
		// because we had to clear history and game config.
		setActiveController(welcomeController());
	}
}

void Application::saveGame()
{
	QString fileName = QFileDialog::getSaveFileName(mainWindow(), tr("Save game"), QString(), 
													tr("Paper Soccer save files (*.pss)"));
	if (!fileName.isEmpty()) {
		saveGame(fileName);
	}
}

void Application::saveGame(const QString& fileName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly)) {
		QMessageBox::critical(mainWindow(), tr("Cannot open file"), 
							  tr("The save file cannot be opened. Game was not saved."));
		return;
	}

	QDataStream stream(&file);
	stream << *gameConfig() << *history();
	recentlySaved()->add(file.fileName());

	if (stream.status() != QDataStream::Ok) {
		QMessageBox::critical(mainWindow(), tr("Error"), tr("Some error occured when writing the save file. " 
															"The file was written but it is probably corrupted."));
	}
}

void Application::quit()
{
	if (confirm()) {
		mainWindow_->close();
	}
}

QAction* Application::newGameAction()
{
	return newGameAction_;
}

QAction* Application::loadGameAction()
{
	return loadGameAction_;
}

QAction* Application::saveGameAction()
{
	return saveGameAction_;
}

QAction* Application::quitAction()
{
	return quitAction_;
}

bool Application::confirm()
{
	if (history()->size() > 0) {
		QMessageBox mbox;
		mbox.setIcon(QMessageBox::Warning);
		mbox.setWindowTitle("Data may be lost");
		mbox.setText(tr("Any unsaved game data will be lost if you do that. Are you sure?"));
		mbox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		mbox.setDefaultButton(QMessageBox::No);
		if (mbox.exec() == QMessageBox::No) {
			return false;
		}
	}
	return true;
}

} // namespace ps
