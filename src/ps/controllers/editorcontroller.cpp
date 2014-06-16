#include "editorcontroller.hpp"
#include "gamecontroller.hpp"
#include "../application.hpp"
#include "../views/editorview.hpp"
#include "../views/boardview.hpp"
#include "../views/playerswitch.hpp"
#include "../models/history.hpp"

namespace ps {

void EditorController::setup(Application* app)
{
	this->app = app;
	this->view = app->editorView();

	connect(view->playerSwitch(), &PlayerSwitch::clicked, this, &EditorController::switchPlayer);
	connect(view->playButton(), &QCommandLinkButton::clicked, this, &EditorController::play);
	connect(view->boardView(), &BoardView::pointClicked, this, &EditorController::pointClicked);
	connect(view->boardView(), &BoardView::pointMouseEnter, this, &EditorController::pointMouseEnter);
	connect(view->boardView(), &BoardView::pointMouseLeave, this, &EditorController::pointMouseLeave);
}

void EditorController::activate()
{
	app->setActiveView(view);
	view->setConfig(app->gameConfig());
	view->boardView()->setBoard(board());

	view->playerSwitch()->setAnimationsEnabled(false);
	view->playerSwitch()->setEnabled(true);
	view->playerSwitch()->setSelected(board()->currentPlayer());
	view->playerSwitch()->setAnimationsEnabled(true);

	// Actions
	app->newGameAction()->setEnabled(true);
	app->loadGameAction()->setEnabled(true);
	app->saveGameAction()->setEnabled(true);
	app->quitAction()->setEnabled(true);
}

void EditorController::deactivate()
{
	view->setConfig(nullptr);
	view->boardView()->setBoard(nullptr);
}

Board* EditorController::board()
{
	return app->history()->focusedBoard();
}

void EditorController::switchPlayer()
{
	board()->setCurrentPlayer(!board()->currentPlayer());
	view->playerSwitch()->switchSelected();
}

void EditorController::play()
{
	app->setActiveController(app->gameController());
}

Maybe<Edge> EditorController::flippableEdge(QPoint start, QPoint end)
{
	Maybe<Direction> dir = pointToDir(end - start);
	if (dir.isNone()) {
		return none;
	}

	Edge edge{start, dir.get()};
	if (!board()->isEdgeInside(edge)) {
		return none;
	}

	switch (board()->edgeCategory(edge)) {
		case EdgeCategory::Old:
		case EdgeCategory::Empty:
			return edge;

		default:
			return none;
	}
}

void EditorController::flipEdge(QPoint start, QPoint end)
{
	Maybe<Edge> edge = flippableEdge(start, end);
	if (edge.isNone()) {
		return;
	}

	switch (board()->edgeCategory(edge.get())) {
		case EdgeCategory::Old:
			board()->setEdgeCategory(edge.get(), EdgeCategory::Empty);
			break;

		case EdgeCategory::Empty:
			board()->setEdgeCategory(edge.get(), EdgeCategory::Old);
			break;

		default:
			Q_ASSERT(false);
			break;
	}

	view->boardView()->update();
	view->boardView()->setDraggingMode(BoardView::NoDrag);
}

void EditorController::pointClicked(QPoint point, Qt::MouseButton button)
{
	if (view->boardView()->draggingMode() == BoardView::DragEdge) {
		if (button == Qt::RightButton) {
			view->boardView()->setDraggingMode(BoardView::NoDrag);
		} else if (button == Qt::LeftButton) {
			flipEdge(view->boardView()->startPoint(), point);
		}
	} else if (view->boardView()->draggingMode() == BoardView::NoDrag) {
		if (button == Qt::RightButton) {
			board()->setBall(point);
			view->boardView()->update();
		} else if (button == Qt::LeftButton) {
			view->boardView()->setDraggingMode(BoardView::DragEdge);
			view->boardView()->setStartPoint(point);
			view->boardView()->setSnapping(true);
			view->boardView()->setSnapFilter([point, this] (QPoint end) {
				return flippableEdge(point, end).isSome();
			});
		}
	}
}

void EditorController::pointMouseEnter(QPoint point)
{
	if (view->boardView()->draggingMode() == BoardView::NoDrag) {
		view->boardView()->setCursor({Qt::PointingHandCursor});
	}
}

void EditorController::pointMouseLeave(QPoint point)
{
	view->boardView()->setCursor({});
}

} // namespace ps
