#include "NodeOperatorOneParam.h"

#include "ConnectorInput.h"
#include "ConnectorOutput.h"

#include <QPainter>

namespace GraphWidget
{

qreal NodeOperatorOneParam::width = 70.0;
qreal NodeOperatorOneParam::height = 70.0;

/**
 * @brief Constructor
 */
NodeOperatorOneParam::NodeOperatorOneParam(Type type) : Node()
{
	setCacheMode(DeviceCoordinateCache);

	unsigned int inputMask = 0;
	unsigned int outputMask = 0;

	switch (type)
	{
		case NEGATION:
		{
			symbol = "NEG";
			inputMask = TYPE_FLOAT_BIT;
			outputMask = TYPE_FLOAT_BIT;
		}
		break;

		case ABSOLUTE:
		{
			symbol = "ABS";
			inputMask = TYPE_FLOAT_BIT;
			outputMask = TYPE_FLOAT_BIT;
		}
		break;

		case NOT:
		{
			symbol = "NOT";
			inputMask = TYPE_BOOL_BIT;
			outputMask = TYPE_BOOL_BIT;
		}
		break;
	}

	input = new ConnectorInput(this, 0);
	input->setPos(0.0, 35.0);
	input->setMask(READ_BIT | inputMask);

	output = new ConnectorOutput(this, 0);
	output->setPos(width, 35.0);
	output->setMask(READ_BIT | outputMask);
}

/**
 * @brief Destructor
 */
NodeOperatorOneParam::~NodeOperatorOneParam(void)
{
	// ...
}

/**
 * @brief Node::getInputConnector
 * @return
 */
ConnectorInput * NodeOperatorOneParam::getInputConnector(unsigned int index)
{
	if (index == 0)
	{
		return input;
	}

	return nullptr;
}

/**
 * @brief Node::getOutputConnector
 * @return
 */
ConnectorOutput * NodeOperatorOneParam::getOutputConnector(unsigned int index)
{
	if (index == 0)
	{
		return output;
	}

	return nullptr;
}

/**
 * @brief NodeOperatorOneParam::getWidth
 * @return
 */
qreal NodeOperatorOneParam::getWidth(void) const
{
	return(width);
}

/**
 * @brief NodeOperatorOneParam::getHeight
 * @return
 */
qreal NodeOperatorOneParam::getHeight(void) const
{
	return(height);
}

/**
 * @brief Destructor
 * @return
 */
QRectF NodeOperatorOneParam::boundingRect(void) const
{
	double border = 1.0;
	QRectF rect = QRectF(0.0 - (border * 0.5) - Connector::radius, 0.0 - (border * 0.5) - Connector::radius, width + (border) + (Connector::radius * 2), height + (border) + (Connector::radius * 2));
	return(rect);
}

/**
 * @brief NodeOperatorOneParam::shape
 * @return
 */
QPainterPath NodeOperatorOneParam::shape(void) const
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
void NodeOperatorOneParam::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
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
