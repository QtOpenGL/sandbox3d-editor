#pragma once

#include "RendererWrapper.h"

#include <QtOpenGL>

#if (QT_VERSION < QT_VERSION_CHECK(5, 4, 0))
#include <QGLWidget>
typedef QGLWidget QOpenGLWidget;
#endif // (QT_VERSION < QT_VERSION_CHECK(5, 4, 0))

#include "Camera/Camera.h"

class QDir;
class QString;

class aiScene;
class aiNode;

class DrawableSurface : public QOpenGLWidget
{
	Q_OBJECT

public:

	explicit DrawableSurface	(QWidget *parent = 0);
	virtual ~DrawableSurface	(void);

	void	ResetCamera			(void);

	void	setCurrentTexture	(const std::string & strFinalTextureId);

protected:

	void	initializeGL		(void);
	void	resizeGL			(int w, int h);
	void	paintGL				(void);

	void	mousePressEvent		(QMouseEvent * event);
	void	mouseMoveEvent		(QMouseEvent * event);
	void	mouseReleaseEvent	(QMouseEvent * event);

	void	wheelEvent			(QWheelEvent * event);

	void    keyPressEvent       (QKeyEvent * event);
	void    keyReleaseEvent     (QKeyEvent * event);

public:

	struct SubMeshDefinition
	{
		unsigned int triangle_count; // GL_TRIANGLES
		unsigned int index_offset; // GL_UNSIGNED_INT
		unsigned int base_vertex;
		SubMesh::Material material;
		const GPU::Texture<GL_TEXTURE_2D> * m_pNormalMap;
		vec3 m_vMin;
		vec3 m_vMax;
	};

private:

	Object * getObjectAtPos(const ivec2 & pos);

	GPU::Texture<GL_TEXTURE_2D> * loadTexture(const QString & filepath);

	void loadAllMaterials(const aiScene * scene);

	// Rendering
	Camera		m_camera;

	// Navigation
	ivec2		m_vLastPos;
	bool		m_bMoved;

	//
	unsigned int m_query;

	vec4 m_vClearColor;
	vec4 m_vAmbientColor;

	std::vector<Mesh*> m_apMeshes;

	Object * m_pSelectedObject;

	GLuint m_uCurrentTexture;

signals:

public slots:

	void importScene(const QString & filename);

	void setClearColor(const QColor & color);
	void setAmbientColor(const QColor & color);

	void	onFrameSwapped			(void);
	void	onResized				(void);
};
