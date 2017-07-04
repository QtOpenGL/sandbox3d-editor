#include "Node.h"

#include "Edge.h"

#include <QCursor>

namespace GraphWidget
{

/**
 * @brief Constructor
 */
Node::Node(void)
{
	setZValue(0);
	setFlag(ItemIsMovable);
	setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
}

/**
 * @brief Destructor
 */
Node::~Node(void)
{
	// ...
}

/**
 * @brief Node::getInputConnector
 * @return
 */
ConnectorInput * Node::getInputConnector(unsigned int index)
{
	return(nullptr);
}

/**
 * @brief Node::getOutputConnector
 * @return
 */
ConnectorOutput * Node::getOutputConnector(unsigned int index)
{
	return(nullptr);
}

/**
 * @brief Connector::mousePressEvent
 * @param event
 */
void Node::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	setCursor(Qt::SizeAllCursor);
	QGraphicsItem::mousePressEvent(event);
}

/**
 * @brief Connector::mouseMoveEvent
 * @param event
 */
void Node::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	//setCursor(Qt::ClosedHandCursor);
	QGraphicsItem::mouseMoveEvent(event);
}

/**
 * @brief Connector::mouseReleaseEvent
 * @param event
 */
void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	setCursor(Qt::ArrowCursor);
	QGraphicsItem::mouseReleaseEvent(event);
}

}
