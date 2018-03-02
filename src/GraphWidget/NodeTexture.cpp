#include "NodeTexture.h"

#include "ConnectorInput.h"
#include "ConnectorOutput.h"

#include <QPainter>
#include <QtOpenGL>

#include <qopenglfunctions_3_0.h>
#include <qopenglfunctions_3_1.h>
#include <qopenglfunctions_3_2_compatibility.h>

namespace GraphWidget
{

const qreal NodeTexture::width = 256.0;
const qreal NodeTexture::height = 144.0;

QOpenGLShaderProgram * NodeTexture::shader = nullptr;
QOpenGLBuffer * NodeTexture::vertexPositionBuffer = nullptr;
QOpenGLBuffer * NodeTexture::vertexTexCoordsBuffer = nullptr;

/**
 *
 */
bool NodeTexture::initResources(void)
{
	QOpenGLContext * pContext = QOpenGLContext::currentContext();

	QSurfaceFormat format = pContext->format();

	QPair<int, int> version = format.version();
	QSurfaceFormat::OpenGLContextProfile profile = format.profile();
	
	if (version.first < 3)
	{
		// unsupported
	}
	else if (version.first == 3 && version.second == 0)
	{
		// no resources to create in this case
	}
	else if (version.first == 3 && version.second == 1)
	{
		// same as 3.0 ?
	}
	else // 3.2+
	{
		// ensure this is a compatibility profile so we get the correct functions
		if (profile == QSurfaceFormat::CompatibilityProfile)
		{
			QOpenGLFunctions_3_2_Compatibility * glFuncs = pContext->versionFunctions<QOpenGLFunctions_3_2_Compatibility>();

			if (vertexPositionBuffer == nullptr)
			{
				const float vertexPositions[] =
				{
					0.0f, 0.0f,
					0.0f, 1.0f,
					1.0f, 0.0f,
					1.0f, 1.0f
				};

				vertexPositionBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
				vertexPositionBuffer->create();
				vertexPositionBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
				vertexPositionBuffer->bind();
				vertexPositionBuffer->allocate(vertexPositions, 8 * sizeof(float));
			}

			if (vertexTexCoordsBuffer == nullptr)
			{
				const float vertexTexCoords[] =
				{
					0.0f, 1.0f,
					0.0f, 0.0f,
					1.0f, 1.0f,
					1.0f, 0.0f
				};

				vertexTexCoordsBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
				vertexTexCoordsBuffer->create();
				vertexTexCoordsBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
				vertexTexCoordsBuffer->bind();
				vertexTexCoordsBuffer->allocate(vertexTexCoords, 8 * sizeof(float));
			}

			if (shader == nullptr)
			{
				shader = new QOpenGLShaderProgram();
				
				shader->addShaderFromSourceCode(QOpenGLShader::Vertex,
					"#version 150\n"
					"uniform mat4x4 transform;"
					"in vec2 position;\n"
					"in vec2 color;\n"
					"out vec2 fragColor;\n"
					"void main() {\n"
					"    fragColor = color;\n"
					"    gl_Position = transform * vec4(position, 0.0, 1.0);\n"
					"}\n"
				);
				
				shader->addShaderFromSourceCode(QOpenGLShader::Fragment,
					"#version 150\n"
					"in vec2 fragColor;\n"
					"out vec4 color;\n"
					"void main() {\n"
					"    color = vec4(fragColor.xy, 0.0, 1.0);\n"
					"}\n"
				);

				shader->link();

				shader->bind();

				vertexPositionBuffer->bind();
				shader->enableAttributeArray("position");
				shader->setAttributeBuffer("position", GL_FLOAT, 0, 2);
				vertexPositionBuffer->release();

				vertexTexCoordsBuffer->bind();
				shader->enableAttributeArray("color");
				shader->setAttributeBuffer("color", GL_FLOAT, 0, 2);
				vertexTexCoordsBuffer->release();

				shader->release();
			}
		}
	}

	return(true);
}

/**
 *
 */
bool NodeTexture::releaseResources(void)
{
	return(true);
}

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
	const double border = 1.0;
	const QRectF rect = QRectF(0.0, 0.0, width, height);

	QOpenGLContext * pContext = QOpenGLContext::currentContext();
	
	QSurfaceFormat format = pContext->format();

	QPair<int,int> version = format.version();
	QSurfaceFormat::OpenGLContextProfile profile = format.profile();

	// it seems that Qt currently asks for a CompatibilityProfile context internally
	// this introduces some weird cases that we need to handle
	// we have different cases to handle to ensure maximum compatibility

	// check version first
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
	else if (version.first == 3 && version.second == 1)
	{
		QOpenGLFunctions_3_1 * glFuncs = pContext->versionFunctions<QOpenGLFunctions_3_1>();

		if (glFuncs)
		{
			// TODO : same as 3.0 ? I can't test it ...
		}
	}
	else // 3.2+
	{
		// ensure we have a CompatibilityProfile, just in case Qt asks for a CoreProfile in the future
		if (profile == QSurfaceFormat::CompatibilityProfile)
		{
			QOpenGLFunctions_3_2_Compatibility * glFuncs = pContext->versionFunctions<QOpenGLFunctions_3_2_Compatibility>();

			if (glFuncs)
			{
				initResources();

				shader->bind();
				
				vertexPositionBuffer->bind();
				shader->enableAttributeArray("position");
				shader->setAttributeBuffer("position", GL_FLOAT, 0, 2);

				vertexTexCoordsBuffer->bind();
				shader->enableAttributeArray("color");
				shader->setAttributeBuffer("color", GL_FLOAT, 0, 2);
				
				QGraphicsView * v = scene()->views().first(); // FIXME : need a better way to get the view

				const QPointF sceneP1 = this->mapToScene(rect.topLeft());
				const QPoint viewP1 = v->mapFromScene(sceneP1);
				const QPoint pos1 = v->viewport()->mapToParent(viewP1);

				const QPointF sceneP2 = this->mapToScene(rect.bottomRight());
				const QPoint viewP2 = v->mapFromScene(sceneP2);
				const QPoint pos2 = v->viewport()->mapToParent(viewP2);

				QMatrix4x4 projection;
				projection.ortho(v->rect());

				QMatrix4x4 translation;
				translation.translate(pos1.x(), pos1.y());

				QMatrix4x4 scale;
				scale.scale(pos2.x() - pos1.x(), pos2.y() - pos1.y());

				const QMatrix4x4 transform = projection * translation * scale;
				shader->setUniformValue("transform", transform);

				glFuncs->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				
				vertexPositionBuffer->release();
				vertexTexCoordsBuffer->release();

				shader->release();
			}
		}
	}

	painter->endNativePainting();

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
