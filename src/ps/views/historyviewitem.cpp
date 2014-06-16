#include "historyviewitem.hpp"
#include "boardview.hpp"
#include "historyview.hpp"

#include <QtWidgets/QStackedLayout>
#include <QtWidgets/QLabel>
#include <QGroupBox>

namespace ps {

HistoryViewItem::HistoryViewItem(QWidget* parent, Qt::WindowFlags f)
	: QAbstractButton(parent)
	, boardView_(new BoardView)
	, label(new QLabel)
{
	QSizePolicy policy = sizePolicy();
	policy.setHorizontalPolicy(QSizePolicy::Expanding);
	policy.setVerticalPolicy(QSizePolicy::Preferred);
	policy.setHeightForWidth(true);
	setSizePolicy(policy);
	setWindowFlags(f);

	QFrame* hline = new QFrame;
	hline->setFrameShape(QFrame::HLine);

	QFrame* frame = new QFrame;
	QVBoxLayout* frameLayout = new QVBoxLayout;
	frame->setLayout(frameLayout);
	frame->setFrameShape(QFrame::Box);

	QMargins margins = frameLayout->contentsMargins();
	margins.setLeft(0);
	margins.setRight(0);
	margins.setBottom(0);
	frameLayout->setContentsMargins(margins);

	frameLayout->setSpacing(0);
	frameLayout->addWidget(label, 0, Qt::AlignCenter);
	frameLayout->addSpacing(margins.top());
	frameLayout->addWidget(hline);
	frameLayout->addWidget(boardView_);	

	QStackedLayout* layout = new QStackedLayout;
	layout->addWidget(frame);
	layout->setCurrentWidget(frame);
	setLayout(layout);

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

void HistoryViewItem::paintEvent(QPaintEvent* e)
{
	// Make sure we update the label text.
	QString text;
	if (boardView_->board()) {
		switch (boardView_->board()->currentPlayer()) {
			case Player::One:
				text = tr("1st player");
				break;

			case Player::Two:
				text = tr("2nd player");
				break;
		}
	}
	label->setText(text);

	QWidget::paintEvent(e);
}

} // namespace ps
