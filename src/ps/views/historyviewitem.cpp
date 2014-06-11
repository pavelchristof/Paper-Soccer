#include "historyviewitem.hpp"
#include "boardview.hpp"
#include "historyview.hpp"

namespace ps {

HistoryViewItem::HistoryViewItem(QWidget* parent, Qt::WindowFlags f)
	: QAbstractButton(parent)
	, layout(new QStackedLayout)
	, boardView_(new BoardView)
{
	QSizePolicy policy = sizePolicy();
	policy.setHorizontalPolicy(QSizePolicy::Expanding);
	policy.setVerticalPolicy(QSizePolicy::Preferred);
	policy.setHeightForWidth(true);
	setSizePolicy(policy);

	setWindowFlags(f);
	setLayout(layout);
	layout->addWidget(boardView_);
	layout->setCurrentWidget(boardView_);
	setFocused(false);
}

bool HistoryViewItem::isFocused() const
{
	return isFocused_;
}

void HistoryViewItem::setFocused(bool isFocused)
{
	isFocused_ = isFocused;

	if (isFocused) {
		setCursor({});
		// A lighter green.
		boardView_->setBackgroundColor({0x8A, 0xC1, 0x8A});
	} else {
		setCursor({Qt::PointingHandCursor});
		boardView_->resetBackgroundColor();
	}
}

BoardView* HistoryViewItem::boardView()
{
	return boardView_;
}

const BoardView* HistoryViewItem::boardView() const
{
	return boardView_;
}

void HistoryViewItem::setBoardView(BoardView* boardView)
{
	layout->removeWidget(boardView_);
	boardView_ = boardView;
	layout->addWidget(boardView_);
	setFocused(isFocused());
}

void HistoryViewItem::paintEvent(QPaintEvent* e)
{
	// QAbstractButton makes this method pure, idk why.
	QWidget::paintEvent(e);
}

} // namespace ps
