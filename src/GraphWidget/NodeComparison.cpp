#include "NodeComparison.h"

#include "ConnectorInput.h"
#include "ConnectorOutput.h"

#include "View.h"

#include <QGraphicsScene>

#include <QPainter>

namespace GraphWidget
{

const qreal NodeComparison::width = 200.0;
const qreal NodeComparison::height = 200.0;

/**
 * @brief Constructor
 */
NodeComparison::NodeComparison(void) : Node()
{
	setCacheMode(DeviceCoordinateCache);

	input1 = new ConnectorInput(this, 0);
	input1->setPos(0.0, 40.0 + 15.0);
	input1->setMask(READ_BIT | TYPE_FLOAT_BIT);

	input2 = new ConnectorInput(this, 1);
	input2->setPos(0.0, 40.0 + 15.0 + 25.0);
	input2->setMask(READ_BIT | TYPE_FLOAT_BIT);

	output = new ConnectorOutput(this, 0);
	output->setPos(width, 40.0 + 15.0);
	output->setMask(WRITE_BIT | TYPE_BOOL_BIT);
}

/**
 * @brief Destructor
 */
NodeComparison::~NodeComparison(void)
{
	// ...
}

/**
 * @brief Node::getInputConnector
 * @return
 */
ConnectorInput * NodeComparison::getInputConnector(unsigned int index)
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
ConnectorOutput * NodeComparison::getOutputConnector(unsigned int index)
{
	if (index == 0)
	{
		return(output);
	}

	return nullptr;
}

/**
 * @brief NodeComparison::getWidth
 * @return
 */
qreal NodeComparison::getWidth(void) const
{
	return(width);
}

/**
 * @brief NodeComparison::getHeight
 * @return
 */
qreal NodeComparison::getHeight(void) const
{
	return(height);
}

/**
 * @brief Node::boundingRect
 * @return
 */
QRectF NodeComparison::boundingRect(void) const
{
	double border = 1.0;
	QRectF rect = QRectF(0.0 - (border * 0.5) - Connector::radius, 0.0 - (border * 0.5) - Connector::radius, width + (border) + (Connector::radius * 2), height + (border) + (Connector::radius * 2));
    return(rect);
}

/**
 * @brief NodeComparison::shape
 * @return
 */
QPainterPath NodeComparison::shape(void) const
{
	QRectF rect = QRectF(0.0, 0.0, width, height);

	QPainterPath path;
	path.addRoundedRect(rect, 5.0, 5.0);
	return(path);
}

/**
 * @brief Node::paint
 * @param painter
 * @param option
 * @param widget
 */
void NodeComparison::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
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

	//
	// title
	QRect rect2(0.0, 0.0, width, 40.0);

	QPainterPath path;
    path.setFillRule( Qt::WindingFill );
    path.addRoundedRect(rect2, 5.0, 5.0);
    path.addRect(0.0, 5.0, width, 35.0);

	painter->setPen(Qt::NoPen);
	painter->setBrush(Qt::black);

    painter->drawPath( path.simplified() ); // Only Top left & bottom right corner

	painter->setPen(Qt::white);

	painter->drawText(rect2, Qt::AlignCenter, "comparison");

	painter->setPen(QColor(200, 200, 200, 255));

	{
		int i = 0;

		QSize size(width, Connector::radius * 2.0);

		{
			QRectF rect(input1->pos(), size);
			rect.translate(5.0, -Connector::radius);
			painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, "in1");
			++i;
		}

		{
			QRectF rect(input2->pos(), size);
			rect.translate(5.0, -Connector::radius);
			painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, "in2");
			++i;
		}
	}

	{
		int i = 0;

		QSize size(width-5.0, Connector::radius * 2.0);

		{
			QRectF rect(output->pos(), size);
			rect.translate(-width, -Connector::radius);
			painter->drawText(rect, Qt::AlignRight | Qt::AlignVCenter, "out");
			++i;
		}
	}
}

}
