#ifndef PS_AI_HPP
#define PS_AI_HPP

#include <QtCore/QThread>
#include "models/board.hpp"

namespace ps
{

class AI : public QThread
{
	Q_OBJECT
public:
	/**
	 * Creates an AI starting with @a board.
	 */
	AI(int timestamp, const Board& board);

signals:
	void resultReady(int timestamp, Board board);

protected:
	void run() override;
	int value(const Board& board);
	int alphabeta(const Board& board, int depth, int alfa, int beta, bool maximizing);

private:
	int timestamp;
	Board startingBoard;
	Player player;
};

} // namespace ps

#endif // PS_AI_HPP
