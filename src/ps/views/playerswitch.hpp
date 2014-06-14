#ifndef PS_VIEWS_PLAYERSWITCH_HPP
#define PS_VIEWS_PLAYERSWITCH_HPP

#include "../models/board.hpp"

#include <QtWidgets/QWidget>
#include <QtWidgets/QAbstractButton>
#include <QtCore/QPropertyAnimation>

namespace ps
{

/**
 * Displays the button on a PlayerSwitch.
 */
class PlayerSwitchButton : public QAbstractButton
{
	Q_OBJECT

public:
	explicit PlayerSwitchButton(QWidget* parent = nullptr);
	QSize sizeHint() const override;

protected:
	void paintEvent(QPaintEvent* e) override;

private:
	QPixmap buttonPixmap;
};

/**
 * Displays a fancy button switch.
 */
class PlayerSwitch : public QWidget
{
	Q_OBJECT

public:
	/**
	 * Creates the switch in the player one position.
	 */
	explicit PlayerSwitch(QWidget* parent = nullptr, Qt::WindowFlags f = 0);

	Player selected() const;
	void setSelected(Player player);
	void switchPlayer();

	QSize sizeHint() const override;

signals:
	/**
	 * The selected player was switched to @a player.
	 */
	void switched(Player player);

protected:
	void paintEvent(QPaintEvent*) override;

private:
	int buttonDestX(Player player) const;

	Player selectedPlayer;
	QPixmap backgroundPixmap;
	PlayerSwitchButton* button;
	QPropertyAnimation animation;
};

} // namespace ps

#endif // PS_VIEWS_PLAYERSWITCH_HPP
