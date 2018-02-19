#include "ConnectorOutput.h"

#include "Node.h"
#include "Edge.h"

#include <QPainter>

#include <QGraphicsScene>

#define USE_FULL_CIRCLE 0

namespace GraphWidget
{

/**
 * @brief Constructor
 */
ConnectorOutput::ConnectorOutput(Node * parent, unsigned int index) : Connector(parent, index)
{
	// ...
}

/**
 * @brief Destructor
 */
ConnectorOutput::~ConnectorOutput(void)
{
	// ...
}

/**
 * @brief ConnectorOutput::addEdge
 * @param pEdge
 */
void ConnectorOutput::addEdge(Edge * pEdge)
{
	m_aEdges << pEdge;
}

/**
 * @brief ConnectorOutput::removeEdge
 * @param pEdge
 */
void ConnectorOutput::removeEdge(Edge * pEdge)
{
	QGraphicsScene * pScene = scene();

	if (pScene)
	{
		pScene->removeItem((QGraphicsItem*)pEdge);
	}

	m_aEdges.removeAll(pEdge);
}

/**
 * @brief ConnectorOutput::removeAllEdges
 */
void ConnectorOutput::removeAllEdges(void)
{
	QGraphicsScene * pScene = scene();

	if (pScene)
	{
		for (Edge * pEdge : m_aEdges)
		{
			pScene->removeItem((QGraphicsItem*)pEdge);
		}
	}

	m_aEdges.clear();
}

/**
 * @brief ConnectorOutput::getConnectorType
 * @return
 */
Connector::EType ConnectorOutput::getConnectorType(void) const
{
	return(Connector::Output);
}

/**
 * @brief ConnectorOutput::boundingRect
 * @return
 */
QRectF ConnectorOutput::boundingRect(void) const
{
#if USE_FULL_CIRCLE
	return QRectF(-radius, -radius, radius*2.0, radius*2.0);
#else
	return QRectF(0.0, -radius, radius, radius*2.0);
#endif // USE_FULL_CIRCLE
}

/**
 * @brief ConnectorOutput::shape
 * @return
 */
QPainterPath ConnectorOutput::shape(void) const
{
	QPainterPath path;
	path.addEllipse(-radius, -radius, radius*2.0, radius*2.0);
	return(path);
}

/**
 * @brief ConnectorOutput::paint
 * @param painter
 * @param option
 * @param widget
 */
void ConnectorOutput::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	if (parentItem()->isSelected())
	{
		painter->setBrush(Qt::blue);
	}
	else
	{
		painter->setBrush(Qt::black);
	}

	painter->setPen(Qt::NoPen);

#if USE_FULL_CIRCLE
	painter->drawEllipse(-radius, -radius, radius*2.0, radius*2.0);
#else
	QPainterPath path;
	path.moveTo(QPointF(0.0, 0.0));
	path.arcTo(QRectF(-radius, -radius, radius*2.0, radius*2.0), 90.0, -180.0);
	painter->drawPath(path);
#endif // USE_FULL_CIRCLE
}

/**
 * @brief ConnectorOutput::itemChange
 * @param change
 * @param value
 * @return
 */
QVariant ConnectorOutput::itemChange(GraphicsItemChange change, const QVariant & value)
{
	if (change == ItemSceneHasChanged)
	{
		if (!scene())
		{
			for (Edge * edge : m_aEdges)
			{
				delete edge;
			}

			m_aEdges.clear();
		}
	}

	return QGraphicsItem::itemChange(change, value);
}

}
