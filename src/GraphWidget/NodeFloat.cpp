#include "NodeFloat.h"

#include "ConnectorInput.h"
#include "ConnectorOutput.h"

#include <QLineEdit>
#include <QGraphicsProxyWidget>
#include <QDoubleValidator>

#include <QPainter>

namespace GraphWidget
{

const qreal NodeFloat::width = 120.0;
const qreal NodeFloat::height = 45.0f;
const int NodeFloat::padding = 10;

/**
 * @brief Constructor
 */
NodeFloat::NodeFloat(float defaultValue) : Node(), value(defaultValue), output(nullptr)
{
	setCacheMode(DeviceCoordinateCache);

	QLineEdit * pLineEdit = new QLineEdit(QString::number(defaultValue));
	pLineEdit->resize(width - (padding * 2), height - (padding * 2));
	pLineEdit->setAlignment(Qt::AlignRight);
	pLineEdit->setValidator(new QDoubleValidator(this));

	QGraphicsProxyWidget * pMyProxy = new QGraphicsProxyWidget(this);
	pMyProxy->setWidget(pLineEdit);
	pMyProxy->setPos(padding, padding);

	QObject::connect(pLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(updateValue(const QString &)));

	output = new ConnectorOutput(this, 0);
	output->setPos(width, 25.0f);
	output->setMask(READ_BIT | TYPE_FLOAT_BIT);
}

/**
 * @brief Destructor
 */
NodeFloat::~NodeFloat(void)
{
	// ...
}

/**
 * @brief Node::getInputConnector
 * @return
 */
ConnectorInput * NodeFloat::getInputConnector(unsigned int index)
{
	return(nullptr);
}

/**
 * @brief Node::getOutputConnector
 * @return
 */
ConnectorOutput * NodeFloat::getOutputConnector(unsigned int index)
{
	return(output);
}

/**
 * @brief NodeFloat::getWidth
 * @return
 */
qreal NodeFloat::getWidth(void) const
{
	return(width);
}

/**
 * @brief NodeFloat::getHeight
 * @return
 */
qreal NodeFloat::getHeight(void) const
{
	return(height);
}

/**
 * @brief Destructor
 * @return
 */
QRectF NodeFloat::boundingRect(void) const
{
	double border = 1.0;
	QRectF rect = QRectF(0.0 - (border * 0.5) - Connector::radius, 0.0 - (border * 0.5) - Connector::radius, width + (border) + (Connector::radius * 2), height + (border) + (Connector::radius * 2));
	return(rect);
}

/**
 * @brief NodeFloat::shape
 * @return
 */
QPainterPath NodeFloat::shape(void) const
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
void NodeFloat::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
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
}

/**
 * @brief NodeFloat::updateValue
 * @param strText
 */
void NodeFloat::updateValue(const QString & strText)
{
	value = strText.toFloat();
}

}
