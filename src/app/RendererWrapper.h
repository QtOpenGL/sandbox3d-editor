#pragma once

#include <QVector>
#include <QLibrary>

#include <Vector.h>
#include <Matrix.h>

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
	// ...
	void		initQueue			(const char * szFilename);
	void		releaseQueue		(void);

	//
	// Scene management
	Scene &			getScene			(void);
	const Scene &	getScene			(void) const;
	bool			importToScene		(const char * szFilename);

	//
	// ...
	unsigned int /*GLuint*/ GetRenderTexture(const char * name) const;

protected:

	// ...

private:

	bool loadPlugin(const QString & name, const QString & lib, const QString & nodes, const QString & shaders);

public:

	// ...

protected:

	Scene * m_pScene;

private:

	typedef bool (*renderer_onLoad_function)(void);

	typedef bool (*renderer_onInit_function)(Scene & scene);
	typedef bool (*renderer_onRelease_function)(void);

	typedef void (*renderer_onReady_function)(void);
	typedef void (*renderer_onResize_function)(unsigned int width, unsigned int height);
	typedef void (*renderer_onUpdate_function)(const mat4x4 & matView);

	typedef void (*renderer_initQueue_function)(const char * szFilename);
	typedef void (*renderer_releaseQueue_function)(void);

	typedef unsigned int (*renderer_getRenderTexture_function)(const char * szName);

	struct Plugin
	{
		Plugin(const QString & lib) : library(lib)
		{
			OnInit = nullptr;
			OnRelease = nullptr;

			OnReady = nullptr;
			OnResize = nullptr;
			OnUpdate = nullptr;

			InitQueue = nullptr;
			ReleaseQueue = nullptr;

			GetRenderTexture = nullptr;
		}

		QLibrary library;

		renderer_onInit_function OnInit;
		renderer_onRelease_function OnRelease;

		renderer_onReady_function OnReady;
		renderer_onResize_function OnResize;
		renderer_onUpdate_function OnUpdate;

		renderer_initQueue_function InitQueue;
		renderer_releaseQueue_function ReleaseQueue;

		renderer_getRenderTexture_function GetRenderTexture;
	};

	QVector<Plugin*> m_aPlugins;

};
