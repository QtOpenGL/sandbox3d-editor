#pragma once

#include "Connector.h"

namespace GraphWidget
{

class Node;
class Edge;

class ConnectorOutput : public Connector
{
public:

	//
	// Constructor / Destructor
    explicit		ConnectorOutput		(Node * parent, unsigned int index);
	virtual			~ConnectorOutput	(void);

	//
	// API
	void					addEdge				(Edge * pEdge);
	void					removeEdge			(Edge * pEdge);
	void					removeAllEdges		(void);

	virtual EType			getConnectorType	(void) const override;

protected:

	//
	// Override QGraphicsItem
	QRectF			boundingRect		(void) const override;
	QPainterPath	shape				(void) const override;
    void			paint				(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) override;

	QVariant		itemChange			(GraphicsItemChange change, const QVariant & value) override;

private:

	QList<Edge*> m_aEdges;

};

}
