#include "ConnectorInput.h"

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
ConnectorInput::ConnectorInput(Node * parent, unsigned int index) : Connector(parent, index), m_pEdge(nullptr)
{
	// ...
}

/**
 * @brief Destructor
 */
ConnectorInput::~ConnectorInput(void)
{
	// ...
}

/**
 * @brief ConnectorInput::setEdge
 * @param pEdge
 */
void ConnectorInput::setEdge(Edge * pEdge)
{
	if (nullptr != m_pEdge)
	{
		QGraphicsScene * pScene = scene();
		if (pScene)
		{
			pScene->removeItem((QGraphicsItem*)m_pEdge);
		}
		//delete m_pEdge;
	}

	m_pEdge = pEdge;
}

/**
 * @brief ConnectorInput::getConnectorType
 * @return
 */
Connector::EType ConnectorInput::getConnectorType(void) const
{
	return(Connector::Input);
}

/**
 * @brief ConnectorInput::boundingRect
 * @return
 */
QRectF ConnectorInput::boundingRect(void) const
{
#if USE_FULL_CIRCLE
	return QRectF(-radius, -radius, radius*2.0, radius*2.0);
#else
	return QRectF(-radius, -radius, radius, radius*2.0);
#endif // USE_FULL_CIRCLE
}

/**
 * @brief ConnectorInput::shape
 * @return
 */
QPainterPath ConnectorInput::shape(void) const
{
	QPainterPath path;
	path.addEllipse(-radius, -radius, radius*2.0, radius*2.0);
	return(path);
}

/**
 * @brief ConnectorInput::paint
 * @param painter
 * @param option
 * @param widget
 */
void ConnectorInput::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
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
	path.arcTo(QRectF(-radius, -radius, radius*2.0, radius*2.0), 90.0, 180.0);
	painter->drawPath(path);
#endif // USE_FULL_CIRCLE
}

/**
 * @brief ConnectorInput::itemChange
 * @param change
 * @param value
 * @return
 */
QVariant ConnectorInput::itemChange(GraphicsItemChange change, const QVariant & value)
{
	if (change == ItemSceneHasChanged)
	{
		if (!scene())
		{
			delete m_pEdge;
			m_pEdge = nullptr;
		}
	}

	return QGraphicsItem::itemChange(change, value);
}

}
