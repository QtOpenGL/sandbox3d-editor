#include "RendererWrapper.h"

#define HAVE_OPENGL 1
#define HAVE_VULKAN 0
#define HAVE_GL_GLCOREARB_H 1

#include "Rendering/Rendering.h"

static Rendering * g_pRendering = nullptr; // ugly but needed for now

/**
 * @brief RendererWrapper::RendererWrapper
 */
RendererWrapper::RendererWrapper(void)
{
	// ...
}

/**
 * @brief RendererWrapper::~RendererWrapper
 */
RendererWrapper::~RendererWrapper(void)
{
	// ...
}

/**
 * @brief RendererWrapper::init
 * @return
 */
bool RendererWrapper::init(void)
{
	g_pRendering = new Rendering();
	return(true);
}

/**
 * @brief RendererWrapper::release
 * @return
 */
bool RendererWrapper::release(void)
{
	delete g_pRendering;
	g_pRendering = nullptr;
	return(true);
}

/**
 * @brief RendererWrapper::onReady
 */
void RendererWrapper::onReady(void)
{
	g_pRendering->onReady();
}

/**
 * @brief RendererWrapper::onResize
 * @param width
 * @param height
 */
void RendererWrapper::onResize(int width, int height)
{
	g_pRendering->onResize(width, height);
}

/**
 * @brief RendererWrapper::onUpdate
 * @param mView
 * @param clearColor
 */
void RendererWrapper::onUpdate(const mat4x4 & mView, const vec4 & clearColor)
{
	g_pRendering->onUpdate(mView, clearColor);
}

/**
 * @brief RendererWrapper::initQueue
 * @param szFilename
 */
void RendererWrapper::initQueue(const char * szFilename)
{
	g_pRendering->initQueue(szFilename);
}

/**
 * @brief RendererWrapper::releaseQueue
 */
void RendererWrapper::releaseQueue(void)
{
	g_pRendering->releaseQueue();
}

/**
 * @brief RendererWrapper::GetRenderTexture
 * @param name
 * @return
 */
GLuint RendererWrapper::GetRenderTexture(const char * name) const
{
	const GPU::Texture<GL_TEXTURE_2D> * pRenderTexture = g_pRendering->GetRenderTexture(name);

	if (nullptr != pRenderTexture)
	{
		return(pRenderTexture->GetObject());
	}

	return(0);
}

/**
 * @brief RendererWrapper::SetDefaultFramebuffer
 * @param framebuffer
 */
void RendererWrapper::SetDefaultFramebuffer(GLuint framebuffer)
{
	g_pRendering->SetDefaultFramebuffer(framebuffer);
}

/**
 * @brief RendererWrapper::SetPickBufferFramebuffer
 * @param framebuffer
 */
void RendererWrapper::SetPickBufferFramebuffer(GLuint framebuffer)
{
	g_pRendering->SetPickBufferFramebuffer(framebuffer);
}

/**
 * @brief RendererWrapper::getScene
 * @return
 */
Scene & RendererWrapper::getScene(void)
{
	return(g_pRendering->m_scene);
}

/**
 * @brief RendererWrapper::getProjection
 * @return
 */
const mat4x4 & RendererWrapper::getProjection(void) const
{
	return(g_pRendering->m_matProjection);
}
