#pragma once

#include "Node.h"

class QOpenGLShaderProgram;
class QOpenGLBuffer;

namespace GraphWidget
{

class NodeOperatorTwoParams : public Node
{

public:

	enum Type
	{
		ADDITION,
		SUBTRACTION,
		MULTIPLICATION,
		DIVISION,

		EQUAL_TO,
		NOT_EQUAL_TO,
		GREATER_THAN,
		GREATER_THAN_OR_EQUAL_TO,
		LESS_THAN,
		LESS_THAN_OR_EQUAL_TO,

		AND,
		OR,
	};

	//
	// Constructor / Destructor
	explicit	NodeOperatorTwoParams				(Type type);
	virtual		~NodeOperatorTwoParams				(void) override;

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

	QString						symbol;
	ConnectorInput *			input1;
	ConnectorInput *			input2;
	ConnectorOutput *			output;

public:

	static qreal width;
	static qreal height;
};

}
