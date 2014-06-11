#include "boardview.hpp"

#include <QtGui/QPainter>
#include <QtGui/QtEvents>
#include <QtCore/QDebug>

namespace ps {

BoardView::BoardView(QWidget* parent, Qt::WindowFlags f)
	: QWidget(parent, f)
	, snapping_(true)
	, isDraggingBall_(false)
	, board_(none)
	, pointUnderMouse(none)
{
	resetBallRadius();
	resetPointRadius();
	resetBackgroundColor();
	resetBackgroundLinePen();
	resetBorderEdgePen();
	resetOldEdgePen();
	resetNewEdgePen();
	resetBallBrush();
	setMouseTracking(true);
}

ps::Maybe<const Board&> BoardView::board() const
{
	return board_;
}

void BoardView::setBoard(ps::Maybe<const ps::Board& > board)
{
	board_ = board;
	update();
}

qreal BoardView::ballRadius() const
{
	return ballRadius_;
}

void BoardView::setBallRadius(qreal radius)
{
	ballRadius_ = radius;
	update();
}

void BoardView::resetBallRadius()
{
	setBallRadius(0.1);
}

qreal BoardView::pointRadius() const
{
	return pointRadius_;
}

void BoardView::setPointRadius(qreal radius)
{
	pointRadius_ = radius;
	update();
}

void BoardView::resetPointRadius()
{
	setPointRadius(0.2);
}

QColor BoardView::backgroundColor() const
{
	return backgroundColor_;
}

void BoardView::setBackgroundColor(const QColor& color)
{
	backgroundColor_ = color;
	update();
}

void BoardView::resetBackgroundColor()
{
	setBackgroundColor({0x3f, 0x83, 0x3f});
}

QPen BoardView::backgroundLinePen() const
{
	return backgroundLinePen_;
}

void BoardView::setBackgroundLinePen(const QPen& pen)
{
	backgroundLinePen_ = pen;
	update();
}

void BoardView::resetBackgroundLinePen()
{
	setBackgroundLinePen({{QColor(0x4d, 0x98, 0x4a)}, 0.02, Qt::SolidLine, Qt::RoundCap});
}

QPen BoardView::borderEdgePen() const
{
	return borderEdgePen_;
}

void BoardView::setBorderEdgePen(const QPen& pen)
{
	borderEdgePen_ = pen;
	update();
}

void BoardView::resetBorderEdgePen()
{
	setBorderEdgePen({{Qt::white}, 0.10, Qt::SolidLine, Qt::RoundCap});
}

QPen BoardView::oldEdgePen() const
{
	return oldEdgePen_;
}

void BoardView::setOldEdgePen(const QPen& pen)
{
	oldEdgePen_ = pen;
	update();
}

void BoardView::resetOldEdgePen()
{
	setOldEdgePen({{Qt::white}, 0.05, Qt::SolidLine, Qt::RoundCap});
}

QPen BoardView::newEdgePen() const
{
	return newEdgePen_;
}

void BoardView::setNewEdgePen(const QPen& pen)
{
	newEdgePen_ = pen;
	update();
}

void BoardView::resetNewEdgePen()
{
	setNewEdgePen({{Qt::yellow}, 0.05, Qt::SolidLine, Qt::RoundCap});
}

QBrush BoardView::ballBrush() const
{
	return ballBrush_;
}

void BoardView::setBallBrush(const QBrush& brush)
{
	ballBrush_ = brush;
	update();
}

void BoardView::resetBallBrush()
{
	setBallBrush({Qt::white});
}

bool BoardView::isDraggingBall() const
{
	return isDraggingBall_;
}

void BoardView::setDraggingBall(bool enabled)
{
	isDraggingBall_ = enabled;
	update();
}

bool BoardView::isSnappingEnabled() const
{
	return snapping_;
}

void BoardView::setSnapping(bool enabled)
{
	snapping_ = enabled;
	update();
}

bool BoardView::hasHeightForWidth() const
{
	return board_.isSome();
}

int BoardView::heightForWidth(int width) const
{
	return board_.mapOr<int>([width] (const Board& b) {
		return (width * b.height()) / b.width();
	}, QWidget::heightForWidth(width));
}

void BoardView::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	// Let's pretend that flag does something.
	painter.setRenderHint(QPainter::HighQualityAntialiasing);

	// Paint the background.
	painter.fillRect(painter.viewport(), backgroundColor());

	// If there is no board dont draw anything.
	if (board_.isNone())
		return;
	const Board& board = board_.get();

	// Transform the painter to match board coordinates.
	painter.setTransform(boardToWidgetTransform());

	// Draw background lines.
	const qreal add = 0.5;
	painter.setPen(backgroundLinePen());
	for (int col = -board.halfWidth(); col <= board.halfWidth(); ++col) {
		painter.drawLine(QPointF{(qreal)col, -(board.halfHeight() + add)},
						 QPointF{(qreal)col, board.halfHeight() + add});
	}
	for (int row = -board.halfHeight(); row <= board.halfHeight(); ++row) {
		painter.drawLine(QPointF{-(board.halfWidth() + add), (qreal)row},
						 QPointF{board.halfWidth() + add, (qreal)row});
	}

	// Draw edges.
	for (Edge e : board.edgesInside()) {
		switch (board.edgeCategory(e)) {
			case EdgeCategory::Empty:
				continue;

			case EdgeCategory::Border:
				painter.setPen(borderEdgePen());
				break;

			case EdgeCategory::Old:
				painter.setPen(oldEdgePen());
				break;

			case EdgeCategory::New:
				painter.setPen(newEdgePen());
				break;
		}

		painter.drawLine(e.start(), e.end());
	}

	// Draw the ball and possibly the dragged edge.
	QPointF ballPosition;
	QPoint localPos = mapFromGlobal(QCursor::pos());

	if (isDraggingBall() && rect().contains(localPos)) {
		QPointF start = board.ball();
		QPointF end;

		if (isSnappingEnabled() && pointUnderMouse.isSome()) {
			end = pointUnderMouse.get();
		} else {
			end = widgetToBoardTransform().map(QPointF(localPos));
		}

		ballPosition = end;
		painter.setPen(newEdgePen());
		painter.drawLine(start, end);
	} else {
		ballPosition = board.ball();
	}

	painter.setBrush(ballBrush());
	painter.setPen(Qt::NoPen);
	painter.drawEllipse(ballPosition, ballRadius(), ballRadius());
}

void BoardView::mouseMoveEvent(QMouseEvent* event)
{
	Maybe<QPoint> newPointUnderMouse = board_.bind<QPoint>([=] (const Board& board) -> Maybe<QPoint> {
		QPointF boardPos = widgetToBoardTransform().map(event->localPos());
		QPoint nearestPoint = boardPos.toPoint();

		QPointF difference = boardPos - QPointF{nearestPoint};
		qreal lengthSquared = QPointF::dotProduct(difference, difference);

		if (lengthSquared <= pointRadius() * pointRadius() && board.isPointInside(nearestPoint)) {
			return nearestPoint;
		} else {
			return none;
		}
	});

	if (pointUnderMouse != newPointUnderMouse) {
		if (pointUnderMouse.isSome()) {
			emit pointMouseLeave(pointUnderMouse.get());
		}

		if (newPointUnderMouse.isSome()) {
			emit pointMouseEnter(newPointUnderMouse.get());
		}

		pointUnderMouse = newPointUnderMouse;
	}

	if (isDraggingBall()) {
		update();
	}
}

void BoardView::mouseReleaseEvent(QMouseEvent* event)
{
	if (pointUnderMouse.isSome()) {
		emit pointClicked(pointUnderMouse.get());
	} else {
		event->ignore();
	}
}

QTransform BoardView::boardToWidgetTransform()
{
	return board_.mapOr<QTransform>([this] (const Board& board) {
		QTransform transform;
		transform.translate(width() / 2.0, height() / 2.0);

		QSize boardSize{board.width() + 1, board.height() + 1};
		QSize scaledBoard = boardSize.scaled(size(), Qt::KeepAspectRatio);
		qreal scale = (qreal)scaledBoard.width() / (qreal)boardSize.width();
		transform.scale(scale, -scale);

		return transform;
	}, QTransform{});
}

QTransform BoardView::widgetToBoardTransform()
{
	return board_.mapOr<QTransform>([this] (const Board& board) {
		QTransform transform;

		QSize boardSize{board.width() + 1, board.height() + 1};
		QSize scaledBoard = boardSize.scaled(size(), Qt::KeepAspectRatio);
		qreal scale = (qreal)boardSize.width() / (qreal)scaledBoard.width();
		transform.scale(scale, -scale);

		transform.translate(-width() / 2.0, -height() / 2.0);
		return transform;
	}, QTransform{});
}

} // namespace ps
