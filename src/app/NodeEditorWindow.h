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

	GraphWidget::Node *	createPresentNode				(void);
	GraphWidget::Node *	createOperationNode				(const NodeDescriptor & desc);
	GraphWidget::Node *	createTextureNode				(unsigned int fomat, unsigned int width, unsigned int height);

	GraphWidget::Node *	createAdditionNode				(void);
	GraphWidget::Node *	createSubtractionNode			(void);
	GraphWidget::Node *	createMultiplicationNode		(void);
	GraphWidget::Node *	createDivisionNode				(void);

	GraphWidget::Node *	createNegationNode				(void);
	GraphWidget::Node *	createAbsoluteNode				(void);

	GraphWidget::Node *	createEqualToNode				(void);
	GraphWidget::Node *	createNotEqualToNode			(void);
	GraphWidget::Node *	createGreaterThanNode			(void);
	GraphWidget::Node *	createLessThanNode				(void);
	GraphWidget::Node *	createGreaterThanOrEqualToNode	(void);
	GraphWidget::Node *	createLessThanOrEqualToNode		(void);

	GraphWidget::Node *	createNotNode					(void);
	GraphWidget::Node *	createAndNode					(void);
	GraphWidget::Node *	createOrNode					(void);

	GraphWidget::Node *	createFloatNode					(float value = 0.0f);

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

	QPoint m_lastContextMenuPos;

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

	void on_actionCreateAdditionNode_triggered();
	void on_actionCreateSubtractionNode_triggered();
	void on_actionCreateMultiplicationNode_triggered();
	void on_actionCreateDivisionNode_triggered();

	void on_actionCreateNegationNode_triggered();
	void on_actionCreateAbsoluteNode_triggered();

	void on_actionCreateEqualToNode_triggered();
	void on_actionCreateNotEqualToNode_triggered();
	void on_actionCreateGreaterThanNode_triggered();
	void on_actionCreateLessThanNode_triggered();
	void on_actionCreateGreaterThanOrEqualToNode_triggered();
	void on_actionCreateLessThanOrEqualToNode_triggered();

	void on_actionCreateNotNode_triggered();
	void on_actionCreateAndNode_triggered();
	void on_actionCreateOrNode_triggered();

	void on_actionCreateFloatNode_triggered();
	void on_actionRemoveNode_triggered();

	void createOperationNodeFromDialog();
	void createTextureNodeFromDialog();

signals:

	void graphLoaded(const Graph & graph);
	void graphSaved(const Graph & graph);

};
