#ifndef PS_VIEWS_MAINWINDOW_HPP
#define PS_VIEWS_MAINWINDOW_HPP

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStackedLayout>

namespace ps
{

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

	QStackedLayout* stackedLayout();

private:
	QStackedLayout* layout_;
};

} // namespace ps

#endif // PS_VIEWS_MAINWINDOW_HPP
