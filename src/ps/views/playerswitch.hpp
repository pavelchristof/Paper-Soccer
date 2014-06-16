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
	Q_PROPERTY(QColor color READ color WRITE setColor)

public:
	 PlayerSwitchButton(QWidget* parent = nullptr);
	QSize sizeHint() const override;

	QColor color() const;
	void setColor(QColor color);

protected:
	void paintEvent(QPaintEvent* e) override;

private:
	QPixmap buttonPixmap;
	QRegion region;
	QColor color_;
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
	PlayerSwitch(QWidget* parent = nullptr, Qt::WindowFlags f = 0);

	Player selected() const;
	void setSelected(Player player);
	void switchSelected();

	bool animationsEnabled() const;
	void setAnimationsEnabled(bool enabled);

	QSize sizeHint() const override;

signals:
	void clicked();

protected:
	void paintEvent(QPaintEvent*) override;
	void changeEvent(QEvent* event) override;

private:
	int buttonDestX(Player player) const;

	bool animationsEnabled_;
	Player selectedPlayer;
	QPixmap backgroundPixmap;
	PlayerSwitchButton* button;
	QPropertyAnimation geometryAnim;
	QPropertyAnimation colorAnim;
};

} // namespace ps

#endif // PS_VIEWS_PLAYERSWITCH_HPP
