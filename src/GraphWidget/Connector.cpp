#include "Connector.h"

#include "ConnectionHandler.h"

#include <QPainter>
#include <QCursor>

#include <QGraphicsSceneMouseEvent>

namespace GraphWidget
{

const qreal Connector::radius = 9.0;

/**
 * @brief Constructor
 */
Connector::Connector(Node * parent, unsigned int index) : QGraphicsItem((QGraphicsItem*)parent), m_index(index), m_mask(0)
{
	setZValue(1);
	setCacheMode(NoCache);
	setAcceptHoverEvents(true);
	setAcceptDrops(true);
}

/**
 * @brief Destructor
 */
Connector::~Connector(void)
{
	// ...
}

/**
 * @brief Connector::getIndex
 * @return
 */
unsigned int Connector::getIndex(void) const
{
	return(m_index);
}

/**
 * @brief Connector::setMask
 * @param mask
 */
void Connector::setMask(unsigned int mask)
{
	m_mask = mask;
}

/**
 * @brief Connector::getMask
 * @return
 */
unsigned int Connector::getMask(void) const
{
	return(m_mask);
}

/**
 * @brief Connector::mousePressEvent
 * @param event
 */
void Connector::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	ConnectionHandler & handler = ConnectionHandler::instance();
	handler.start(this);
}

/**
 * @brief Connector::mouseMoveEvent
 * @param event
 */
void Connector::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	ConnectionHandler & handler = ConnectionHandler::instance();
	QPointF absolute_pos = mapToScene(event->pos());
	handler.move(absolute_pos.x(), absolute_pos.y());
}

/**
 * @brief Connector::mouseReleaseEvent
 * @param event
 */
void Connector::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	ConnectionHandler & handler = ConnectionHandler::instance();
	QPointF absolute_pos = mapToScene(event->pos());
	handler.stop(absolute_pos.x(), absolute_pos.y());
}

/**
 * @brief Connector::hoverEnterEvent
 * @param event
 */
void Connector::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
	setCursor(Qt::CrossCursor);
	QGraphicsItem::hoverEnterEvent(event);
}

/**
 * @brief Connector::hoverLeaveEvent
 * @param event
 */
void Connector::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
	setCursor(Qt::ArrowCursor);
	QGraphicsItem::hoverLeaveEvent(event);
}

/**
 * @brief Connector::hoverMoveEvent
 * @param event
 */
void Connector::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
	//setCursor(Qt::ArrowCursor);
	QGraphicsItem::hoverMoveEvent(event);
}

}
