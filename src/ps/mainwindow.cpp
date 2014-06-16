#include "mainwindow.hpp"

#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStyle>

namespace ps {

MainWindow::MainWindow(Application* app)
	: QMainWindow()
	, layout_(new QStackedLayout)
{
	setWindowTitle(tr("Paper soccer"));

	QWidget* container = new QWidget;
	container->setLayout(layout_);
	setCentralWidget(container);

	QMenuBar* menuBar = new QMenuBar;
	QMenu* fileMenu = new QMenu(tr("File"));
	fileMenu->addAction(app->newGameAction());
	fileMenu->addAction(app->loadGameAction());
	fileMenu->addSeparator();
	fileMenu->addAction(app->saveGameAction());
	fileMenu->addSeparator();
	fileMenu->addAction(app->quitAction());
	menuBar->addMenu(fileMenu);
	setMenuBar(menuBar);
}

QStackedLayout* MainWindow::stackedLayout()
{
	return layout_;
}

} // namespace ps
