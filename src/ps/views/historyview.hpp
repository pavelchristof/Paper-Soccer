#ifndef PS_VIEWS_HISTORYVIEW_HPP
#define PS_VIEWS_HISTORYVIEW_HPP

#include "../maybe.hpp"

#include <QtCore/QVector>
#include <QtWidgets/QWidget>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>

namespace ps
{

class Board;
class BoardView;
class History;
class HistoryViewItem;

class WidgetWithAResizedSignal : public QWidget
{
	Q_OBJECT
	
public:
	using QWidget::QWidget;

signals:
	void resized();

protected:
	void resizeEvent(QResizeEvent* event) override;
};

/**
 * Shows the history.
 */
class HistoryView : public QWidget
{
	Q_OBJECT

public:
	HistoryView(QWidget* parent = nullptr, Qt::WindowFlags f = 0);

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

	/**
	 * Calls update() on a item.
	 */
	void updateItem(int i);

signals:
	void itemClicked(int i);

private:
	void focusChanged();
	void pushed();
	void popping();
	
	QVBoxLayout* itemsLayout;
	QScrollArea* scrollArea;
	History* history_;
	QVector<HistoryViewItem*> items;
	QVector<QMetaObject::Connection> connections;
};

} // namespace ps

#endif // PS_VIEWS_HISTORYVIEW_HPP
