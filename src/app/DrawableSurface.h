#pragma once

#include <QtOpenGL>
#include <QOpenGLFunctions_3_2_Core>

#if (QT_VERSION < QT_VERSION_CHECK(5, 4, 0))
#include <QGLWidget>
typedef QGLWidget QOpenGLWidget;
#endif // (QT_VERSION < QT_VERSION_CHECK(5, 4, 0))

#include "Camera/Camera.h"

class QDir;
class QString;

class aiScene;
class aiNode;

class Object;
class Mesh;

class DrawableSurface : public QOpenGLWidget, public QOpenGLFunctions_3_2_Core
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

private:

	Object * getObjectAtPos(const ivec2 & pos);

	void loadAllMaterials(const aiScene * scene);

	// Rendering
	Camera		m_camera;

	// Navigation
	ivec2		m_vLastPos;
	bool		m_bMoved;

	//
	GLuint m_query;

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
