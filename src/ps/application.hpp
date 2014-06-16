#ifndef PS_APPLICATION_HPP
#define PS_APPLICATION_HPP

#include <QtWidgets/QApplication>
#include <QtWidgets/QAction>

namespace ps
{

class MainWindow;
class Controller;
class GameView;
class GameController;
class GameConfig;
class GameConfigView;
class GameConfigController;
class EditorView;
class EditorController;
class RecentlySaved;
class History;
class WelcomeView;
class WelcomeController;

class Application : public QObject
{
	Q_OBJECT

public:
	Application();
	virtual ~Application();

	MainWindow* mainWindow();

	// Controllers
	Controller* activeController();
	void setActiveController(Controller* controller);

	WelcomeController* welcomeController();
	GameConfigController* gameConfigController();
	GameController* gameController();
	EditorController* editorController();

	// Views
	QWidget* activeView();
	void setActiveView(QWidget* view);

	WelcomeView* welcomeView();
	GameConfigView* gameConfigView();
	GameView* gameView();
	EditorView* editorView();

	// Models
	RecentlySaved* recentlySaved();
	GameConfig* gameConfig();
	History* history();

	// Actions
	void newGame();
	void loadGame();
	void loadGame(const QString& fileName);
	void saveGame();
	void saveGame(const QString& fileName);
	void quit();

	QAction* newGameAction();
	QAction* loadGameAction();
	QAction* saveGameAction();
	QAction* quitAction();

private:
	bool confirm();

	MainWindow* mainWindow_;

	// Controllers
	Controller* activeController_;
	WelcomeController* welcomeController_;
	GameConfigController* gameConfigController_;
	GameController* gameController_;
	EditorController* editorController_;

	// Views
	WelcomeView* welcomeView_;
	GameConfigView* gameConfigView_;
	GameView* gameView_;
	EditorView* editorView_;

	// Models
	RecentlySaved* recentlySaved_;
	GameConfig* gameConfig_;
	History* history_;

	// Actions
	QAction* newGameAction_;
	QAction* loadGameAction_;
	QAction* saveGameAction_;
	QAction* quitAction_;
};

} // namespace ps

#endif // PS_APPLICATION_HPP
