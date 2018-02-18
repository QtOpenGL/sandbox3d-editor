#include "RendererWrapper.h"

#include "Scene.h"

#include <QDir>
#include <QMessageBox>
#include <QMouseEvent>

#include <QGLWidget> // for convertToGLFormat

#include <QtXml/QtXml>

/**
 * @brief RendererWrapper::RendererWrapper
 */
RendererWrapper::RendererWrapper(void)
{
	m_pScene = new Scene();
}

/**
 * @brief RendererWrapper::~RendererWrapper
 */
RendererWrapper::~RendererWrapper(void)
{
	delete m_pScene;
}

/**
 * @brief Load Plugins
 * @param szPluginsFile
 * @return
 */
bool RendererWrapper::loadPlugins(const char * szPluginsFile)
{
	QFile plugins_file(szPluginsFile);

	if (!plugins_file.open(QIODevice::ReadOnly))
	{
		return(false);
	}

	QDomDocument dom("plugins_file");
	bool parse_success = dom.setContent(&plugins_file);
	plugins_file.close();

	if (!parse_success)
	{
		return(false);
	}

	QDomElement rootNode = dom.documentElement();

	QDomNode pluginNode = rootNode.firstChild();

	bool bAllPluginsLoaded = true;

	while (!pluginNode.isNull())
	{
		QDomElement element = pluginNode.toElement();

		QString name = element.attribute("name", "unnamed plugin");
		QString lib = element.attribute("lib", "");
		QString nodes = element.attribute("nodes", "");
		QString shaders = element.attribute("shaders", "");

		if (!loadPlugin(name, lib, nodes, shaders))
		{
			bAllPluginsLoaded = false;
		}

		pluginNode = pluginNode.nextSibling();
	}

	return(bAllPluginsLoaded);
}

/**
 * @brief RendererWrapper::init
 * @return
 */
bool RendererWrapper::init(void)
{
	bool bSuccess = true;

	for (Plugin * plugin : m_aPlugins)
	{
		if (plugin->OnInit)
		{
			if (!plugin->OnInit(*m_pScene))
			{
				bSuccess = false;
			}
		}
	}

	return(bSuccess);
}

/**
 * @brief RendererWrapper::release
 * @return
 */
bool RendererWrapper::release(void)
{
	bool bSuccess = true;

	for (Plugin * plugin : m_aPlugins)
	{
		if (plugin->OnRelease)
		{
			if (!plugin->OnRelease())
			{
				bSuccess = false;
			}
		}
	}

	return(bSuccess);
}

/**
 * @brief RendererWrapper::loadPlugin
 * @param name
 * @param lib
 * @param nodes
 * @param shaders
 * @return
 */
bool RendererWrapper::loadPlugin(const QString & name, const QString & lib, const QString & nodes, const QString & shaders)
{
	Plugin * plugin = new Plugin(lib); // QLibrary copy-constructor is private

	//
	// Load renderer library
	if (!plugin->library.load())
	{
		return(false);
	}

	renderer_onLoad_function OnLoad = (renderer_onLoad_function)plugin->library.resolve("renderer_onLoad");

	if (OnLoad == nullptr)
	{
		plugin->library.unload();
		return(false);
	}

	if (!OnLoad())
	{
		plugin->library.unload();
		return(false);
	}

	plugin->OnInit = (renderer_onInit_function)plugin->library.resolve("renderer_onInit");
	plugin->OnRelease = (renderer_onRelease_function)plugin->library.resolve("renderer_onRelease");

	plugin->OnReady = (renderer_onReady_function)plugin->library.resolve("renderer_onReady");
	plugin->OnResize = (renderer_onResize_function)plugin->library.resolve("renderer_onResize");
	plugin->OnUpdate = (renderer_onUpdate_function)plugin->library.resolve("renderer_onUpdate");

	plugin->InitQueue = (renderer_initQueue_function)plugin->library.resolve("renderer_initQueue");
	plugin->ReleaseQueue = (renderer_releaseQueue_function)plugin->library.resolve("renderer_releaseQueue");

	plugin->GetRenderTexture = (renderer_getRenderTexture_function)plugin->library.resolve("renderer_getRenderTexture");

	m_aPlugins.append(plugin);

	return(true);
}


/**
 * @brief RendererWrapper::onReady
 */
void RendererWrapper::onReady(void)
{
	for (Plugin * plugin : m_aPlugins)
	{
		if (plugin->OnReady)
		{
			plugin->OnReady();
		}
	}
}

/**
 * @brief RendererWrapper::onResize
 * @param width
 * @param height
 */
void RendererWrapper::onResize(unsigned int width, unsigned int height)
{
	for (Plugin * plugin : m_aPlugins)
	{
		if (plugin->OnResize)
		{
			plugin->OnResize(width, height);
		}
	}
}

/**
 * @brief RendererWrapper::onUpdate
 * @param mView
 * @param clearColor
 */
void RendererWrapper::onUpdate(const mat4x4 & mView)
{
	for (Plugin * plugin : m_aPlugins)
	{
		if (plugin->OnUpdate)
		{
			plugin->OnUpdate(mView);
		}
	}
}

/**
 * @brief RendererWrapper::initQueue
 * @param szFilename
 */
void RendererWrapper::initQueue(const char * szFilename)
{
	for (Plugin * plugin : m_aPlugins)
	{
		if (plugin->InitQueue)
		{
			plugin->InitQueue(szFilename);
		}
	}
}

/**
 * @brief RendererWrapper::releaseQueue
 */
void RendererWrapper::releaseQueue(void)
{
	for (Plugin * plugin : m_aPlugins)
	{
		if (plugin->ReleaseQueue)
		{
			plugin->ReleaseQueue();
		}
	}
}

/**
 * @brief RendererWrapper::GetRenderTexture
 * @param name
 * @return
 */
GLuint RendererWrapper::GetRenderTexture(const char * name) const
{
	for (Plugin * plugin : m_aPlugins)
	{
		if (plugin->GetRenderTexture)
		{
			GLuint textureId = plugin->GetRenderTexture(name);

			if (textureId != 0)
			{
				return(textureId);
			}
		}
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
