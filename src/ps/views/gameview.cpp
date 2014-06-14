#include "gameview.hpp"
#include "boardview.hpp"
#include "historyview.hpp"
#include "playerswitch.hpp"

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>
#include <QtSvg/QSvgWidget>

namespace ps {

GameView::GameView(QWidget* parent, Qt::WindowFlags f)
	: QWidget(parent, f)
{
	QVBoxLayout* verLayout = new QVBoxLayout;

	// Center the switch horizontaly.
	playerSwitch_ = new PlayerSwitch;
	QHBoxLayout* switchLayout = new QHBoxLayout;
	switchLayout->addStretch();
	switchLayout->addWidget(playerSwitch_);
	switchLayout->addStretch();
	QWidget* switchContainer = new QWidget;
	switchContainer->setLayout(switchLayout);

	verLayout->addWidget(switchContainer);
	verLayout->addStretch();

	QWidget* leftContainer = new QWidget;
	QSizePolicy policy = leftContainer->sizePolicy();
	policy.setHorizontalStretch(1);
	leftContainer->setSizePolicy(policy);
	leftContainer->setLayout(verLayout);

	boardView_ = new BoardView;
	policy = boardView_->sizePolicy();
	policy.setHorizontalStretch(4);
	boardView_->setSizePolicy(policy);
	boardView_->setMinimumSize(200, 200);

	historyView_ = new HistoryView;
	policy = historyView_->sizePolicy();
	policy.setHorizontalStretch(1);
	historyView_->setSizePolicy(policy);

	QHBoxLayout* horLayout = new QHBoxLayout;
	horLayout->addWidget(leftContainer);
	horLayout->addWidget(boardView_);
	horLayout->addWidget(historyView_);
	setLayout(horLayout);
}

BoardView* GameView::boardView()
{
	return boardView_;
}

const BoardView* GameView::boardView() const
{
	return boardView_;
}

HistoryView* GameView::historyView()
{
	return historyView_;
}

const HistoryView* GameView::historyView() const
{
	return historyView_;
}

} // namespace ps
