#pragma once

#include "Node.h"

class QOpenGLShaderProgram;
class QOpenGLBuffer;

namespace GraphWidget
{

class NodeOperatorOneParam : public Node
{

public:

	enum Type
	{
		NEGATION,
		ABSOLUTE,

		NOT,
	};

	//
	// Constructor / Destructor
	explicit	NodeOperatorOneParam				(Type type);
	virtual		~NodeOperatorOneParam				(void) override;

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
	ConnectorInput *			input;
	ConnectorOutput *			output;

public:

	static qreal width;
	static qreal height;
};

}
