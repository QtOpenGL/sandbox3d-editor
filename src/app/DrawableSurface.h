#pragma once

#include <QtOpenGL>
#include <QOpenGLFunctions_3_2_Core>

#if (QT_VERSION < QT_VERSION_CHECK(5, 4, 0))
#include <QGLWidget>
typedef QGLWidget QOpenGLWidget;
#endif // (QT_VERSION < QT_VERSION_CHECK(5, 4, 0))

#include "Objects/Camera/Camera.h"

#include "RenderGraph.h"

class QString;

class Object;

class DrawableSurface : public QOpenGLWidget, public QOpenGLFunctions_3_2_Core
{
	Q_OBJECT

public:

	explicit DrawableSurface	(QWidget *parent = 0);
	virtual ~DrawableSurface	(void);

	void	ResetCamera			(void);

	void	DrawObjectsAABB		(bool bEnable);
	void	DrawSceneAABB		(bool bEnable);

	void	setCurrentTexture	(const QString & strFinalTextureId);

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

	// Rendering
	Camera		m_camera;

	// Navigation
	ivec2		m_vLastPos;
	bool		m_bMoved;

	//
	// AABB debug
	bool		m_bDrawObjectsAABB;
	bool		m_bDrawSceneAABB;

	//
	//
	Object * m_pSelectedObject;

	//
	// Draw
	GLuint m_uCurrentTexture;

	//
	// Render Graph
	RenderGraph::Queue * m_pRenderQueue;

signals:

public slots:

	void importScene(const QString & filename);

	void	onFrameSwapped			(void);
	void	onResized				(void);
};
