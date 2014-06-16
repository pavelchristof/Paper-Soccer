#ifndef PS_MODELS_GAMECONFIG_HPP
#define PS_MODELS_GAMECONFIG_HPP

#include "board.hpp"

#include <QtCore/QDataStream>

namespace ps
{

class GameConfig
{
public:
	static const int minSize = 2;
	static const int maxSize = 30;

	GameConfig();
	GameConfig(const GameConfig&) = default;
	GameConfig(GameConfig&&) = default;
	GameConfig& operator =(const GameConfig&) = default;
	GameConfig& operator =(GameConfig&&) = default;

	QSize size() const;
	void setSize(QSize size);

	int width() const;
	void setWidth(int width);

	int height() const;
	void setHeight(int height);

	bool isPlayerHuman(Player player) const;
	void setPlayerHuman(Player player, bool human);

private:
	friend QDataStream& operator <<(QDataStream& stream, const GameConfig& config);
	friend QDataStream& operator >>(QDataStream& stream, GameConfig& config);

	int width_;
	int height_;
	bool isPlayerHuman_[2];
};

QDataStream& operator <<(QDataStream& stream, const GameConfig& config);
QDataStream& operator >>(QDataStream& stream, GameConfig& config);

} // namespace ps

#endif // PS_MODELS_GAMECONFIG_HPP
