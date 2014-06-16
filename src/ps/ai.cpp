#include "ai.hpp"

namespace ps {

const int infinity = 1000000;

AI::AI(int timestamp, const Board& board)
	: player(board.currentPlayer())
	, timestamp(timestamp)
	, startingBoard(board)
{
}

void AI::run()
{
	Maybe<QVector<Direction>> bestMove{none};
	int bestValue = - 2 * infinity;

	int stepsBefore = startingBoard.currentMove().size();
	startingBoard.enumerateMoves([&] (Board& child, const QVector<Direction>& move) {
		int stepsAdded = move.size() - stepsBefore;
		int value = alphabeta(child, 6 - stepsAdded, -infinity, infinity, true);

		if (value > bestValue) {
			bestMove = move;
			bestValue = value;
		}

		return !isInterruptionRequested();
	});

	if (!isInterruptionRequested() && bestMove.isSome()) {
		startingBoard.setCurrentMove(bestMove.get());
		emit resultReady(timestamp, startingBoard);
	}
}

int AI::alphabeta(Board& board, int depth, int alfa, int beta, bool maximizing)
{
	if (isInterruptionRequested())
		return 0;

	if (depth <= 0 || board.winner().isSome()) {
		return value(board);
	}

	int stepsBefore = board.currentMove().size();
	if (maximizing) {
		board.enumerateMoves([&] (Board& child, const QVector<Direction>& move) {
			int stepsAdded = move.size() - stepsBefore;
			alfa = qMax(alfa, alphabeta(child, depth - stepsAdded, alfa, beta, false));
			return beta > alfa;
		});
		return alfa;
	} else {
		board.enumerateMoves([&] (Board& child, const QVector<Direction>& move) {
			int stepsAdded = move.size() - stepsBefore;
			beta = qMin(beta, alphabeta(child, depth - stepsAdded, alfa, beta, true));
			return beta > alfa;
		});
		return beta;
	}
}

int AI::value(const Board& board)
{
	Maybe<Player> winner = board.winner();
	if (winner.isSome()) {
		if (winner.get() == player) {
			return infinity;
		} else {
			return -infinity;
		}
	}

	if (player == Player::One) {
		return -board.ball().y();
	} else {
		return board.ball().y();
	}
}

} // namespace ps
