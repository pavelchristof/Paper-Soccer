#include "gamecontroller.hpp"
#include "editorcontroller.hpp"
#include "../application.hpp"
#include "../views/gameview.hpp"
#include "../views/historyview.hpp"
#include "../views/boardview.hpp"
#include "../views/playerswitch.hpp"
#include "../models/gameconfig.hpp"
#include "../models/history.hpp"
#include "../mainwindow.hpp"
#include "../ai.hpp"

#include <QtWidgets/QMessageBox>

namespace ps {

GameController::GameController()
	: state_(Disabled)
	, time_(0)
{
}

void GameController::setup(Application* app)
{
	this->app = app;
	this->view = app->gameView();

	connect(view->historyView(), &HistoryView::itemClicked, this, &GameController::historyItemClicked);
	connect(view->boardView(), &BoardView::clicked, this, &GameController::boardClicked);
	connect(view->boardView(), &BoardView::pointClicked, this, &GameController::pointClicked);
	connect(view->boardView(), &BoardView::pointMouseEnter, this, &GameController::pointMouseEnter);
	connect(view->boardView(), &BoardView::pointMouseLeave, this, &GameController::pointMouseLeave);
	connect(view->startHintButton(), &QPushButton::clicked, this, &GameController::startHint);
	connect(view->stopHintButton(), &QPushButton::clicked, this, &GameController::stopHint);
	connect(view->startAiButton(), &QPushButton::clicked, this, &GameController::aiStart);
	connect(view->stopAiButton(), &QPushButton::clicked, this, &GameController::aiStop);
	connect(view->playerSwitch(), &PlayerSwitch::clicked, this, &GameController::endTurn);
	connect(view->editButton(), &QCommandLinkButton::clicked, this, &GameController::edit);
}

void GameController::activate()
{
	view->historyView()->setHistory(app->history());
	app->setActiveView(view);
	app->mainWindow()->showMaximized();

	// Actions
	app->newGameAction()->setEnabled(true);
	app->loadGameAction()->setEnabled(true);
	app->saveGameAction()->setEnabled(true);
	app->quitAction()->setEnabled(true);

	// Set the state based on the board and the game config
	view->playerSwitch()->setAnimationsEnabled(false);
	setState(Disabled);
	disabledToAppropriate(true);
	view->playerSwitch()->setAnimationsEnabled(true);
}

void GameController::deactivate()
{
	anyToDisabled();
	view->historyView()->setHistory(nullptr);
	view->boardView()->setBoard(nullptr);
}

Board* GameController::board()
{
	return app->history()->focusedBoard();
}

bool GameController::clearFutureHistory()
{
	// If the focused item is the last item then there is nothing to clear.
	if (app->history()->focusedIndex() == some(app->history()->size() - 1)) {
		return true;
	}

	// Otherwise ask the user.
	QMessageBox mbox;
	mbox.setIcon(QMessageBox::Warning);
	mbox.setWindowTitle(tr("Future history will be lost"));
	mbox.setText(tr("Performing this action will clear the history after the current time. Do you wish to continue?"));
	mbox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	mbox.setDefaultButton(QMessageBox::No);
	if (mbox.exec() == QMessageBox::Yes) {
		app->history()->clearAfterFocus();
		return true;
	} else {
		return false;
	}
}

void GameController::updateFocusedBoard(bool updatePlayerSwitch)
{
	view->boardView()->update();
	view->historyView()->updateItem(app->history()->focusedIndex().get());
	if (updatePlayerSwitch) {
		view->playerSwitch()->setEnabled(board()->canFinishMove());
	}
}

void GameController::safeDeleteAi()
{
	// This is a bit tricky:
	// If the AI is finished then we will just call deleteLater here.
	// If the AI is not finished we either:
	//     - call deleteLater after the signal fires
	//     - call deleteLater after the signal fires AND here, but that's ok.

	connect(ai, &QThread::finished, ai, &QThread::deleteLater);
	if (ai->isFinished()) {
		ai->deleteLater();
	}
}

void GameController::edit()
{
	// Ask the user.
	if (app->history()->size() > 1) {
		QMessageBox mbox;
		mbox.setWindowTitle(tr("History will be lost"));
		mbox.setText(tr("Performing this action will clear the history. Do you wish to continue?"));
		mbox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		mbox.setDefaultButton(QMessageBox::No);
		if (mbox.exec() == QMessageBox::No) {
			return;
		}
	}

	// Prepare the history and launch the EditorController.
	Board boardCopy = *board();
	boardCopy.convertCurrentMoveToOldEdges();
	anyToDisabled();
	app->history()->clear();
	app->history()->push(boardCopy);
	app->history()->setFocusedIndex(0);
	app->setActiveController(app->editorController());
}

void GameController::historyItemClicked(int i)
{
	anyToDisabled();
	app->history()->setFocusedIndex(i);
	disabledToAppropriate(false);
}

void GameController::boardClicked(Qt::MouseButton button)
{
	if (button != Qt::RightButton)
		return;

	if (state() == Human || state() == HumanFinished) {
		removeSegment();
	} else if (state() == HumanDraggingBall) {
		stopDraggingBall();
	}
}

void GameController::pointClicked(QPoint point, Qt::MouseButton button)
{
	if (state() == Human) {
		if (point == board()->ball()) {
			startDraggingBall();
		}
	} else if (state() == HumanDraggingBall) {
		if (board()->canStepTo(point)) {
			addSegment(point);
		}
	}
}

void GameController::pointMouseEnter(QPoint point)
{
	if (state() == Human && point == board()->ball()) {
		view->boardView()->setCursor({Qt::PointingHandCursor});
	}
}

void GameController::pointMouseLeave(QPoint point)
{
	view->boardView()->setCursor(QCursor());
}

int GameController::time() const
{
	return time_;
}

GameController::State GameController::state() const
{
	return state_;
}

void GameController::setState(State state)
{
	state_ = state;
	++time_;
}

void GameController::disabledToHuman()
{
	Q_ASSERT(state() == Disabled);
	setState(Human);
	view->boardView()->setBoard(board());
	view->playerSwitch()->setSelected(board()->currentPlayer());
	view->playerSwitch()->setEnabled(board()->canFinishMove());
	view->aiBox()->hide();
	view->hintBox()->show();
	view->startHintButton()->setEnabled(true);
	view->stopHintButton()->setEnabled(false);
}

void GameController::disabledToAIStopped()
{
	Q_ASSERT(state() == Disabled);
	setState(AIStopped);
	view->boardView()->setBoard(board());
	view->playerSwitch()->setSelected(board()->currentPlayer());
	view->playerSwitch()->setEnabled(board()->canFinishMove());
	view->aiBox()->show();
	view->hintBox()->hide();
	view->startAiButton()->setEnabled(true);
	view->stopAiButton()->setEnabled(false);
}

void GameController::startDraggingBall()
{
	Q_ASSERT(state() == Human);
	setState(HumanDraggingBall);
	view->boardView()->setDraggingMode(BoardView::DragBall);
	view->boardView()->setSnapping(true);
	view->boardView()->setSnapFilter([this] (QPoint point) { return board()->canStepTo(point); });
}

void GameController::stopDraggingBall()
{
	Q_ASSERT(state() == HumanDraggingBall);
	setState(Human);
	view->boardView()->setDraggingMode(BoardView::NoDrag);
}

void GameController::addSegment(QPoint endPoint)
{
	Q_ASSERT(state() == HumanDraggingBall);

	if (!clearFutureHistory()) {
		return;
	}

	board()->pushStep(pointToDir(endPoint - board()->ball()).get());
	updateFocusedBoard(true);

	if (board()->canFinishMove() || board()->winner().isSome()) {
		stopDraggingBall();
	}

	if (board()->winner().isSome()) {
		humanToHumanFinished();
	}
}

void GameController::removeSegment()
{
	Q_ASSERT(state() == Human || state() == HumanFinished);

	if (!clearFutureHistory()) {
		return;
	}

	if (!board()->currentMove().empty()) {
		board()->popStep();
		updateFocusedBoard(true);
	}

	if (state() == HumanFinished && board()->winner().isNone()) {
		humanFinishedToHuman();
	}
}

void GameController::startHint()
{
	Q_ASSERT(state() == Human);

	if (!clearFutureHistory()) {
		return;
	}

	setState(HumanHintRunning);
	view->playerSwitch()->setEnabled(false);
	view->startHintButton()->setEnabled(false);
	view->stopHintButton()->setEnabled(true);
	QApplication::setOverrideCursor({Qt::BusyCursor});

	ai = new AI(time(), *board());
	connect(ai, &AI::resultReady, this, &GameController::hintResultReady, Qt::QueuedConnection);
	ai->start();
}

void GameController::hintResultReady(int timestamp, Board board)
{
	// It is possible that a late signal arrives e.g. when we cancel 
	// the hint, but the resultReady signal is already waiting in the queue.
	// Just ignore signals that dont match the current time.
	if (timestamp == time()) {
		finishHint(board);
	}
}

void GameController::stopHint()
{
	Q_ASSERT(state() == HumanHintRunning);
	setState(Human);
	view->playerSwitch()->setEnabled(board()->canFinishMove());
	view->startHintButton()->setEnabled(true);
	view->stopHintButton()->setEnabled(false);
	QApplication::restoreOverrideCursor();
	ai->requestInterruption();
	safeDeleteAi();
}

void GameController::finishHint(Board boardAfter)
{
	Q_ASSERT(state() == HumanHintRunning);
	setState(Human);
	view->startHintButton()->setEnabled(true);
	view->stopHintButton()->setEnabled(false);
	QApplication::restoreOverrideCursor();
	safeDeleteAi();
	*board() = boardAfter;
	updateFocusedBoard(true);
}

void GameController::humanToHumanFinished()
{
	Q_ASSERT(state() == Human);
	setState(HumanFinished);
	view->startHintButton()->setEnabled(false);
}

void GameController::humanFinishedToHuman()
{
	Q_ASSERT(state() == HumanFinished);
	setState(Human);
	view->startHintButton()->setEnabled(true);
}

void GameController::humanToDisabled()
{
	Q_ASSERT(state() == Human);
	setState(Disabled);
	view->boardView()->setBoard(nullptr);
	view->playerSwitch()->setEnabled(false);
	view->aiBox()->hide();
	view->hintBox()->hide();
}

void GameController::aiStart()
{
	Q_ASSERT(state() == AIStopped);

	if (!clearFutureHistory()) {
		return;
	}

	setState(AIRunning);
	view->playerSwitch()->setEnabled(false);
	view->startAiButton()->setEnabled(false);
	view->stopAiButton()->setEnabled(true);
	QApplication::setOverrideCursor({Qt::BusyCursor});

	ai = new AI(time(), *board());
	connect(ai, &AI::resultReady, this, &GameController::aiResultReady, Qt::QueuedConnection);
	ai->start();
}

void GameController::aiResultReady(int timestamp, Board board)
{
	// It is possible that a late signal arrives e.g. when we cancel 
	// the hint, but the resultReady signal is already waiting in the queue.
	// Just ignore signals that dont match the current time.
	if (timestamp == time()) {
		aiFinish(board);
	}
}

void GameController::aiStop()
{
	Q_ASSERT(state() == AIRunning);
	setState(AIStopped);
	view->playerSwitch()->setEnabled(board()->canFinishMove());
	view->startAiButton()->setEnabled(true);
	view->stopAiButton()->setEnabled(false);
	QApplication::restoreOverrideCursor();
	ai->requestInterruption();
	safeDeleteAi();
}

void GameController::aiFinish(Board boardAfter)
{
	Q_ASSERT(state() == AIRunning);
	setState(AIStopped);
	view->startHintButton()->setEnabled(true);
	view->stopHintButton()->setEnabled(false);
	QApplication::setOverrideCursor({});
	safeDeleteAi();

	*board() = boardAfter;
	updateFocusedBoard(true);

	if (boardAfter.canFinishMove() && boardAfter.winner().isNone()) {
		endTurn();
	}
}

void GameController::aiStoppedToDisabled()
{
	Q_ASSERT(state() == AIStopped);
	setState(Disabled);
	view->boardView()->setBoard(nullptr);
	view->playerSwitch()->setEnabled(false);
	view->aiBox()->hide();
	view->hintBox()->hide();
}

void GameController::anyToDisabled()
{
	switch (state()) {
		case Disabled:
			break;

		case Human:
			humanToDisabled();
			break;

		case HumanDraggingBall:
			stopDraggingBall();
			humanToDisabled();
			break;

		case HumanHintRunning:
			stopHint();
			humanToDisabled();
			break;

		case HumanFinished:
			humanFinishedToHuman();
			humanToDisabled();
			break;

		case AIStopped:
			aiStoppedToDisabled();
			break;

		case AIRunning:
			aiStop();
			aiStoppedToDisabled();
			break;
	}
}

void GameController::disabledToAppropriate(bool runAi)
{
	Q_ASSERT(state() == Disabled);

	if (board() == nullptr) {
		return;
	}

	if (app->gameConfig()->isPlayerHuman(board()->currentPlayer())) {
		disabledToHuman();
		if (board()->winner().isSome()) {
			humanToHumanFinished();
		}
	} else {
		disabledToAIStopped();

		if (runAi) {
			aiStart();
		}
	}
}

void GameController::endTurn()
{
	Q_ASSERT(state() == Human || state() == HumanDraggingBall || state() == AIStopped);
	Q_ASSERT(board()->canFinishMove());

	if (!clearFutureHistory()) {
		return;
	}

	Board boardCopy(*board());
	boardCopy.finishMove();

	anyToDisabled();
	app->history()->push(boardCopy);
	app->history()->focusLast();
	disabledToAppropriate(true);
}

} // namespace ps
