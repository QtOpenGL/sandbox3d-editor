#include "NodePass.h"

#include "ConnectorInput.h"
#include "ConnectorOutput.h"

#include "View.h"

#include <QGraphicsScene>

#include <QPainter>

namespace GraphWidget
{

const qreal NodePass::width = 200.0;
const qreal NodePass::height = 200.0;

/**
 * @brief Constructor
 */
NodePass::NodePass(void) : Node(), title("unnamed node")
{
	setCacheMode(DeviceCoordinateCache);
}

/**
 * @brief Destructor
 */
NodePass::~NodePass(void)
{
	// ...
}

/**
 * @brief NodePass::setTitle
 * @param strTitle
 */
void NodePass::setTitle(const QString & strTitle)
{
	title = strTitle;
}

/**
 * @brief NodePass::getTitle
 * @return
 */
const QString & NodePass::getTitle(void) const
{
	return(title);
}

/**
 * @brief NodePass::addInput
 * @param strName
 */
void NodePass::addInput(const QString & strName, unsigned int mask)
{
	unsigned int count = inputList.count();

	ConnectorInput * pConnector = new ConnectorInput(this, count);
	pConnector->setPos(0.0, 40.0 + 15.0 + 25.0 * count);
	pConnector->setMask(READ_BIT | mask);
	inputList << pConnector;
	inputNameList << strName;
}

/**
 * @brief NodePass::addOutput
 * @param strName
 */
void NodePass::addOutput(const QString & strName, unsigned int mask)
{
	unsigned int count = outputList.count();

	ConnectorOutput * pConnector = new ConnectorOutput(this, count);
	pConnector->setPos(width, 40.0 + 15.0 + 25.0 * count);
	if (mask == TYPE_TEXTURE_BIT)
	{
		pConnector->setMask(WRITE_BIT | mask);
	}
	else
	{
		pConnector->setMask(READ_BIT | mask);
	}
	outputList << pConnector;
	outputNameList << strName;
}

/**
 * @brief Node::getInputConnector
 * @return
 */
ConnectorInput * NodePass::getInputConnector(unsigned int index)
{
	if (index >= inputList.count())
	{
		return(nullptr);
	}

	return(inputList[index]);
}

/**
 * @brief Node::getOutputConnector
 * @return
 */
ConnectorOutput * NodePass::getOutputConnector(unsigned int index)
{
	if (index >= outputList.count())
	{
		return(nullptr);
	}

	return(outputList[index]);
}

/**
 * @brief NodePass::getWidth
 * @return
 */
qreal NodePass::getWidth(void) const
{
	return(width);
}

/**
 * @brief NodePass::getHeight
 * @return
 */
qreal NodePass::getHeight(void) const
{
	return(height);
}

/**
 * @brief Node::boundingRect
 * @return
 */
QRectF NodePass::boundingRect(void) const
{
	double border = 1.0;
	QRectF rect = QRectF(0.0 - (border * 0.5) - Connector::radius, 0.0 - (border * 0.5) - Connector::radius, width + (border) + (Connector::radius * 2), height + (border) + (Connector::radius * 2));
    return(rect);
}

/**
 * @brief NodePass::shape
 * @return
 */
QPainterPath NodePass::shape(void) const
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
void NodePass::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
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

	painter->drawText(rect2, Qt::AlignCenter, title);

	painter->setPen(QColor(200, 200, 200, 255));

	{
		int i = 0;

		QSize size(width, Connector::radius * 2.0);

		for (const QString & strName : inputNameList)
		{
			QRectF rect(inputList[i]->pos(), size);
			rect.translate(5.0, -Connector::radius);
			painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, strName);
			++i;
		}
	}

	{
		int i = 0;

		QSize size(width-5.0, Connector::radius * 2.0);

		for (const QString & strName : outputNameList)
		{
			QRectF rect(outputList[i]->pos(), size);
			rect.translate(-width, -Connector::radius);
			painter->drawText(rect, Qt::AlignRight | Qt::AlignVCenter, strName);
			++i;
		}
	}
}

}
