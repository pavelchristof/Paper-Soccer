#ifndef PS_MAINWINDOW_HPP
#define PS_MAINWINDOW_HPP

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStackedLayout>

#include "application.hpp"

namespace ps
{

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(Application* app);

	QStackedLayout* stackedLayout();

private:
	QStackedLayout* layout_;
};

} // namespace ps

#endif // PS_MAINWINDOW_HPP
