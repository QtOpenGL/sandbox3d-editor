#pragma once

#define HAVE_OPENGL 1
#define HAVE_VULKAN 0
#define HAVE_GL_GLCOREARB_H 1

#include "Rendering/Rendering.h"

#include <QtOpenGL>

#if (QT_VERSION < QT_VERSION_CHECK(5, 4, 0))
#include <QGLWidget>
typedef QGLWidget QOpenGLWidget;
#endif // (QT_VERSION < QT_VERSION_CHECK(5, 4, 0))

#include "Camera/Camera.h"

#define NB_BUFFER 10

class QDir;
class QString;

class aiScene;
class aiNode;

class Mesh::Instance;

class DrawableSurface : public QOpenGLWidget
{
	Q_OBJECT

public:

	explicit DrawableSurface	(QWidget *parent = 0);
	virtual ~DrawableSurface	(void);

	void	ResetCamera			(void);

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

private:

	void	loadSprites			(void);

	GPU::Texture<GL_TEXTURE_2D> * loadTexture(const QString & filepath);

	void loadAllMaterials(const aiScene * scene);

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

	void addMeshRecursive(const aiNode * nd, const mat4x4 & parentTransformation, const std::vector<SubMesh*> & preloaded, GPU::Buffer<GL_ARRAY_BUFFER> * vertexBuffer, GPU::Buffer<GL_ELEMENT_ARRAY_BUFFER> * indexBuffer, const std::vector<SubMeshDefinition> & offsets, const std::vector<Mesh::VertexSpec> & specs);

	// Rendering
	Camera		m_camera;

	// Navigation
	ivec2		m_vLastPos;
	bool		m_bMoved;

	//
	unsigned int m_query;

	GPU::Buffer<GL_PIXEL_UNPACK_BUFFER> * m_apBuffer [NB_BUFFER];
	GPU::Buffer<GL_PIXEL_UNPACK_BUFFER> * m_pSpecial;

	vec4 m_vClearColor;
	vec4 m_vAmbientColor;

	std::vector<Mesh*> m_apMeshes;

	Mesh::Instance * m_pSelectedObject;

public:

	Rendering	m_renderer;

signals:

public slots:

	void importScene(const QString & filename);

	void setClearColor(const QColor & color);
	void setAmbientColor(const QColor & color);

	void	onResized				(void);
};
