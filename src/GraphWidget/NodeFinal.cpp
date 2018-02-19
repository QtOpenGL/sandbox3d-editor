#include "NodeFinal.h"

#include "ConnectorInput.h"

#include <QPainter>

namespace GraphWidget
{

const qreal NodeFinal::radius = 30.0;

/**
 * @brief Constructor
 */
NodeFinal::NodeFinal(void) : Node(), input(nullptr)
{
	setCacheMode(DeviceCoordinateCache);
	input = new ConnectorInput(this, 0);
	input->setPos(0.0, radius);
	input->setMask(READ_BIT | TYPE_TEXTURE_BIT);
}

/**
 * @brief Destructor
 */
NodeFinal::~NodeFinal(void)
{
	// ...
}

/**
 * @brief Node::getInputConnector
 * @return
 */
ConnectorInput * NodeFinal::getInputConnector(unsigned int index)
{
	return(input);
}

/**
 * @brief Node::getOutputConnector
 * @return
 */
ConnectorOutput * NodeFinal::getOutputConnector(unsigned int index)
{
	return(nullptr);
}

/**
 * @brief NodeFinal::getWidth
 * @return
 */
qreal NodeFinal::getWidth(void) const
{
	return(radius*2.0);
}

/**
 * @brief NodeFinal::getHeight
 * @return
 */
qreal NodeFinal::getHeight(void) const
{
	return(radius*2.0);
}

/**
 * @brief Node::boundingRect
 * @return
 */
QRectF NodeFinal::boundingRect(void) const
{
	double border = 1.0;
	QRectF rect = QRectF(0.0 - (border * 0.5), 0.0 - (border * 0.5), (radius * 2.0) + (border), (radius * 2.0) + (border)).normalized();
    return(rect);
}

/**
 * @brief NodeFinal::shape
 * @return
 */
QPainterPath NodeFinal::shape(void) const
{
	QRectF rect = boundingRect();

	QPainterPath path;
	path.addRect(rect);
	return(path);
}

/**
 * @brief Node::paint
 * @param painter
 * @param option
 * @param widget
 */
void NodeFinal::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	double border = 1.0;

	painter->setBrush(Qt::red);

	QPen pen;

	pen.setWidthF(border);

	if (isSelected())
	{
		pen.setColor(Qt::blue);
	}
	else
	{
		pen.setColor(Qt::black);
	}

	painter->setPen(pen);

	painter->drawEllipse(0.0, 0.0, radius*2.0, radius*2.0);
}

}
