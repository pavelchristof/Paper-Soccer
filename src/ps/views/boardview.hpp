#ifndef PS_VIEWS_BOARD_VIEW_HPP
#define PS_VIEWS_BOARD_VIEW_HPP

#include "../maybe.hpp"
#include "../models/board.hpp"

#include <QtWidgets/QWidget>
#include <QtGui/QPen>
#include <functional>

namespace ps {

/**
 * Shows the game board.
 */
class BoardView : public QWidget
{
	Q_OBJECT

public:
	BoardView(QWidget* parent = nullptr, Qt::WindowFlags f = 0);

	/**
	 * @returns the displayed board.
	 * @note Can be null.
	 */
	const Board* board() const;

	/**
	 * Sets the displayed board.
	 * @note Can be null.
	 */
	void setBoard(const Board* board);

	qreal ballRadius() const;
	void setBallRadius(qreal radius);
	void resetBallRadius();

	qreal pointRadius() const;
	void setPointRadius(qreal radius);
	void resetPointRadius();

	QColor backgroundColor() const;
	void setBackgroundColor(const QColor& color);
	void resetBackgroundColor();

	QPen backgroundLinePen() const;
	void setBackgroundLinePen(const QPen& pen);
	void resetBackgroundLinePen();

	QPen borderEdgePen() const;
	void setBorderEdgePen(const QPen& pen);
	void resetBorderEdgePen();

	QPen oldEdgePen() const;
	void setOldEdgePen(const QPen& pen);
	void resetOldEdgePen();

	QPen newEdgePen() const;
	void setNewEdgePen(const QPen& pen);
	void resetNewEdgePen();

	QBrush ballBrush() const;
	void setBallBrush(const QBrush& brush);
	void resetBallBrush();

	enum DraggingMode {
		NoDrag,
		DragEdge /* (QPoint startPoint) */,
		DragBall
	};

	DraggingMode draggingMode() const;
	void setDraggingMode(DraggingMode mode);

	/**
	 * @see DragEdge
	 */
	// @{
	QPoint startPoint() const;
	void setStartPoint(QPoint point);
	// @}

	bool isSnappingEnabled() const;
	void setSnapping(bool enabled);

	std::function<bool (QPoint)> snapFilter() const;
	void setSnapFilter(std::function<bool (QPoint)> filter);

	bool hasHeightForWidth() const override;
	int heightForWidth(int width) const override;

signals:
	void clicked(Qt::MouseButton button);
	void pointClicked(QPoint point, Qt::MouseButton button);
	void pointMouseEnter(QPoint point);
	void pointMouseLeave(QPoint point);

protected:
	virtual void paintEvent(QPaintEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);

private:
	QTransform boardToWidgetTransform();
	QTransform widgetToBoardTransform();

	qreal ballRadius_;
	qreal pointRadius_;
	QColor backgroundColor_;
	QPen backgroundLinePen_;
	QPen borderEdgePen_;
	QPen oldEdgePen_;
	QPen newEdgePen_;
	QBrush ballBrush_;

	bool snapping_;
	std::function<bool (QPoint)> snapFilter_;
	DraggingMode draggingMode_;
	QPoint startPoint_;
	
	const Board* board_;
	Maybe<QPoint> pointUnderMouse;
};

} // namespace ps

#endif // PS_VIEWS_BOARD_VIEW_HPP
