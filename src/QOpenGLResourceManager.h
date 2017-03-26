#pragma once

#include <QtOpenGL>

class QOpenGLResourceManager
{

protected:

	QOpenGLResourceManager(void);
	~QOpenGLResourceManager(void);

public:

	static QOpenGLResourceManager & instance(void);

	bool			init				(void);
	void			release				(void);

	void			bindQuadResources	(void);
	void			unbindQuadResources	(void);

private:

	static QOpenGLResourceManager instance_;

	// for quad rendering
	QOpenGLBuffer				m_vertexBufferQuad;
	QOpenGLVertexArrayObject	m_vertexArrayQuad;
	QOpenGLShaderProgram *		m_pShaderQuad;
};
