#include "historyview.hpp"
#include "historyviewitem.hpp"
#include "boardview.hpp"
#include "../model/board.hpp"
#include "../model/history.hpp"

#include <QtWidgets/QLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QStackedLayout>
#include <QtGui/QtEvents>

namespace ps {

HistoryView::HistoryView(QWidget* parent, Qt::WindowFlags f)
	: QWidget(parent, f)
	, history_(nullptr)
	, itemsLayout(new QVBoxLayout)
	, scrollArea(new QScrollArea)
{
	// Setup the scrollArea.
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	// Weird stuff happens if we set qt::ScrollBarAsNeeded. If the items fit the ScrollArea just right the scroll bar 
	// disappears, items get larger - the scroll bar appears, items get smaller - the scroll bar disappears again...
	// And that triggers even more Qt bugs after you resize the window: items no longer resize with the window...
	// So keep the scroll bar always on.
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	scrollArea->setWidgetResizable(true);

	QWidget* container = new QWidget;
	container->setLayout(itemsLayout);
	scrollArea->setWidget(container);

	// Set the scroll area as the displayed widget.
	setLayout(new QStackedLayout);
	layout()->addWidget(scrollArea);
}

History* HistoryView::history()
{
	return history_;
}

const History* HistoryView::history() const
{
	return history_;
}

void HistoryView::setHistory(History* history)
{
	if (history_ != nullptr) {
		for (auto conn : connections) {
			disconnect(conn);
		}

		for (HistoryViewItem* item : items) {
			itemsLayout->removeWidget(item);
			delete item;
		}
		items.clear();
	}

	history_ = history;

	if (history != nullptr) {
		for (size_t i = 0; i < history->size(); ++i) {
			pushed();
		}
		focusChanged();

		connections.push_back(connect(history, &History::focusChanged, this, &HistoryView::focusChanged));
		connections.push_back(connect(history, &History::boardModified, this, &HistoryView::boardModified));
		connections.push_back(connect(history, &History::pushed, this, &HistoryView::pushed));
		connections.push_back(connect(history, &History::popped, this, &HistoryView::popped));
	}
}

void HistoryView::focusChanged()
{
	for (size_t i = 0; i < items.size(); ++i) {
		bool focused = history()->focusedIndex() == some(i);
		items[i]->setFocused(focused);
		if (focused) {
			scrollArea->ensureWidgetVisible(items[i]);
		}
	}
}

void HistoryView::boardModified(size_t i)
{
	items[i]->boardView()->setBoard(history()->boardAt(i));
}

void HistoryView::pushed()
{
	size_t i = items.size();
	HistoryViewItem* item = new HistoryViewItem(this);
	item->boardView()->setBoard(history()->boardAt(i));
	connect(item, &HistoryViewItem::clicked, [this, i] () { emit itemClicked(i); });
	itemsLayout->addWidget(item);
	items.push_back(item);
}

void HistoryView::popped()
{
	HistoryViewItem* item = items.back();
	itemsLayout->removeWidget(item);
	items.pop_back();
	delete item;
}

} // namespace ps
