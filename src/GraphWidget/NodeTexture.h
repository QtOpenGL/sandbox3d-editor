#pragma once

#include "Node.h"

class QOpenGLShaderProgram;
class QOpenGLBuffer;

namespace GraphWidget
{

class NodeTexture : public Node
{
public:

	//
	// Constructor / Destructor
    explicit	NodeTexture			(void);
	virtual		~NodeTexture		(void);

	//
	// API
	void		setTexture(unsigned int id) { texture = id; }

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

public: // Resources

	static bool initResources(void);
	static bool releaseResources(void);

protected:

	static QOpenGLShaderProgram * shader;
	static QOpenGLBuffer * vertexPositionBuffer;
	static QOpenGLBuffer * vertexTexCoordsBuffer;

private:

	unsigned int				texture;
	QList<ConnectorInput*>		inputList;
	ConnectorOutput *			output;

public:

	static const qreal width;
	static const qreal height;
};

}
