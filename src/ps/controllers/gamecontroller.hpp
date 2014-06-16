#ifndef PS_CONTROLLERS_GAMECONTROLLER_HPP
#define PS_CONTROLLERS_GAMECONTROLLER_HPP

#include "controller.hpp"
#include "../models/board.hpp"
#include "../views/gameview.hpp"

namespace ps
{

class AI;
class Board;

class GameController : public Controller
{
	Q_OBJECT

public:
	GameController();

	void setup(Application* app) override;
	void activate() override;
	void deactivate() override;

private:
	// -----------------
	// Utility functions

	Board* board();
	bool clearFutureHistory();
	void updateFocusedBoard(bool updatePlayerSwitch);
	void safeDeleteAi();

	// -------
	// Signal handlers

	void edit();
	void historyItemClicked(int i);
	void boardClicked(Qt::MouseButton button);
	void pointClicked(QPoint point, Qt::MouseButton button);
	void pointMouseEnter(QPoint point);
	void pointMouseLeave(QPoint point);
	void hintResultReady(int timestamp, Board board);
	void aiResultReady(int timestamp, Board board);

	// ------
	// States

	enum State {
		Disabled,
		Human,
		HumanDraggingBall,
		HumanHintRunning,
		HumanFinished,
		AIRunning,
		AIStopped
	};

	int time() const;
	State state() const;
	void setState(State state);

	// -----------------
	// State transitions

	// Disabled -> Human
	void disabledToHuman();

	// Disabled -> AIStopped
	void disabledToAIStopped();

	// Human -> HumanDraggingBall
	void startDraggingBall();

	// HumanDraggingBall -> Human
	void stopDraggingBall();

	// HumanDraggingBall -> {Human, HumanDraggingBall, HumanFinished}
	void addSegment(QPoint endPoint);

	// {Human, HumanFinished} -> Human
	void removeSegment();

	// Human -> {Human, HumanHintRunning}
	void startHint();

	// HumanHintRunning -> Human
	void stopHint();

	// HumanHintRunning -> Human
	void finishHint(Board boardAfter);

	// Human -> HumanFinished
	void humanToHumanFinished();

	// HumanFinished -> Human
	void humanFinishedToHuman();

	// Human -> Disabled
	void humanToDisabled();

	// AIStopped -> {AIStopped, AIRunning}
	void aiStart();

	// AIRunning -> AIStopped
	void aiStop();

	// AIRunning -> AIStopped
	void aiFinish(Board boardAfter);

	// AIStopped -> Disabled
	void aiStoppedToDisabled();

	// * -> Disabled
	void anyToDisabled();

	// Disabled -> {Human, HumanFinished, AIRunning, AIStopped}
	void disabledToAppropriate(bool runAi);

	// {Human, HumanDraggingBall, AIStopped} -> {Human, HumanFinished, AIStopped, AIRunning}
	void endTurn();

	// -------
	// Fields

	Application *app;
	GameView *view;
	State state_;
	int time_;
	AI* ai;
};

} // namespace ps

#endif // PS_CONTROLLERS_GAMECONTROLLER_HPP
