#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QDebug>
#include <QMainWindow>
#include <iostream>

#include "shape.hpp"
#include "model/board.hpp"
#include "model/history.hpp"
#include "views/boardview.hpp"
#include "views/historyview.hpp"
#include "views/gameview.hpp"

using namespace ps;
using namespace std;

void initHistory(History& history) 
{
	Board board({8, 12});

	board.pushStep(North);
	board.finishMove();
	history.push(board);

	board.pushStep(East);
	board.finishMove();
	history.push(board);

	board.pushStep(North);
	board.finishMove();
	history.push(board);
	
	board.pushStep(East);
	board.finishMove();
	history.push(board);

	board.pushStep(North);
	board.finishMove();
	history.push(board);
	
	board.pushStep(East);
	board.finishMove();
	history.push(board);

	board.pushStep(North);
	board.finishMove();
	history.push(board);
	
	board.pushStep(East);
	board.finishMove();
	history.push(board);

	board.pushStep(SouthWest);
	board.pushStep(South);
	history.push(board);

	history.setFocusedIndex(2);
}

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	QMainWindow window;

	History history;
	initHistory(history);

	GameView gameView;
	gameView.boardView()->setBoard(history.focusedBoard());
	gameView.historyView()->setHistory(&history);

	QObject::connect(gameView.historyView(), &HistoryView::itemClicked, [&history, &gameView] (size_t i) {
		history.setFocusedIndex(i);
		gameView.boardView()->setBoard(history.focusedBoard());
	});

	window.setCentralWidget(&gameView);
	window.show();

	return app.exec();
}
