#include "NodeEditorWindow.h"
#include "ui_NodeEditorWindow.h"

#include "NodeOperationCreationWindow.h"
#include "NodeTextureCreationWindow.h"

#include <QVBoxLayout>
#include <QStatusBar>
#include <QMenuBar>
#include <QContextMenuEvent>
#include <QAction>
#include <QDir>
#include <QMessageBox>
#include <QLabel>

#include "MainWindow.h"
#include "DrawableSurface.h"

#include <graphicsnodescene.hpp>
#include <graphicsnodeview.hpp>

#include <graphicsnode.hpp>
#include <graphicsbezieredge.hpp>
#include <graphicsnodesocket.hpp>

#include <qobjectnode.hpp>

#include <jansson.h>

#include <assert.h>

#include <algorithm>

#include "Graph/Graph.h"
#include "Graph/Node.h"
#include "Graph/Edge.h"

#define GRAPH_FILE_PATH "data/render-graph.json"

#define SCENE_SIZE_X 20000.0
#define SCENE_SIZE_Y 20000.0

class GraphicsNodeTexture : public GraphicsNode
{
public:

	GraphicsNodeTexture(QGraphicsItem *parent = nullptr) : GraphicsNode(parent)
	{
		_title_item->hide();
	}

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override
	{
		painter->beginNativePainting();

		// Draw Render Target using OpenGL
		// MUST use old OpenGL because it seems that QGraphicsView does NOT support OpenGL > 3
		glEnable(GL_TEXTURE_2D);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBindTexture(GL_TEXTURE_2D, 3);
		glBegin(GL_QUADS);
		glTexCoord2d(0.0,1.0); glVertex2d(0, 0);
		glTexCoord2d(1.0,1.0); glVertex2d(_width, 0);
		glTexCoord2d(1.0,0.0); glVertex2d(_width, _height);
		glTexCoord2d(0.0,0.0); glVertex2d(0, _height);
		glEnd();

		painter->endNativePainting();

		painter->setPen(isSelected() ? _pen_selected : _pen_default);
		painter->setBrush(Qt::NoBrush);
		painter->drawRect(QRectF(0.0f, 0.0f, _width, _height));
	}

};

/**
 * @brief Constructor
 * @param Main Window
 */
NodeEditorWindow::NodeEditorWindow(MainWindow * pMainWindow)
: QDialog(pMainWindow)
, ui(new Ui::NodeEditorWindow)
, m_pView(nullptr)
, m_pScene(nullptr)
{
	ui->setupUi(this); // created with Qt Designer

	// Custom Widgets
	m_pMenuBar				= new QMenuBar(this); // can't add a Menu bar from Designer
	m_pStatusBar			= new QStatusBar(this); // can't add a Status bar from Designer

	m_pWidgetGL				= new QOpenGLWidget(this);

	m_pScene				= new GraphicsNodeScene(this);
	m_pView					= new GraphicsNodeView(m_pScene, this);

	ui->layout->addWidget(m_pMenuBar);
	ui->layout->addWidget(m_pView);
	ui->layout->addWidget(m_pStatusBar);

	// Context Menus
	m_pContextMenuScene		= new QMenu(this);
	m_pContextMenuNode		= new QMenu(this);

	// Window
	setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint);

	// Node Widget
	m_pScene->setSceneRect(-(SCENE_SIZE_X*0.5), -(SCENE_SIZE_Y*0.5), SCENE_SIZE_X, SCENE_SIZE_Y);

	// Enable OpenGL rendering for the Node View
	QSurfaceFormat format;
	format.setSamples(4);

	m_pWidgetGL->setFormat(format);

	m_pView->setViewport(m_pWidgetGL);
	m_pView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

	// Load descriptors (to create nodes)
	{
		loadNodeDescriptors();
	}

	//
	// Create windows
	{
		m_pNodeOperationCreationWindow	= new NodeOperationCreationWindow(m_aNodeDescriptors, this);
		m_pNodeTextureCreationWindow = new NodeTextureCreationWindow(this);
	}

	// Actions
	m_pContextMenuScene->addAction(ui->actionCreateUserDefinedNode);
	m_pContextMenuScene->addAction(ui->actionCreateTextureNode);
	m_pContextMenuNode->addAction(ui->actionRemoveNode);

	addAction(ui->actionRemoveNode);
	addAction(ui->actionSave);

	connect(m_pNodeOperationCreationWindow, SIGNAL(accepted()), this, SLOT(createOperationNodeFromDialog()));
	connect(m_pNodeTextureCreationWindow, SIGNAL(accepted()), this, SLOT(createTextureNodeFromDialog()));

	// Load current Render Graph
	if (!loadGraph())
	{
		createPresentNode();
	}

	connect(pMainWindow->m_pDrawable, &QOpenGLWidget::frameSwapped, this, &NodeEditorWindow::onSceneFrameSwapped);
}

/**
 * @brief Destructor
 */
NodeEditorWindow::~NodeEditorWindow(void)
{
	// nothing here
}

/**
 * @brief NodeEditorWindow::closeEvent
 * @param event
 */
void NodeEditorWindow::closeEvent(QCloseEvent * pEvent)
{
	// nothing here
}

/**
 * @brief NodeEditorWindow::contextMenuEvent
 * @param event
 */
void NodeEditorWindow::contextMenuEvent(QContextMenuEvent * pEvent)
{
	QGraphicsItem * item = m_pView->itemAt(pEvent->pos());

	m_pScene->clearSelection();

	if (item)
	{
		switch (item->type())
		{
			case GraphicsNodeItemTypes::TypeNode:
			{
				item->setSelected(true);

				m_pContextMenuNode->exec(pEvent->globalPos());
			}
			break;
		}
	}
	else
	{
		m_pContextMenuScene->exec(pEvent->globalPos());
	}
}

/**
 * @brief NodeEditorWindow::loadNodeDescriptors
 */
void NodeEditorWindow::loadNodeDescriptors(void)
{
	QDir dir("data/nodes");
	QStringList list = dir.entryList(QDir::Files);

	bool successful = true;

	for (QString & filename : list)
	{
		NodeDescriptor desc;
		bool loading = desc.loadFromFile(dir.filePath(filename).toStdString());

		if (loading)
		{
			m_aNodeDescriptors.push_back(desc);
		}
		else
		{
			successful = false;
		}
	}

	if (!successful)
	{
		int ret = QMessageBox::warning(this, tr("Sandbox 3D"), tr("All Nodes were not loaded successfully !"));
		assert(0 == ret);
	}
}

/**
 * @brief NodeEditorWindow::loadGraph
 * @return
 */
bool NodeEditorWindow::loadGraph(void)
{
	Graph G;
	if (!G.loadFromFile(GRAPH_FILE_PATH))
	{
		return(false);
	}

	assert(G.getType() == "RenderGraph");

	std::map<uintptr_t, GraphicsNode *> mapUID;

	for (Node * pNode : G.getNodes())
	{
		// ID
		const std::string & strUID = pNode->getId();
		uintptr_t uid =  atoll(strUID.c_str());
		assert(0 != uid);

		// Type
		const std::string & strType = pNode->getType();
		const char * szType = strType.c_str();

		// Label
		const std::string & strLabel = pNode->getLabel();
		const char * szLabel = strLabel.c_str();

		// Metadata
		const std::string & strNodePosX = pNode->getMetaData("xloc");
		const std::string & strNodePosY = pNode->getMetaData("yloc");
		const std::string & strNodeSizeX = pNode->getMetaData("width");
		const std::string & strNodeSizeY = pNode->getMetaData("height");

		// Create Node
		GraphicsNode * n = nullptr;

		if ("operation" == strType)
		{
			const std::string & strSubType = pNode->getMetaData("subtype");
			const char * szSubType = strSubType.c_str();

			std::vector<NodeDescriptor>::iterator it = std::find(m_aNodeDescriptors.begin(), m_aNodeDescriptors.end(), szSubType);
			assert(it != m_aNodeDescriptors.end());

			n = createOperationNode(*it);
		}
		else if ("texture" == strType)
		{
			const std::string & strFormat = pNode->getMetaData("format");
			int iFormat = atoi(strFormat.c_str());

			n = createTextureNode(iFormat, 1024, 1024);
		}
		else if ("present" == strType)
		{
			n = createPresentNode();
		}
		else
		{
			assert(false);
		}

		n->setPos(atof(strNodePosX.c_str()), atof(strNodePosY.c_str()));
		n->setSize(atof(strNodeSizeX.c_str()), atof(strNodeSizeY.c_str()));
		n->setTitle(szLabel); // currently useless

		mapUID[uid] = n;
	}

	for (Edge * pEdge : G.getEdges())
	{
		// Source
		Node * pSourceNode = pEdge->getSource();
		uintptr_t source_uid = atoll(pSourceNode->getId().c_str());

		// Target
		Node * pTargetNode = pEdge->getTarget();
		uintptr_t target_uid = atoll(pTargetNode->getId().c_str());

		// Source Id
		const std::string & strSourceId = pEdge->getMetaData("source_id");
		unsigned int iSourceId = atoi(strSourceId.c_str());

		// Target Id
		const std::string & strTargetId = pEdge->getMetaData("target_id");
		unsigned int iTargetId = atoi(strTargetId.c_str());

		// Create Edge
		GraphicsNode * pGraphicsSourceNode = mapUID[source_uid];
		assert(nullptr != pGraphicsSourceNode);

		GraphicsNode * pGraphicsTargetNode = mapUID[target_uid];
		assert(nullptr != pGraphicsTargetNode);

		GraphicsDirectedEdge * e = new GraphicsBezierEdge();
		e->connect(pGraphicsSourceNode, iSourceId, pGraphicsTargetNode, iTargetId);

		m_pScene->addItem(e);
	}

	return(true);
}

/**
 * @brief NodeEditorWindow::saveGraph
 * @return
 */
bool NodeEditorWindow::saveGraph(void)
{
	json_t * pRoot = json_object();

	// Add the graph
	json_t * pGraph = json_object();
	json_object_set(pRoot, "graph", pGraph);

	// Type
	json_t * pGraphType = json_string("RenderGraph");
	json_object_set(pGraph, "type", pGraphType);

	// Label
	json_t * pGraphLabel = json_string("Unnamed Render Graph");
	json_object_set(pGraph, "label", pGraphLabel);

	// Metadata
	json_t * pGraphMetadata = json_object();
	json_object_set(pGraph, "metadata", pGraphMetadata);

	// Add the nodes/edges
	json_t * pGraphNodes = json_array();
	json_object_set(pGraph, "nodes", pGraphNodes);

	json_t * pGraphEdges = json_array();
	json_object_set(pGraph, "edges", pGraphEdges);

	QList<QGraphicsItem*> items = m_pScene->items();

	for (QGraphicsItem * item : items)
	{
		switch (item->type())
		{
			case GraphicsNodeItemTypes::TypeNode:
			{
				GraphicsNode * pNodeItem = static_cast<GraphicsNode*>(item);
				assert(nullptr != pNodeItem);

				std::string & strNodeType = m_mapNodeType[pNodeItem];

				json_t * pNode = json_object();
				json_array_append(pGraphNodes, pNode);

				// ID
				json_t * pNodeID = json_integer(uintptr_t(pNodeItem));
				json_object_set(pNode, "id", pNodeID);

				// Type
				json_t * pNodeType = json_string(m_mapNodeType[pNodeItem].c_str());
				json_object_set(pNode, "type", pNodeType);

				// Label
				json_t * pNodeLabel = json_string(pNodeItem->getTitle().toLocal8Bit());
				json_object_set(pNode, "label", pNodeLabel);

				// Metadata
				json_t * pNodeMetada = json_object();
				json_object_set(pNode, "metadata", pNodeMetada);

				if (strNodeType == "operation")
				{
					json_t * pNodeSubType = json_string(m_mapNode[pNodeItem]->identifier.c_str());
					json_object_set(pNodeMetada, "subtype", pNodeSubType);
				}
				else if (strNodeType == "texture")
				{
					json_t * pNodeSubType = json_integer(0);
					json_object_set(pNodeMetada, "format", pNodeSubType);
				}

				json_t * pNodePosX = json_real(pNodeItem->pos().x());
				json_object_set(pNodeMetada, "xloc", pNodePosX);

				json_t * pNodePosY = json_real(pNodeItem->pos().y());
				json_object_set(pNodeMetada, "yloc", pNodePosY);

				json_t * pNodeSizeX = json_real(pNodeItem->width());
				json_object_set(pNodeMetada, "width", pNodeSizeX);

				json_t * pNodeSizeY = json_real(pNodeItem->height());
				json_object_set(pNodeMetada, "height", pNodeSizeY);
			}
			break;

			case GraphicsNodeItemTypes::TypeBezierEdge:
			{
				GraphicsBezierEdge * pEdgeItem = static_cast<GraphicsBezierEdge*>(item);
				assert(nullptr != pEdgeItem);

				json_t * pEdge = json_object();
				json_array_append(pGraphEdges, pEdge);

				// Source
				json_t * pEdgeSource = json_integer(uintptr_t(pEdgeItem->getSource()->parentItem()));
				json_object_set(pEdge, "source", pEdgeSource);

				// Target
				json_t * pEdgeTarget = json_integer(uintptr_t(pEdgeItem->getSink()->parentItem()));
				json_object_set(pEdge, "target", pEdgeTarget);

				// Directed
				json_t * pEdgeDirected = json_true();
				json_object_set(pEdge, "directed", pEdgeDirected);

				// Metadata
				json_t * pEdgeMetada = json_object();
				json_object_set(pEdge, "metadata", pEdgeMetada);

				json_t * pEdgeSourceId = json_integer(pEdgeItem->getSource()->getIndex());
				json_object_set(pEdgeMetada, "source_id", pEdgeSourceId);

				json_t * pEdgeTargetId = json_integer(pEdgeItem->getSink()->getIndex());
				json_object_set(pEdgeMetada, "target_id", pEdgeTargetId);
			}
			break;

			default:
			{
				// don't save anything else
			}
		}
	}

	int success = json_dump_file(pRoot, GRAPH_FILE_PATH, 0);

	return(0 == success);
}

/**
 * @brief NodeEditorWindow::createDefaultNodes
 */
GraphicsNode * NodeEditorWindow::createPresentNode(void)
{
	GraphicsNode * n = new GraphicsNode();

	n->setTitle(QString("Present"));

	n->add_sink(QString("Backbuffer"), nullptr, 0);

	m_pScene->addItem(n);

	m_mapNodeType.insert(std::pair<const GraphicsNode*, std::string>(n, "present"));

	return(n);
}

/**
 * @brief NodeEditorWindow::createNode
 * @param desc
 */
GraphicsNode * NodeEditorWindow::createOperationNode(const NodeDescriptor & desc)
{
	GraphicsNode * n = new GraphicsNode();

	n->setTitle(QString(desc.name.c_str()));

	int input_index = 0;

	for (const NodeDescriptor::Input & input : desc.inputs)
	{
		n->add_sink(QString(input.name.c_str()), nullptr, input_index);
		++input_index;
	}

	int output_index = 0;

	for (const NodeDescriptor::Output & output : desc.outputs)
	{
		n->add_source(QString(output.name.c_str()), nullptr, output_index);
		++output_index;
	}

	m_pScene->addItem(n);

	m_mapNode.insert(std::pair<const GraphicsNode*, const NodeDescriptor*>(n, &desc));
	m_mapNodeType.insert(std::pair<const GraphicsNode*, std::string>(n, "operation"));

	return(n);
}
/**
 * @brief NodeEditorWindow::createNode
 * @param desc
 */
GraphicsNode * NodeEditorWindow::createTextureNode(unsigned int format, unsigned int width, unsigned int height)
{
	GraphicsNode * n = new GraphicsNodeTexture();

	n->setTitle(QString("Texture"));

	n->add_sink(QString(), nullptr, 0);
	n->add_source(QString(), nullptr, 0);

	QPixmap * pPixmap = new QPixmap(128, 128);
	pPixmap->fill(Qt::red);

	QLabel * pLabel = new QLabel();
	pLabel->setPixmap(*pPixmap);

	//n->setCentralWidget(pLabel);

	m_pScene->addItem(n);

	//m_mapNode.insert(std::pair<const GraphicsNode*, const NodeDescriptor*>(n, &desc));
	m_mapNodeType.insert(std::pair<const GraphicsNode*, std::string>(n, "texture"));

	return(n);
}

/**
 * @brief NodeEditorWindow::onSceneFrameSwapped
 */
void NodeEditorWindow::onSceneFrameSwapped(void)
{
	m_pWidgetGL->update();
}

/**
 * @brief NodeEditorWindow::on_actionSave_triggered
 */
void NodeEditorWindow::on_actionSave_triggered(void)
{
	saveGraph();
}

/**
 * @brief NodeEditorWindow::actionCreateUserDefinedNode
 */
void NodeEditorWindow::on_actionCreateUserDefinedNode_triggered(void)
{
	m_pNodeOperationCreationWindow->show();
}

/**
 * @brief NodeEditorWindow::actionCreateUserDefinedNode
 */
void NodeEditorWindow::on_actionCreateTextureNode_triggered(void)
{
	m_pNodeTextureCreationWindow->show();
}

/**
 * @brief NodeEditorWindow::on_actionRemoveNode_triggered
 */
void NodeEditorWindow::on_actionRemoveNode_triggered(void)
{
	QList<QGraphicsItem *> list = m_pScene->selectedItems();

	for (QGraphicsItem * item : list)
	{
		if (item->type() == GraphicsNodeItemTypes::TypeNode)
		{
			item->setSelected(false);
			m_pScene->removeItem(item);
		}
	}
}

/**
 * @brief NodeEditorWindow::createNodeFromWindow
 */
void NodeEditorWindow::createOperationNodeFromDialog(void)
{
	NodeDescriptor * pDesc = m_pNodeOperationCreationWindow->getCurrentDescriptor();

	if (pDesc)
	{
		createOperationNode(*pDesc);
	}
}

/**
 * @brief NodeEditorWindow::createNodeFromWindow
 */
void NodeEditorWindow::createTextureNodeFromDialog(void)
{
	unsigned int format = m_pNodeTextureCreationWindow->getCurrentFormat();

	createTextureNode(format, 1024, 1024);
}
