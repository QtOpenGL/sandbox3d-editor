#pragma once

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
	void		onResize			(int width, int height);
	void		onUpdate			(const mat4x4 & mView);

	//
	// ...
	void		initQueue			(const char * szFilename);
	void		releaseQueue		(void);

	//
	// Scene management
	Scene &		getScene			(void);
	bool		importToScene		(const char * szFilename);

	//
	// ...
	unsigned int /*GLuint*/ GetRenderTexture(const char * name) const;

	void SetDefaultFramebuffer(unsigned int /*GLuint*/ framebuffer);

	//
	// ...
	const mat4x4 & getProjection(void) const;

};
