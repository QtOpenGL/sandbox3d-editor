#include "NodeTexture.h"

#include "ConnectorInput.h"
#include "ConnectorOutput.h"

#include <QPainter>
#include <QtOpenGL>

#include <QOpenGLFunctions_3_0>

namespace GraphWidget
{

const qreal NodeTexture::width = 256.0;
const qreal NodeTexture::height = 144.0;

/**
 * @brief Constructor
 */
NodeTexture::NodeTexture(void) : Node(), texture(0), output(nullptr)
{
    setCacheMode(NoCache);

	ConnectorInput * input = new ConnectorInput(this, 0);
	input->setPos(0.0f, 25.0f);
	input->setMask(WRITE_BIT | TYPE_TEXTURE_BIT);
	inputList << input;

	output = new ConnectorOutput(this, 0);
	output->setPos(width, 25.0f);
	output->setMask(READ_BIT | TYPE_TEXTURE_BIT);
}

/**
 * @brief Destructor
 */
NodeTexture::~NodeTexture(void)
{
	// ...
}

/**
 * @brief Node::getInputConnector
 * @return
 */
ConnectorInput * NodeTexture::getInputConnector(unsigned int index)
{
	return(inputList[index]);
}

/**
 * @brief Node::getOutputConnector
 * @return
 */
ConnectorOutput * NodeTexture::getOutputConnector(unsigned int index)
{
	return(output);
}

/**
 * @brief NodeTexture::getWidth
 * @return
 */
qreal NodeTexture::getWidth(void) const
{
	return(width);
}

/**
 * @brief NodeTexture::getHeight
 * @return
 */
qreal NodeTexture::getHeight(void) const
{
	return(height);
}

/**
 * @brief Destructor
 * @return
 */
QRectF NodeTexture::boundingRect(void) const
{
	double border = 1.0;
	QRectF rect = QRectF(0.0 - (border * 0.5) - Connector::radius, 0.0 - (border * 0.5) - Connector::radius, width + (border) + (Connector::radius * 2), height + (border) + (Connector::radius * 2));
	return(rect);
}

/**
 * @brief NodeTexture::shape
 * @return
 */
QPainterPath NodeTexture::shape(void) const
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
void NodeTexture::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	QOpenGLContext * pContext = QOpenGLContext::currentContext();
	
	QSurfaceFormat format = pContext->format();

	QPair<int,int> version = format.version();
	QSurfaceFormat::OpenGLContextProfile profile = format.profile();

	// it seems that Qt currently uses a CompatibilityProfile internally
	// not sure why ... but just ensure it, in case this changes in the future
	if (profile == QSurfaceFormat::CompatibilityProfile)
	{
		if (version.first < 3)
		{
			// unsupported
		}
		else if (version.first == 3 && version.second == 0)
		{
			// Historically, Mesa always returns a 3.0 context when asking a CompatibilityProfile
			// This is about to change (February 2018) but just handle OpenGL 3.0 context correctly
			QOpenGLFunctions_3_0 * glFuncs = pContext->versionFunctions<QOpenGLFunctions_3_0>();

			if (glFuncs)
			{
				glFuncs->glEnable(GL_TEXTURE_2D);
				glFuncs->glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
				glFuncs->glBindTexture(GL_TEXTURE_2D, texture);
				glFuncs->glBegin(GL_QUADS);
				glFuncs->glTexCoord2d(0.0, 1.0); glFuncs->glVertex2d(0.0, 0.0);
				glFuncs->glTexCoord2d(1.0, 1.0); glFuncs->glVertex2d(0.0 + width, 0.0);
				glFuncs->glTexCoord2d(1.0, 0.0); glFuncs->glVertex2d(0.0 + width, 0.0 + height);
				glFuncs->glTexCoord2d(0.0, 0.0); glFuncs->glVertex2d(0.0, 0.0 + height);
				glFuncs->glEnd();
			}
		}
		else
		{
			// TODO
		}
	}

	painter->endNativePainting();

	double border = 1.0;
	QRectF rect = QRectF(0.0, 0.0, width, height);

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
	painter->drawRect(rect);
}

}
