#include "gameconfig.hpp"

namespace ps {

GameConfig::GameConfig()
	: width_(10)
	, height_(12)
	, isPlayerHuman_{true, false}
{
}

QSize GameConfig::size() const
{
	return {width(), height()};
}

void GameConfig::setSize(QSize size)
{
	setWidth(size.width());
	setHeight(size.height());
}

int GameConfig::width() const
{
	return width_;
}

void GameConfig::setWidth(int width)
{
	Q_ASSERT(width >= minSize && width <= maxSize);
	Q_ASSERT(width % 2 == 0);
	width_ = width;
}

int GameConfig::height() const
{
	return height_;
}

void GameConfig::setHeight(int height)
{
	Q_ASSERT(height >= minSize && height <= maxSize);
	Q_ASSERT(height % 2 == 0);
	height_ = height;
}

bool GameConfig::isPlayerHuman(Player player) const
{
	return isPlayerHuman_[static_cast<quint8>(player)];
}

void GameConfig::setPlayerHuman(Player player, bool human)
{
	isPlayerHuman_[static_cast<quint8>(player)] = human;
}

} // namespace ps
