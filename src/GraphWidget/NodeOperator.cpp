#include "NodeOperator.h"

#include "ConnectorInput.h"
#include "ConnectorOutput.h"

#include <QPainter>

namespace GraphWidget
{

qreal NodeOperator::width = 70.0;
qreal NodeOperator::height = 70.0;

/**
 * @brief Constructor
 */
NodeOperator::NodeOperator(Type type) : Node()
{
	setCacheMode(DeviceCoordinateCache);

	unsigned int mask = 0;

	switch (type)
	{
		case ADDITION:
		{
			symbol = "+";
			mask = TYPE_FLOAT_BIT;
		}
		break;

		case SUBTRACTION:
		{
			symbol = "-";
			mask = TYPE_FLOAT_BIT;
		}
		break;

		case MULTIPLICATION:
		{
			symbol = "\u00D7";
			mask = TYPE_FLOAT_BIT;
		}
		break;

		case DIVISION:
		{
			symbol = "\u00F7";
			mask = TYPE_FLOAT_BIT;
		}
		break;

		case EQUAL_TO:
		{
			symbol = "=";
			mask = TYPE_BOOL_BIT;
		}
		break;

		case NOT_EQUAL_TO:
		{
			symbol = "\u2260";
			mask = TYPE_BOOL_BIT;
		}
		break;

		case GREATER_THAN:
		{
			symbol = ">";
			mask = TYPE_BOOL_BIT;
		}
		break;

		case GREATER_THAN_OR_EQUAL_TO:
		{
			symbol = "\u2265";
			mask = TYPE_BOOL_BIT;
		}
		break;

		case LESS_THAN:
		{
			symbol = "<";
			mask = TYPE_BOOL_BIT;
		}
		break;

		case LESS_THAN_OR_EQUAL_TO:
		{
			symbol = "\u2264";
			mask = TYPE_BOOL_BIT;
		}
		break;
	}

	input1 = new ConnectorInput(this, 0);
	input1->setPos(0.0, 20.0);
	input1->setMask(READ_BIT | TYPE_FLOAT_BIT);

	input2 = new ConnectorInput(this, 1);
	input2->setPos(0.0, 50.0);
	input2->setMask(READ_BIT | TYPE_FLOAT_BIT);

	output = new ConnectorOutput(this, 0);
	output->setPos(width, 35.0);
	output->setMask(READ_BIT | mask);
}

/**
 * @brief Destructor
 */
NodeOperator::~NodeOperator(void)
{
	// ...
}

/**
 * @brief Node::getInputConnector
 * @return
 */
ConnectorInput * NodeOperator::getInputConnector(unsigned int index)
{
	if (index == 0)
	{
		return input1;
	}
	else if (index == 1)
	{
		return input2;
	}

	return nullptr;
}

/**
 * @brief Node::getOutputConnector
 * @return
 */
ConnectorOutput * NodeOperator::getOutputConnector(unsigned int index)
{
	if (index == 0)
	{
		return(output);
	}

	return nullptr;
}

/**
 * @brief NodeOperator::getWidth
 * @return
 */
qreal NodeOperator::getWidth(void) const
{
	return(width);
}

/**
 * @brief NodeOperator::getHeight
 * @return
 */
qreal NodeOperator::getHeight(void) const
{
	return(height);
}

/**
 * @brief Destructor
 * @return
 */
QRectF NodeOperator::boundingRect(void) const
{
	double border = 1.0;
	QRectF rect = QRectF(0.0 - (border * 0.5) - Connector::radius, 0.0 - (border * 0.5) - Connector::radius, width + (border) + (Connector::radius * 2), height + (border) + (Connector::radius * 2));
	return(rect);
}

/**
 * @brief NodeOperator::shape
 * @return
 */
QPainterPath NodeOperator::shape(void) const
{
	QRectF rect = QRectF(0.0, 0.0, width, height);

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
void NodeOperator::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	double border = 1.0;
	QRectF rect = QRectF(0.0, 0.0, width, height);

	painter->setBrush(QColor(90, 90, 90, 220));

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

	painter->drawRoundedRect(rect, 5.0, 5.0);

	painter->setPen(Qt::white);

	QFont font;
	font.setPixelSize(30);
	font.setBold(true);

	painter->setFont(font);

	painter->drawText(rect, Qt::AlignCenter, symbol);
}

}
