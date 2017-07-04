#pragma once

#include "Connector.h"

namespace GraphWidget
{

class Node;
class Edge;

class ConnectorInput : public Connector
{
public:

	//
	// Constructor / Destructor
    explicit				ConnectorInput		(Node * parent, unsigned int index);
	virtual					~ConnectorInput		(void);

	//
	// API
	void					setEdge				(Edge * pEdge);

	virtual EType			getConnectorType	(void) const override;

protected:

	//
	// Override QGraphicsItem
	QRectF			boundingRect		(void) const override;
	QPainterPath	shape				(void) const override;
    void			paint				(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) override;

private:

	Edge * m_pEdge;

};

}
