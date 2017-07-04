#pragma once

#include <QGraphicsItem>


namespace GraphWidget
{

class View;
class Edge;
class ConnectorInput;
class ConnectorOutput;

class Node : public QGraphicsItem
{
public:

	//
	// Constructor / Destructor
    explicit	Node			(void);
	virtual		~Node			(void);

	//
	// Override Type
	enum { Type = UserType + 1 };
    int type(void) const override { return Type; }

	//
	// API
	virtual ConnectorInput *	getInputConnector	(unsigned int index);
	virtual ConnectorOutput *	getOutputConnector	(unsigned int index);

	virtual qreal				getWidth			(void) const = 0;
	virtual qreal				getHeight			(void) const = 0;

protected:

	void		mousePressEvent		(QGraphicsSceneMouseEvent * event) override;
    void		mouseMoveEvent		(QGraphicsSceneMouseEvent * event) override;
    void		mouseReleaseEvent	(QGraphicsSceneMouseEvent * event) override;

};

}
