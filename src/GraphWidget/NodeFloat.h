#pragma once

#include "Node.h"

class QOpenGLShaderProgram;
class QOpenGLBuffer;

namespace GraphWidget
{

class NodeFloat : public QObject, public Node
{
	Q_OBJECT

public:

	//
	// Constructor / Destructor
	explicit	NodeFloat			(float defaultValue = 0.0f);
	virtual		~NodeFloat			(void) override;

	//
	// API
	void		setValue(float v) { value = v; }
	float		getValue(void) const { return value; }

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

private slots:

	void updateValue(const QString & strText);

private:

	float value;

	ConnectorOutput *			output;

public:

	static qreal width;
	static qreal height;
};

}
