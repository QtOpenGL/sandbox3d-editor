#pragma once

#include <QGraphicsItem>

namespace GraphWidget
{

class View;
class Node;
class ConnectorInput;
class ConnectorOutput;

class Edge : public QGraphicsItem
{
public:

	//
	// Constructor / Destructor
	explicit	Edge			(ConnectorOutput * source, ConnectorInput * destination);
	virtual		~Edge			(void);

	//
	// Override Type
	enum { Type = UserType + 2 };
    int type(void) const override { return Type; }

	//
	// API
	Node * getSourceNode(void);
	Node * getDestinationNode(void);

	unsigned int getSourceIndex(void) const;
	unsigned int getDestinationIndex(void) const;

	bool disconnectNodes(void) const;

protected:

	//
	// Override QGraphicsItem
	QRectF		boundingRect	(void) const override;
    void		paint			(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) override;

private:

	ConnectorOutput * m_pSource;
	ConnectorInput * m_pDestination;

};

}
