#include "playerswitch.hpp"

#include <QtGui/QPainter>
#include <QtGui/QtEvents>
#include <QtGui/QBitmap>

namespace ps {

PlayerSwitchButton::PlayerSwitchButton(QWidget* parent)
	: QAbstractButton(parent)
	, buttonPixmap(":/images/player-switch/button.png")
	, color_(0, 0, 0, 0)
{
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	QBitmap mask = buttonPixmap.mask();
	setMask(mask);
	region = mask;
	setCursor({Qt::PointingHandCursor});
}

QColor PlayerSwitchButton::color() const
{
	return color_;
}

void PlayerSwitchButton::setColor(QColor color)
{
	color_ = color;
	update();
}

QSize PlayerSwitchButton::sizeHint() const
{
	return buttonPixmap.size();
}

void PlayerSwitchButton::paintEvent(QPaintEvent* e)
{
	QPainter painter(this);
	painter.setClipRegion(region);
	painter.drawPixmap(0, 0, buttonPixmap);
	painter.fillRect(painter.viewport(), color());
}

PlayerSwitch::PlayerSwitch(QWidget* parent, Qt::WindowFlags f)
	: QWidget(parent, f)
	, animationsEnabled_(true)
	, selectedPlayer(Player::One)
	, backgroundPixmap(":/images/player-switch/background.png")
	, button(new PlayerSwitchButton(this))
	, geometryAnim(button, "geometry")
	, colorAnim(button, "color")
{
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	setMask(backgroundPixmap.mask());

	QSize buttonSize = button->sizeHint();
	QPoint buttonPos = {buttonDestX(selectedPlayer), (backgroundPixmap.height() - buttonSize.height()) / 2};
	button->setGeometry({buttonPos, buttonSize});

	connect(button, &PlayerSwitchButton::clicked, this, &PlayerSwitch::clicked);
}

Player PlayerSwitch::selected() const
{
	return selectedPlayer;
}

void PlayerSwitch::setSelected(ps::Player player)
{
	if (selectedPlayer != player) {
		selectedPlayer = player;

		geometryAnim.stop();
		QRect geom = button->geometry();
		geom.setX(buttonDestX(player));

		if (animationsEnabled()) {
			geometryAnim.setDuration(500);
			geometryAnim.setStartValue(button->geometry());
			geometryAnim.setEndValue(geom);
			geometryAnim.setEasingCurve(QEasingCurve::InOutQuad);
			geometryAnim.start();
		} else {
			button->setGeometry(geom);
		}
	}
}

void PlayerSwitch::switchSelected()
{
	setSelected(!selected());
}

bool PlayerSwitch::animationsEnabled() const
{
	return animationsEnabled_;
}

void PlayerSwitch::setAnimationsEnabled(bool enabled)
{
	animationsEnabled_ = enabled;
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

void PlayerSwitch::changeEvent(QEvent* event)
{
	QWidget::changeEvent(event);

	if (event->type() == QEvent::EnabledChange) {
		colorAnim.stop();

		QColor endValue;
		if (isEnabled()) {
			endValue = QColor{0, 0, 0, 0};
		} else {
			endValue = QColor{0, 0, 0, 32};
		}

		if (animationsEnabled()) {
			colorAnim.setDuration(500);
			colorAnim.setStartValue(button->color());
			colorAnim.setEndValue(endValue);
			colorAnim.setEasingCurve(QEasingCurve::Linear);
			colorAnim.start();
		} else {
			button->setColor(endValue);
		}
	}
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

	// Never reaches here.
	return horPadding;
}

} // namespace ps
