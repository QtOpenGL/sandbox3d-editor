#pragma once

#include "NodeDescription.h"

#include <QDialog>

class QOpenGLWidget;
class QOpenGLContext;

class QMenuBar;
class QStatusBar;
class QMenu;

class NodeOperationCreationWindow;
class NodeTextureCreationWindow;

class QGraphicsScene;

class MainWindow;

class Graph;

namespace GraphWidget {
	class View;
	class Node;
}

namespace Ui {
	class NodeEditorWindow;
}

class NodeEditorWindow : public QDialog
{
	Q_OBJECT

public:

	explicit			NodeEditorWindow		(MainWindow * pMainWindow);
	virtual				~NodeEditorWindow		(void);

	void				init					(void);

	virtual void		closeEvent				(QCloseEvent * event) Q_DECL_OVERRIDE;
	virtual void		contextMenuEvent		(QContextMenuEvent * event) Q_DECL_OVERRIDE;

protected:

	void				loadNodeDescriptors		(void);

	bool				loadGraph				(void);
	bool				saveGraph				(void);
	bool				createGraph				(Graph & graph);

	GraphWidget::Node *	createPresentNode		(void);
	GraphWidget::Node *	createOperationNode		(const NodeDescriptor & desc);
	GraphWidget::Node *	createTextureNode		(unsigned int fomat, unsigned int width, unsigned int height);

	void				updateTextures			(void);

private:

	Ui::NodeEditorWindow * ui;

protected:

	QMenuBar *				m_pMenuBar;
	QStatusBar *			m_pStatusBar;

	GraphWidget::View *		m_pView;
	QGraphicsScene *		m_pScene;

	QOpenGLWidget *			m_pWidgetGL;

	QMenu *					m_pContextMenuScene;
	QMenu *					m_pContextMenuNode;

	NodeOperationCreationWindow *	m_pNodeOperationCreationWindow;
	NodeTextureCreationWindow *		m_pNodeTextureCreationWindow;

	std::map<const GraphWidget::Node*, const NodeDescriptor*> m_mapOperationNodes;
	std::map<const GraphWidget::Node*, unsigned int> m_mapTextureNodes;
	std::map<const GraphWidget::Node*, std::string> m_mapNodeType;

public:

	std::vector<NodeDescriptor>		m_aNodeDescriptors;

private slots:


public slots:

	void onSceneFrameSwapped(void);

	void on_actionSave_triggered();

	void on_actionCreateUserDefinedNode_triggered();
	void on_actionCreateTextureNode_triggered();
	void on_actionRemoveNode_triggered();

	void createOperationNodeFromDialog();
	void createTextureNodeFromDialog();

signals:

	void graphLoaded(const Graph & graph);
	void graphSaved(const Graph & graph);

};
