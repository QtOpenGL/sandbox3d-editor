#include "Edge.h"

#include "ConnectorInput.h"
#include "ConnectorOutput.h"

#include <math.h>

#include <QPainter>

namespace GraphWidget
{

/**
 * @brief Constructor
 */
Edge::Edge(ConnectorOutput * source, ConnectorInput * destination) : m_pSource(source), m_pDestination(destination)
{
	setZValue(-1);
	setCacheMode(NoCache);
	setAcceptedMouseButtons(0);
    m_pSource->addEdge(this);
	m_pDestination->setEdge(this);
}

/**
 * @brief Destructor
 */
Edge::~Edge(void)
{
	m_pSource->removeEdge(this);
	m_pDestination->setEdge(nullptr);
}

/**
 * @brief Edge::getSourceNode
 * @return
 */
Node * Edge::getSourceNode(void)
{
	return((Node*)((QGraphicsItem*)m_pSource)->parentItem());
}

/**
 * @brief Edge::getDestinationNode
 * @return
 */
Node * Edge::getDestinationNode(void)
{
	return((Node*)((QGraphicsItem*)m_pDestination)->parentItem());
}

/**
 * @brief Edge::getSourceIndex
 * @return
 */
unsigned int Edge::getSourceIndex(void) const
{
	return(m_pSource->getIndex());
}

/**
 * @brief Edge::getDestinationIndex
 * @return
 */
unsigned int Edge::getDestinationIndex(void) const
{
	return(m_pDestination->getIndex());
}

/**
 * @brief Edge::disconnectNodes
 * @return
 */
bool Edge::disconnectNodes(void) const
{
	m_pDestination->setEdge(nullptr);
	m_pSource->removeAllEdges();
	return(true);
}

/**
 * @brief Edge::boundingRect
 * @return
 */
QRectF Edge::boundingRect(void) const
{
	if (!m_pSource || !m_pDestination)
	{
		return QRectF();
	}

	QPointF sourcePoint = m_pSource->parentItem()->mapToScene(m_pSource->pos());
	QPointF destPoint = m_pDestination->parentItem()->mapToScene(m_pDestination->pos());

	return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(), destPoint.y() - sourcePoint.y())).normalized();
}

/**
 * @brief Edge::paint
 * @param painter
 * @param option
 * @param widget
 */
void Edge::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	if (!m_pSource || !m_pDestination)
	{
		return;
	}

	QPointF sourcePoint(m_pSource->pos());
	QPointF destPoint(m_pDestination->pos());

	sourcePoint.rx() += Connector::radius;
	destPoint.rx() -= Connector::radius;

	QPointF sourcePointScene = m_pSource->parentItem()->mapToScene(sourcePoint);
	QPointF destPointScene = m_pDestination->parentItem()->mapToScene(destPoint);

	// Draw the line
	painter->setPen(QPen(Qt::black, 3.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

#if 0
	QLineF line(sourcePoint, destPoint);
	if (qFuzzyCompare(line.length(), qreal(0.)))
	{
		return;
	}

	painter->drawLine(line);
#else
	float distanceX = abs(sourcePointScene.x() - destPointScene.x());

	QPainterPath path(sourcePointScene);
	path.cubicTo(sourcePointScene + QPointF(distanceX*0.5f, 0.0f), destPointScene - QPointF(distanceX*0.5f, 0.0f), destPointScene);

	painter->drawPath(path);
#endif
}

}
