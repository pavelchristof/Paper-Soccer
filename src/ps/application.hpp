#ifndef PS_APPLICATION_HPP
#define PS_APPLICATION_HPP

#include <QtWidgets/QApplication>

namespace ps
{

class MainWindow;
class Controller;
class GameView;
class GameController;
class GameConfig;
class GameConfigView;
class GameConfigController;
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

	// Views
	QWidget* activeView();
	void setActiveView(QWidget* view);

	WelcomeView* welcomeView();
	GameConfigView* gameConfigView();
	GameView* gameView();

	// Models
	RecentlySaved* recentlySaved();
	GameConfig* gameConfig();
	History* history();

	// Common actions
	void newGame();
	void loadGame();
	void loadGame(const QString& fileName);
	void saveGame();
	void saveGame(const QString& fileName);

private:
	MainWindow* mainWindow_;

	// Controllers
	Controller* activeController_;
	WelcomeController* welcomeController_;
	GameConfigController* gameConfigController_;
	GameController* gameController_;

	// Views
	WelcomeView* welcomeView_;
	GameConfigView* gameConfigView_;
	GameView* gameView_;

	// Models
	RecentlySaved* recentlySaved_;
	GameConfig* gameConfig_;
	History* history_;
};

} // namespace ps

#endif // PS_APPLICATION_HPP
