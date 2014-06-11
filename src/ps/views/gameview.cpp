#include "gameview.hpp"
#include "boardview.hpp"
#include "historyview.hpp"

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>

namespace ps {

GameView::GameView(QWidget* parent, Qt::WindowFlags f)
	: QWidget(parent, f)
{
	QVBoxLayout* verLayout = new QVBoxLayout;
	verLayout->addWidget(new QPushButton{"Test"});
	verLayout->addWidget(new QPushButton{"Another test"});

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
