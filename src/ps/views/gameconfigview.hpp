#ifndef PS_VIEWS_GAMECONFIGVIEW_HPP
#define PS_VIEWS_GAMECONFIGVIEW_HPP

#include <QtWidgets/QWidget>
#include "../maybe.hpp"

class Ui_GameConfig;

namespace ps
{

class GameConfig;

class GameConfigView : public QWidget
{
	Q_OBJECT

public:
	explicit GameConfigView(QWidget* parent = nullptr, Qt::WindowFlags f = 0);

	GameConfig* config() const;
	void setConfig(GameConfig* config);

signals:
	void cancelClicked();
	void startGameClicked();

private:
	Ui_GameConfig* ui;
	GameConfig* config_;
};

} // namespace ps

#endif // PS_VIEWS_GAMECONFIGVIEW_HPP
