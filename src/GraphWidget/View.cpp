#include "View.h"

#include "ConnectionHandler.h"

#include <math.h>

#include <QKeyEvent>

namespace GraphWidget
{

/**
 * @brief Constructor
 * @param parent
 */
View::View(QGraphicsScene * pScene, QWidget * parent) : QGraphicsView(parent), title("No title")
{
	setScene(pScene);
	//setCacheMode(CacheBackground);
	setViewportUpdateMode(BoundingRectViewportUpdate);
	setRenderHint(QPainter::HighQualityAntialiasing);
	setTransformationAnchor(AnchorUnderMouse);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	scale(qreal(1.0), qreal(1.0));
	ConnectionHandler::instance().setView(this);
}

/**
 * @brief Destructor
 */
View::~View(void)
{

}

/**
 * @brief View::setTitle
 */
void View::setTitle(const QString & strTitle)
{
	title.setText(strTitle);
}

/**
 * @brief View::keyPressEvent
 * @param event
 */
void View::keyPressEvent(QKeyEvent *event)
{
	QGraphicsView::keyPressEvent(event); // tmp
}

/**
 * @brief View::mousePressEvent
 * @param event
 */
void View::mousePressEvent(QMouseEvent * event)
{
	switch (event->button())
	{
		case Qt::MiddleButton:
		{
			if (dragMode() == NoDrag)
			{
				// Start Drag
				setDragMode(ScrollHandDrag);
				setInteractive(false);
				// HACK : ScrollHandDrag only responds to LeftButton
				QMouseEvent forceScrollEvent(event->type(), event->localPos(), event->windowPos(), event->screenPos(), Qt::LeftButton, 0, event->modifiers());
				QGraphicsView::mousePressEvent(&forceScrollEvent);
			}
			else
			{
				QGraphicsView::mousePressEvent(event);
			}
		}
		break;

		case Qt::LeftButton:
		{
			if (dragMode() == NoDrag)
			{
				setDragMode(RubberBandDrag);
			}

			QGraphicsView::mousePressEvent(event);
		}
		break;

		default:
		{
			QGraphicsView::mousePressEvent(event);
		}
	}
}

/**
 * @brief View::mouseReleaseEvent
 * @param event
 */
void View::mouseReleaseEvent(QMouseEvent * event)
{
	switch (event->button())
	{
		case Qt::MiddleButton:
		{
			if (dragMode() == ScrollHandDrag)
			{
				// HACK : ScrollHandDrag only responds to LeftButton
				QMouseEvent forceScrollEvent(event->type(), event->localPos(), event->windowPos(), event->screenPos(), Qt::LeftButton, 0, event->modifiers());
				QGraphicsView::mouseReleaseEvent(&forceScrollEvent);
				// Stop Drag
				setInteractive(true);
				setDragMode(NoDrag);
			}
			else
			{
				QGraphicsView::mouseReleaseEvent(event);
			}
		}
		break;

		case Qt::LeftButton:
		{
			QGraphicsView::mouseReleaseEvent(event);

			if (dragMode() == RubberBandDrag)
			{
				setDragMode(NoDrag);
			}
		}
		break;

		default:
		{
			QGraphicsView::mouseReleaseEvent(event);
		}
	}
}

/**
 * @brief View::mouseMoveEvent
 * @param event
 */
void View::mouseMoveEvent(QMouseEvent * event)
{
	QGraphicsView::mouseMoveEvent(event);
}

/**
 * @brief View::wheelEvent
 * @param event
 */
#ifndef QT_NO_WHEELEVENT
void View::wheelEvent(QWheelEvent *event)
{
	scaleView(pow((double)2, event->delta() / 240.0));
}
#endif // QT_NO_WHEELEVENT

/**
 * @brief Draw Background
 * @param painter
 * @param rect
 */
void View::drawBackground(QPainter * painter, const QRectF &rect)
{
	//
	// Draw color
    painter->fillRect(rect, QColor(60, 60, 60));

	//
	// Draw title
	QPointF topLeft(rect.left(), rect.top());
	QPointF viewCoord = mapFromScene(topLeft);
	viewCoord.rx() += 10.0;

	QFont font = painter->font();
    font.setBold(true);
    font.setPixelSize(92);

    painter->setFont(font);
    painter->setPen(QColor(255, 255, 255, 20));

	painter->setMatrixEnabled(false);

    painter->drawStaticText(viewCoord, title);
}

/**
 * @brief View::scaleView
 * @param scaleFactor
 */
void View::scaleView(qreal scaleFactor)
{
	qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}

}
