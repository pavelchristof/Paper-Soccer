#include "ai.hpp"

namespace ps {

const int infinity = 1000000;

AI::AI(int timestamp, const Board& board)
	: timestamp(timestamp)
	, startingBoard(board)
{
}

void AI::run()
{
	Maybe<QVector<Direction>> bestMove{none};
	int bestValue = - 2 * infinity;

	startingBoard.enumerateMoves([&] (const Board& child, const QVector<Direction>& move) {
		int value = alphabeta(child, 4, -infinity, infinity, true);

		if (value > bestValue) {
			bestMove = move;
			bestValue = value;
		}

		return !isInterruptionRequested();
	});

	if (bestMove.isSome()) {
		startingBoard.setCurrentMove(bestMove.get());
		emit resultReady(timestamp, startingBoard);
	}
}

int AI::alphabeta(const Board& board, int depth, int alfa, int beta, bool maximizing)
{
	if (isInterruptionRequested())
		return 0;

	if (depth <= 0 || board.winner().isSome()) {
		return value(board);
	}

	int& alfaOrBeta = maximizing ? alfa : beta;
	board.enumerateMoves([&] (const Board& child, const QVector<Direction>& moves) {
		int stepsAdded = moves.size() - board.currentMove().size();
		alfaOrBeta = qMax(alfaOrBeta, alphabeta(child, depth - stepsAdded, alfa, beta, !maximizing));
		return beta > alfa;
	});
	return alfaOrBeta;
}

int AI::value(const Board& board)
{
	Maybe<Player> winner = board.winner();
	if (winner.isSome()) {
		if (winner.get() == startingBoard.currentPlayer()) {
			return infinity;
		} else {
			return -infinity;
		}
	}

	int enemyGate = board.halfHeight();
	if (player == Player::One) {
		enemyGate = -enemyGate;
	}

	int ydist = abs(enemyGate - board.ball().y());
	int xdist = qMax(0, abs(board.ball().x()) - 1);
	return -ydist - xdist;
}

} // namespace ps
