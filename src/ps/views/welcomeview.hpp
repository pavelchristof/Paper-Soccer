#ifndef PS_VIEWS_WELCOMEVIEW_HPP
#define PS_VIEWS_WELCOMEVIEW_HPP

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QListView>

namespace ps
{

class WelcomeView : public QWidget
{
	Q_OBJECT

public:
	explicit WelcomeView(QWidget* parent = nullptr, Qt::WindowFlags f = 0);

	QPushButton* newGameButton();
	QPushButton* loadGameButton();
	QListView* recentlySavedList();

private:
	QPushButton* newGameButton_;
	QPushButton* loadGameButton_;
	QListView* recentlySavedList_;
};

}

#endif // PS_VIEWS_WELCOMEVIEW_HPP
