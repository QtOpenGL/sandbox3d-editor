#include "ConnectionHandler.h"

#include "View.h"
#include "Connector.h"
#include "Edge.h"

#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>

namespace GraphWidget {

/**
 * @brief ConnectionHandler::instance
 * @return
 */
ConnectionHandler & ConnectionHandler::instance()
{
	static ConnectionHandler static_instance;
	return(static_instance);
}

/**
 * @brief Constructor
 */
ConnectionHandler::ConnectionHandler(void) : m_pView(nullptr), m_pLine(nullptr), m_pStartConnector(nullptr)
{
	m_pLine = new QGraphicsLineItem();
}

/**
 * @brief Destructor
 */
ConnectionHandler::~ConnectionHandler(void)
{

}

/**
 * @brief ConnectionHandler::setView
 * @param view
 */
void ConnectionHandler::setView(View * view)
{
	m_pView = view;
	m_pView->scene()->addItem(m_pLine);
}

/**
 * @brief ConnectionHandler::start
 */
void ConnectionHandler::start(Connector * connector)
{
	m_pStartConnector = connector;
}

/**
 * @brief ConnectionHandler::move
 * @param x
 * @param y
 */
void ConnectionHandler::move(float x, float y)
{
	if (nullptr != m_pStartConnector)
	{
		QPointF startPos = m_pStartConnector->mapToScene(QPointF(0.0, 0.0));
		m_pLine->setLine(startPos.x(), startPos.y(), x, y);
	}
}

/**
 * @brief ConnectionHandler::stop
 */
void ConnectionHandler::stop(float x, float y)
{
	if (nullptr != m_pStartConnector)
	{
		QPointF endPos = m_pView->mapFromScene(x, y);

		QGraphicsItem * pEndItem = m_pView->itemAt(endPos.x(), endPos.y());

		if (nullptr != pEndItem)
		{
			if (pEndItem->type() == Connector::Type)
			{
				Connector * pEndConnector = (Connector*)(pEndItem);

				if (m_pStartConnector->getMask() == pEndConnector->getMask())
				{
					if (m_pStartConnector->getConnectorType() == Connector::Input && pEndConnector->getConnectorType() == Connector::Output)
					{
						Edge * edge = new Edge((ConnectorOutput*)pEndConnector, (ConnectorInput*)m_pStartConnector);
						m_pView->scene()->addItem(edge);
					}
					else if (pEndConnector->getConnectorType() == Connector::Input && m_pStartConnector->getConnectorType() == Connector::Output)
					{
						Edge * edge = new Edge((ConnectorOutput*)m_pStartConnector, (ConnectorInput*)pEndConnector);
						m_pView->scene()->addItem(edge);
					}
				}
			}
		}

		m_pLine->setLine(0.0, 0.0, 0.0, 0.0);

		m_pStartConnector = nullptr;
	}
}

}
