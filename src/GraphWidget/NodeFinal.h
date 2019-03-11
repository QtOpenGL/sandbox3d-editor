#pragma once

#include "Node.h"

namespace GraphWidget
{

class ConnectorInput;

class NodeFinal : public Node
{
public:

	//
	// Constructor / Destructor
    explicit	NodeFinal		(void);
	virtual		~NodeFinal		(void) override;

	//
	// API
	virtual ConnectorInput *	getInputConnector	(unsigned int index) override;
	virtual ConnectorOutput *	getOutputConnector	(unsigned int index) override;

	virtual qreal				getWidth			(void) const override;
	virtual qreal				getHeight			(void) const override;

protected:

	//
	// Override QGraphicsItem
	QRectF			boundingRect	(void) const override;
	QPainterPath	shape			(void) const override;
    void			paint			(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) override;

private:

	ConnectorInput *			input;

public:

	static const qreal radius;
};

}
