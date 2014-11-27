#pragma once

#include <GPU.h>

#include <Vector.h>
#include <Matrix.h>

class LightAccumBuffer
{
public:

	LightAccumBuffer	(void);
	~LightAccumBuffer	(void);

	bool	init	(const GPU::Texture<GL_TEXTURE_2D> * pTexture);
	void	free	(void);

	bool	enable	(void);
	bool	disable	(void);

	GLuint	GetObject(void) { return(m_uObject); }

private:

	GLuint m_uObject;

};
