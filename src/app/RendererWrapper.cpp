#include "RendererWrapper.h"

#include "Scene.h"

#include <QDir>
#include <QMessageBox>
#include <QMouseEvent>

#include <QGLWidget> // for convertToGLFormat

#include <QtXml/QtXml>

#include "QOpenGLResourceManager.h"

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
		QString dir = element.attribute("dir", "");

		if (!loadPlugin(name, lib, nodes, dir))
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
			if (!plugin->OnInit(*m_pScene, m_factory))
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
bool RendererWrapper::loadPlugin(const QString & name, const QString & lib, const QString & nodesDirectory, const QString & baseDirectory)
{
	Plugin * plugin = new Plugin(name, lib); // QLibrary copy-constructor is private

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

	const std::string strBaseDir = baseDirectory.toStdString();

	if (!OnLoad(strBaseDir.data()))
	{
		plugin->library.unload();
		return(false);
	}

	plugin->OnInit = (renderer_onInit_function)plugin->library.resolve("renderer_onInit");
	plugin->OnRelease = (renderer_onRelease_function)plugin->library.resolve("renderer_onRelease");

	plugin->OnReady = (renderer_onReady_function)plugin->library.resolve("renderer_onReady");
	plugin->OnResize = (renderer_onResize_function)plugin->library.resolve("renderer_onResize");
	plugin->OnUpdate = (renderer_onUpdate_function)plugin->library.resolve("renderer_onUpdate");

	m_aPlugins.append(plugin);

	m_aNodeDirectories.append(nodesDirectory);

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
	m_iWidth = width;
	m_iHeight = height;

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
 * @brief RendererWrapper::createRenderGraph
 * @return
 */
RenderGraph::Instance *	RendererWrapper::createRenderGraph(const Graph & graph)
{
	return m_factory.createInstanceFromGraph(graph, mapTextures, mapValues);
}

/**
 * @brief RendererWrapper::destroyRenderGraph
 */
void RendererWrapper::destroyRenderGraph(RenderGraph::Instance * pInstance) const
{
	m_factory.destroyInstance(pInstance);
}

/**
 * @brief RendererWrapper::getRenderTexture
 * @param pInstance
 * @return
 */
unsigned int /*GLuint*/ RendererWrapper::getRenderTexture(RenderGraph::Instance * pInstance, const char * name) const
{
	auto it = mapTextures.find(name);

	if (it != mapTextures.end())
	{
		return pInstance->getRenderTexture(it->second);
	}

	return 0;
}

/**
 * @brief RendererWrapper::setConstant
 * @param pInstance
 * @param name
 * @param value
 */
void RendererWrapper::setConstant(RenderGraph::Instance * pInstance, const char * name, unsigned int value) const
{
	auto it = mapValues.find(name);

	if (it != mapValues.end())
	{
		pInstance->setConstant(it->second[0], value); // FIXME
	}
}

/**
 * @brief RendererWrapper::setConstant
 * @param pInstance
 * @param name
 * @param value
 */
void RendererWrapper::setConstant(RenderGraph::Instance * pInstance, const char * name, int value) const
{
	auto it = mapValues.find(name);

	if (it != mapValues.end())
	{
		pInstance->setConstant(it->second[0], value); // FIXME
	}
}

/**
 * @brief RendererWrapper::setConstant
 * @param pInstance
 * @param name
 * @param value
 */
void RendererWrapper::setConstant(RenderGraph::Instance * pInstance, const char * name, float value) const
{
	auto it = mapValues.find(name);

	if (it != mapValues.end())
	{
		pInstance->setConstant(it->second[0], value); // FIXME
	}
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
