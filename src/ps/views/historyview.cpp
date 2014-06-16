#include "historyview.hpp"
#include "historyviewitem.hpp"
#include "boardview.hpp"
#include "../models/board.hpp"
#include "../models/history.hpp"

#include <QtWidgets/QApplication>
#include <QtWidgets/QLayout>
#include <QtWidgets/QStackedLayout>
#include <QtWidgets/QScrollBar>
#include <QtGui/QtEvents>

namespace ps {

void WidgetWithAResizedSignal::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);
	emit resized();
}

HistoryView::HistoryView(QWidget* parent, Qt::WindowFlags f)
	: QWidget(parent, f)
	, itemsLayout(new QVBoxLayout)
	, scrollArea(new QScrollArea)
	, history_(nullptr)
{
	// Setup the scrollArea.
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	// Weird stuff happens if we set qt::ScrollBarAsNeeded. If the items fit the ScrollArea just right the scroll bar 
	// disappears, items get larger - the scroll bar appears, items get smaller - the scroll bar disappears again...
	// And that triggers even more Qt bugs after you resize the window: items no longer resize with the window...
	// So keep the scroll bar always on.
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	scrollArea->setWidgetResizable(true);

	// One stretch item before the history items and one after them.
	itemsLayout->addStretch();
	itemsLayout->addStretch();

	// Create the container widget.
	WidgetWithAResizedSignal* container = new WidgetWithAResizedSignal;
	container->setLayout(itemsLayout);
	scrollArea->setWidget(container);

	// Set the scroll area as the displayed widget.
	setLayout(new QStackedLayout);
	layout()->addWidget(scrollArea);

	// Ensure that the focused board is visible.
	// QScrollArea::ensureWidgetVisible is useable only after a resize event. If you call it in a more sensible place
	// (like the focusChanged method) it will just scroll to a random widget. Citing some forum post:
	// "ensureWidgetVisible cannot be called immediately [after adding an item] because the scroll area does not know 
	// its final size yet"
	connect(container, &WidgetWithAResizedSignal::resized, [this] () {
		if (history() && history()->focusedIndex().isSome()) {
			scrollArea->ensureWidgetVisible(items[history()->focusedIndex().get()]);
		}
	});
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
		for (int i = 0; i < history->size(); ++i) {
			pushed();
		}
		focusChanged();

		connections.push_back(connect(history, &History::focusChanged, this, &HistoryView::focusChanged));
		connections.push_back(connect(history, &History::pushed, this, &HistoryView::pushed));
		connections.push_back(connect(history, &History::popping, this, &HistoryView::popping));
	}
}

void HistoryView::updateItem(int i)
{
	items[i]->update();
}

void HistoryView::focusChanged()
{
	for (int i = 0; i < items.size(); ++i) {
		items[i]->setFocused(history()->focusedIndex() == some(i));
	}
}

void HistoryView::pushed()
{
	int i = items.size();
	HistoryViewItem* item = new HistoryViewItem;
	item->boardView()->setBoard(history()->boardAt(i));
	connect(item, &HistoryViewItem::clicked, [this, i] () { emit itemClicked(i); });
	itemsLayout->insertWidget(itemsLayout->count() - 1, item);
	items.push_back(item);
}

void HistoryView::popping()
{
	HistoryViewItem* item = items.back();
	itemsLayout->removeWidget(item);
	items.pop_back();
	delete item;
}

} // namespace ps
