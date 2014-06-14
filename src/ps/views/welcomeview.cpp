#include "welcomeview.hpp"

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>

namespace ps {

WelcomeView::WelcomeView(QWidget* parent, Qt::WindowFlags f)
	: QWidget(parent, f)
{
	newGameButton_ = new QPushButton(style()->standardIcon(QStyle::SP_FileIcon), tr("New game..."));
	loadGameButton_ = new QPushButton(style()->standardIcon(QStyle::SP_DialogOpenButton), tr("Load game..."));
	recentlySavedList_ = new QListView;
	recentlySavedList_->setEditTriggers(QAbstractItemView::NoEditTriggers);

	QHBoxLayout* buttonsLayout = new QHBoxLayout;
	buttonsLayout->addWidget(newGameButton_);
	buttonsLayout->addWidget(loadGameButton_);

	QVBoxLayout* viewLayout = new QVBoxLayout;
	viewLayout->addItem(buttonsLayout);
	viewLayout->addWidget(new QLabel(tr("Recently saved:")));
	viewLayout->addWidget(recentlySavedList_);
	setLayout(viewLayout);
}

QPushButton* WelcomeView::newGameButton()
{
	return newGameButton_;
}

QPushButton* WelcomeView::loadGameButton()
{
	return loadGameButton_;
}

QListView* WelcomeView::recentlySavedList()
{
	return recentlySavedList_;
}

} // namespace ps
