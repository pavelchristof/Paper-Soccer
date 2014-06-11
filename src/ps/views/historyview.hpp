#ifndef PS_VIEWS_HISTORYVIEW_HPP
#define PS_VIEWS_HISTORYVIEW_HPP

#include "../maybe.hpp"

#include <QtWidgets/QWidget>
#include <QtWidgets/QScrollArea>

namespace ps
{

class Board;
class BoardView;
class History;
class HistoryViewItem;

/**
 * Shows the history.
 */
class HistoryView : public QWidget
{
	Q_OBJECT

public:
	explicit HistoryView(QWidget* parent = nullptr, Qt::WindowFlags f = nullptr);

	/**
	 * @returns the displayed history.
	 * @note Can be null.
	 */
	// @{
	History* history();
	const History* history() const;
	// @}

	/**
	 * Sets the displayed history.
	 * @note Can be null.
	 */
	void setHistory(History* history);

signals:
	void itemClicked(size_t i);

private:
	void focusChanged();
	void boardModified(size_t i);
	void pushed();
	void popped();
	
	QLayout* itemsLayout;
	QScrollArea* scrollArea;
	History* history_;
	std::vector<HistoryViewItem*> items;
	std::vector<QMetaObject::Connection> connections;
};

} // namespace ps

#endif // PS_VIEWS_HISTORYVIEW_HPP
