#pragma once

#include <QVector>
#include <QLibrary>

#include <Vector.h>
#include <Matrix.h>

#include "RenderGraph.h"

class Scene;

class RendererWrapper
{
public:

	//
	// Constructor
	RendererWrapper		(void);
	~RendererWrapper	(void);

	bool		loadPlugins			(const char * szPluginsFile);

	//
	// ...
	bool		init				(void);
	bool		release				(void);

	//
	// ...
	bool		onInit				(void);
	bool		onRelease			(void);

	//
	// ...
	void		onReady				(void);
	void		onResize			(unsigned int width, unsigned int height);
	void		onUpdate			(const mat4x4 & mView);

	//
	// Render Graph management
	RenderGraph::Instance *	createRenderGraph(const Graph & graph);
	void destroyRenderGraph(RenderGraph::Instance * pInstance) const;

	unsigned int /*GLuint*/ getRenderTexture(RenderGraph::Instance * pInstance, const char * name) const;
	void setConstant(RenderGraph::Instance * pInstance, const char * name, unsigned int value) const;
	void setConstant(RenderGraph::Instance * pInstance, const char * name, int value) const;
	void setConstant(RenderGraph::Instance * pInstance, const char * name, float value) const;

	//
	// Scene management
	Scene &			getScene			(void);
	const Scene &	getScene			(void) const;
	bool			importToScene		(const char * szFilename);

	//
	// ...
	QVector<QString> GetNodesDirectories(void) const
	{
		return(m_aNodeDirectories);
	}

protected:

	// ...

private:

	bool loadPlugin(const QString & name, const QString & lib, const QString & nodesDirectory, const QString & shaders);

public:

	// ...

protected:

	RenderGraph::Factory m_factory;

	Scene * m_pScene;

private:

	typedef bool (*renderer_onLoad_function)(const char * baseDirectory);

	typedef bool (*renderer_onInit_function)(Scene & scene, RenderGraph::Factory & factory);
	typedef bool (*renderer_onRelease_function)(void);

	typedef void (*renderer_onReady_function)(void);
	typedef void (*renderer_onResize_function)(unsigned int width, unsigned int height);
	typedef void (*renderer_onUpdate_function)(const mat4x4 & matView);

	struct Plugin
	{
		Plugin(const QString & strName, const QString & strLibPath) : name(strName), library(strLibPath)
		{
			OnInit = nullptr;
			OnRelease = nullptr;

			OnReady = nullptr;
			OnResize = nullptr;
			OnUpdate = nullptr;
		}

		QString name;
		QLibrary library;

		renderer_onInit_function OnInit;
		renderer_onRelease_function OnRelease;

		renderer_onReady_function OnReady;
		renderer_onResize_function OnResize;
		renderer_onUpdate_function OnUpdate;
	};

	QVector<Plugin*> m_aPlugins;

	QVector<QString> m_aNodeDirectories;

	unsigned int m_iWidth;
	unsigned int m_iHeight;

	std::map<std::string, unsigned int> mapTextures;
	std::map<std::string, std::vector<unsigned int>> mapValues;
};
