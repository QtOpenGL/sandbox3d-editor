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

	void			bindBboxResources	(const QMatrix4x4 & ModelViewProjection, const QVector3D & BBoxMin, const QVector3D & BBoxMax);
	void			unbindBboxResources	(void);

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
