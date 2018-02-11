#include "RendererWrapper.h"

#include "Scene.h"

#include <QDir>
#include <QMessageBox>
#include <QMouseEvent>

#include <QGLWidget> // for convertToGLFormat

#ifndef RENDERER_LIB
#	error "RENDERER_LIB must be defined"
#endif // RENDERER_LIB

/**
 * @brief RendererWrapper::RendererWrapper
 */
RendererWrapper::RendererWrapper(void) : m_rendererLibrary(RENDERER_LIB)
{
	m_pScene = new Scene();

	//
	// Load renderer library
	if (!m_rendererLibrary.load())
	{
		return;
	}

	renderer_onLoad_function OnLoad = (renderer_onLoad_function)m_rendererLibrary.resolve("renderer_onLoad");

	if (OnLoad == nullptr)
	{
		m_rendererLibrary.unload();
		return;
	}

	if (!OnLoad())
	{
		m_rendererLibrary.unload();
		return;
	}

	m_pOnInit = (renderer_onInit_function)m_rendererLibrary.resolve("renderer_onInit");
	m_pOnRelease = (renderer_onRelease_function)m_rendererLibrary.resolve("renderer_onRelease");

	m_pOnReady = (renderer_onReady_function)m_rendererLibrary.resolve("renderer_onReady");
	m_pOnResize = (renderer_onResize_function)m_rendererLibrary.resolve("renderer_onResize");
	m_pOnUpdate = (renderer_onUpdate_function)m_rendererLibrary.resolve("renderer_onUpdate");

	m_pInitQueue = (renderer_initQueue_function)m_rendererLibrary.resolve("renderer_initQueue");
	m_pReleaseQueue = (renderer_releaseQueue_function)m_rendererLibrary.resolve("renderer_releaseQueue");

	m_pGetRenderTexture = (renderer_getRenderTexture_function)m_rendererLibrary.resolve("renderer_getRenderTexture");
}

/**
 * @brief RendererWrapper::~RendererWrapper
 */
RendererWrapper::~RendererWrapper(void)
{
	m_rendererLibrary.unload();
	delete m_pScene;
}

/**
 * @brief RendererWrapper::init
 * @return
 */
bool RendererWrapper::init(void)
{
	if (m_pOnInit)
	{
		return(m_pOnInit(*m_pScene));
	}

	return(false);
}

/**
 * @brief RendererWrapper::release
 * @return
 */
bool RendererWrapper::release(void)
{
	if (m_pOnRelease)
	{
		return(m_pOnRelease());
	}

	return(false);
}

/**
 * @brief RendererWrapper::onReady
 */
void RendererWrapper::onReady(void)
{
	if (m_pOnReady)
	{
		m_pOnReady();
	}
}

/**
 * @brief RendererWrapper::onResize
 * @param width
 * @param height
 */
void RendererWrapper::onResize(unsigned int width, unsigned int height)
{
	if (m_pOnResize)
	{
		m_pOnResize(width, height);
	}
}

/**
 * @brief RendererWrapper::onUpdate
 * @param mView
 * @param clearColor
 */
void RendererWrapper::onUpdate(const mat4x4 & mView)
{
	if (m_pOnUpdate)
	{
		m_pOnUpdate(mView);
	}
}

/**
 * @brief RendererWrapper::initQueue
 * @param szFilename
 */
void RendererWrapper::initQueue(const char * szFilename)
{
	if (m_pInitQueue)
	{
		m_pInitQueue(szFilename);
	}
}

/**
 * @brief RendererWrapper::releaseQueue
 */
void RendererWrapper::releaseQueue(void)
{
	if (m_pReleaseQueue)
	{
		m_pReleaseQueue();
	}
}

/**
 * @brief RendererWrapper::GetRenderTexture
 * @param name
 * @return
 */
GLuint RendererWrapper::GetRenderTexture(const char * name) const
{
	if (m_pGetRenderTexture)
	{
		return(m_pGetRenderTexture(name));
	}

	return(0);
}

/**
 * @brief RendererWrapper::getScene
 * @return
 */
Scene & RendererWrapper::getScene(void)
{
	return(*m_pScene);
}

/**
 * @brief RendererWrapper::getScene
 * @return
 */
const Scene & RendererWrapper::getScene(void) const
{
	return(*m_pScene);
}

/**
 * @brief RendererWrapper::importToScene
 * @param szFilename
 * @return
 */
bool RendererWrapper::importToScene(const char * szFilename)
{
	return(m_pScene->import(szFilename));
}
