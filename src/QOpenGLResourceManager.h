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

protected:

	void			initShaders			(void);

private:

	static QOpenGLResourceManager instance_;

	// shared vertex buffer
	QOpenGLBuffer				m_sharedVertexBuffer;

	// Quad Rendering
	QOpenGLVertexArrayObject	m_vertexArrayQuad;
	QOpenGLShaderProgram *		m_pShaderQuad;

	// Bounding Box Rendering
	QOpenGLVertexArrayObject	m_vertexArrayBbox;
	QOpenGLShaderProgram *		m_pShaderBbox;
};
