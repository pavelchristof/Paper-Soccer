#include "playerswitch.hpp"

#include <QtGui/QPainter>
#include <QtGui/QtEvents>
#include <QtGui/QBitmap>

namespace ps {

PlayerSwitchButton::PlayerSwitchButton(QWidget* parent)
	: QAbstractButton(parent)
	, buttonPixmap(":/images/player-switch/button.png")
{
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	setMask(buttonPixmap.mask());
	setCursor({Qt::PointingHandCursor});
}

QSize PlayerSwitchButton::sizeHint() const
{
	return buttonPixmap.size();
}

void PlayerSwitchButton::paintEvent(QPaintEvent* e)
{
	QPainter painter(this);
	painter.drawPixmap(0, 0, buttonPixmap);
}

PlayerSwitch::PlayerSwitch(QWidget* parent, Qt::WindowFlags f)
	: QWidget(parent, f)
	, selectedPlayer(Player::One)
	, backgroundPixmap(":/images/player-switch/background.png")
	, button(new PlayerSwitchButton(this))
	, animation(button, "geometry")
{
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	setMask(backgroundPixmap.mask());

	QSize buttonSize = button->sizeHint();
	QPoint buttonPos = {buttonDestX(selectedPlayer), (backgroundPixmap.height() - buttonSize.height()) / 2};
	button->setGeometry({buttonPos, buttonSize});

	connect(button, &PlayerSwitchButton::clicked, this, &PlayerSwitch::switchPlayer);
}

Player PlayerSwitch::selected() const
{
	return selectedPlayer;
}

void PlayerSwitch::setSelected(Player player)
{
	if (selectedPlayer != player) {
		selectedPlayer = player;

		animation.stop();
		animation.setDuration(1000);
		QRect geom = button->geometry();
		animation.setStartValue(geom);
		geom.setX(buttonDestX(player));
		animation.setEndValue(geom);
		animation.setEasingCurve(QEasingCurve::InOutQuad);
		animation.start();

		emit switched(player);
	}
}

void PlayerSwitch::switchPlayer()
{
	setSelected(!selected());
}

QSize PlayerSwitch::sizeHint() const
{
	return backgroundPixmap.size();
}

void PlayerSwitch::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.drawPixmap(0, 0, backgroundPixmap);
}

int PlayerSwitch::buttonDestX(Player player) const
{
	const int horPadding = 2;
	switch (player) {
		case Player::One:
			return backgroundPixmap.width() - horPadding - button->sizeHint().width();

		case Player::Two:
			return horPadding;
	}
	assert(false);
}

} // namespace ps
