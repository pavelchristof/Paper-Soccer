#include "mainwindow.hpp"

namespace ps {

MainWindow::MainWindow()
	: QMainWindow()
	, layout_(new QStackedLayout)
{
	setWindowTitle(tr("Paper soccer"));

	QWidget* container = new QWidget;
	container->setLayout(layout_);
	setCentralWidget(container);

}

QStackedLayout* MainWindow::stackedLayout()
{
	return layout_;
}

} // namespace ps
