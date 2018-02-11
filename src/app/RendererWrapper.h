#pragma once

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

	//
	// ...
	bool		init				(void);
	bool		release				(void);

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

	Scene * m_pScene;

private:

	QLibrary m_rendererLibrary;

	typedef bool (*renderer_onLoad_function)(void);

	typedef bool (*renderer_onInit_function)(Scene & scene);
	typedef bool (*renderer_onRelease_function)(void);

	typedef void (*renderer_onReady_function)(void);
	typedef void (*renderer_onResize_function)(unsigned int width, unsigned int height);
	typedef void (*renderer_onUpdate_function)(const mat4x4 & matView);

	typedef void (*renderer_initQueue_function)(const char * szFilename);
	typedef void (*renderer_releaseQueue_function)(void);

	typedef unsigned int (*renderer_getRenderTexture_function)(const char * szName);

	renderer_onInit_function m_pOnInit;
	renderer_onRelease_function m_pOnRelease;

	renderer_onReady_function m_pOnReady;
	renderer_onResize_function m_pOnResize;
	renderer_onUpdate_function m_pOnUpdate;

	renderer_initQueue_function m_pInitQueue;
	renderer_releaseQueue_function m_pReleaseQueue;

	renderer_getRenderTexture_function m_pGetRenderTexture;

};
