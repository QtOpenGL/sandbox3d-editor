#include "DrawableSurface.h"

#include "RendererWrapper.h"

#include "MainWindow.h"

#include <QMouseEvent>

#include "QOpenGLResourceManager.h"

#include <assert.h>

#define ENABLE_PICKBUFFER 0

extern RendererWrapper g_RendererWrapper;

/**
 * @brief Constructor
 * @param parent
 */
DrawableSurface::DrawableSurface(QWidget *parent)
: QOpenGLWidget(parent)
, m_camera()
, m_vLastPos(0, 0)
, m_pSelectedObject(nullptr)
, m_uCurrentTexture(0)
{
	setAutoFillBackground(false);
	setFocusPolicy(Qt::StrongFocus);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
	connect(this, &QOpenGLWidget::resized, this, &DrawableSurface::onResized);
	connect(this, &QOpenGLWidget::frameSwapped, this, &DrawableSurface::onFrameSwapped);
#endif // (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
}

/**
 * @brief Destructor
 */
DrawableSurface::~DrawableSurface(void)
{
	// ...
}

/**
 * @brief DrawableSurface::ResetCamera
 */
void DrawableSurface::ResetCamera(void)
{
	m_camera = Camera();
	update();
}

/**
 * @brief DrawableSurface::setCurrentTexture
 * @param strFinalTextureId
 */
void DrawableSurface::setCurrentTexture(const std::string & strFinalTextureId)
{
	m_uCurrentTexture = g_RendererWrapper.GetRenderTexture(strFinalTextureId.c_str());
}

/**
 * @brief initializeGL
 */
void DrawableSurface::initializeGL(void)
{
	QOpenGLFunctions_3_2_Core::initializeOpenGLFunctions();
	QOpenGLResourceManager::instance().init();

	GLint v;
	glGetIntegerv(GL_CONTEXT_FLAGS, &v);
	//assert (v & GL_CONTEXT_FLAG_DEBUG_BIT);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	g_RendererWrapper.onReady();

	g_RendererWrapper.initQueue("/tmp/render.xml");
}

/**
 * @brief drawable::resizeGL
 * @param w
 * @param h
 */
void DrawableSurface::resizeGL(int w, int h)
{
	g_RendererWrapper.onResize(w, h);

	QOpenGLResourceManager & manager = QOpenGLResourceManager::instance();
	manager.onResize(QSize(w, h));
}

/**
 * @brief drawable::paintGL
 */
void DrawableSurface::paintGL(void)
{
	const mat4x4 & matView = m_camera.getViewMatrix();

	g_RendererWrapper.onUpdate(matView);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	QOpenGLResourceManager & manager = QOpenGLResourceManager::instance();

#if ENABLE_PICKBUFFER
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, manager.pickBufferFramebufferObject());

	{
		unsigned int i = 0;

		for (const Object & object : g_RendererWrapper.getScene().getObjects())
		{
			object.mesh->bind();

//			SetUniform(m_pickBufferPipeline.m_uShaderObject, "Model", object.transformation);

			glVertexAttribI1ui(5, i);

			for (SubMesh * m : object.getDrawCommands())
			{
				m->drawGL();
			}

			object.mesh->unbind();

			++i;
		}
	}
#endif // ENABLE_PICKBUFFER

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, defaultFramebufferObject());

	if (0 != m_uCurrentTexture)
	{
		manager.bindQuadResources();

		glBindTexture(GL_TEXTURE_2D, m_uCurrentTexture);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		manager.unbindQuadResources();
	}

#if ENABLE_PICKBUFFER
	if (nullptr != m_pSelectedObject)
	{
		Mesh::BoundingBox bbox = m_pSelectedObject->mesh->m_BoundingBox;

		mat4x4 mMVP = (g_RendererWrapper.getProjection() * (matView * m_pSelectedObject->transformation));

		QVector3D BBoxMin(bbox.min.x, bbox.min.y, bbox.min.z);
		QVector3D BBoxMax(bbox.max.x, bbox.max.y, bbox.max.z);

		manager.bindBboxResources(QMatrix4x4((float*)&mMVP), BBoxMin, BBoxMax);

		glDrawArrays(GL_POINTS, 0, 1);

		manager.unbindBboxResources();
	}
#endif // ENABLE_PICKBUFFER
}

/**
 * @brief DrawableSurface::onFrameSwapped
 */
void DrawableSurface::onFrameSwapped(void)
{
	//update();
}

/**
 * @brief DrawableSurface::onResized
 */
void DrawableSurface::onResized(void)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
	g_RendererWrapper.SetDefaultFramebuffer(defaultFramebufferObject());
#endif // (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
}

/**
 * @brief drawable::mousePressEvent
 * @param event
 */
void DrawableSurface::mousePressEvent(QMouseEvent * event)
{
	ivec2 pos(event->x(), event->y());
	m_vLastPos = pos;
	m_bMoved = false;
	update();
}

/**
 * @brief drawable::mouseMoveEvent
 * @param event
 */
void DrawableSurface::mouseMoveEvent(QMouseEvent * event)
{
	ivec2 pos(event->x(), event->y());

	//if (event->button() & Qt::LeftButton)
	{
		ivec2 diff = pos - m_vLastPos;

		if (event->modifiers() & Qt::ControlModifier)
		{
			// Translate X/Y
			vec2 translation(diff.x / 100.0f, diff.y / -100.0f);
			m_camera.TranslateXY(translation);
		}
		else
		{
			vec2 rotation(diff.x / -500.0f, diff.y / -500.0f);
			m_camera.Rotate(rotation);
		}
	}

	m_vLastPos = pos;
	m_bMoved = true;

	update();
}

/**
 * @brief drawable::mouseReleaseEvent
 * @param event
 */
void DrawableSurface::mouseReleaseEvent(QMouseEvent * event)
{
	ivec2 pos(event->x(), event->y());

	if (!m_bMoved)
	{

		m_pSelectedObject = getObjectAtPos(pos);
	}

	update();
}

/**
 * @brief DrawableSurface::getObjectAtPos
 * @param pos
 * @return
 */
Object * DrawableSurface::getObjectAtPos(const ivec2 & pos)
{
	Object * object = nullptr;

#if ENABLE_PICKBUFFER
	QOpenGLResourceManager & manager = QOpenGLResourceManager::instance();

	glBindFramebuffer(GL_READ_FRAMEBUFFER, manager.pickBufferFramebufferObject());
	glReadBuffer(GL_COLOR_ATTACHMENT0);

	unsigned int id = UINT32_MAX;

	glReadPixels(pos.x, height() - pos.y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &id);

	if (id < g_RendererWrapper.getScene().getObjectCount())
	{
		object = (Object*)&(g_RendererWrapper.getScene().getObjects()[id]); // ugly
	}

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glReadBuffer(GL_BACK);
#endif // ENABLE_PICKBUFFER

	return(object);
}

/**
 * @brief wheelEvent
 * @param event
 */
void DrawableSurface::wheelEvent(QWheelEvent * event)
{
	float translation = event->delta() / -100.0f;
	m_camera.TranslateZ(translation);
	update();
}

/**
 * @brief DrawableSurface::keyPressEvent
 * @param event
 */
void DrawableSurface::keyPressEvent(QKeyEvent *event)
{

}

/**
 * @brief DrawableSurface::keyReleaseEvent
 * @param event
 */
void DrawableSurface::keyReleaseEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Delete)
	{
#if ENABLE_PICKBUFFER
		if (nullptr != m_pSelectedObject)
		{
			g_RendererWrapper.getScene().remove(*m_pSelectedObject);
			m_pSelectedObject = nullptr;
			update();
		}
#endif // ENABLE_PICKBUFFER
	}
}

/**
 * @brief DrawableSurface::importScene
 * @param filename
 */
void DrawableSurface::importScene(const QString & filename)
{
	makeCurrent();

	g_RendererWrapper.importToScene(filename.toLatin1().data());
}
