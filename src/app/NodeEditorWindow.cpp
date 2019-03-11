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
#include "RendererWrapper.h"
#include "DrawableSurface.h"

#include <assert.h>

#include <algorithm>

#include "Graph.h"
#include "Node.h"
#include "Edge.h"

#include "GraphWidget/View.h"
#include "GraphWidget/Edge.h"
#include "GraphWidget/Connector.h"
#include "GraphWidget/Node.h"
#include "GraphWidget/NodeTexture.h"
#include "GraphWidget/NodePass.h"
#include "GraphWidget/NodeFinal.h"
#include "GraphWidget/NodeComparison.h"
#include "GraphWidget/NodeFloat.h"

#define GRAPH_FILE_PATH "data/render-graph.json"

#define SCENE_SIZE_X 20000.0
#define SCENE_SIZE_Y 20000.0

GLenum StringToGLenum(const char * szEnumName)
{
	if (!strcmp(szEnumName, "R8")) return(GL_R8);
	else if (!strcmp(szEnumName, "RG8")) return(GL_RG8);
	else if (!strcmp(szEnumName, "RGBA8")) return(GL_RGBA8);

	else if (!strcmp(szEnumName, "R16")) return(GL_R16);
	else if (!strcmp(szEnumName, "RG16")) return(GL_RG16);
	else if (!strcmp(szEnumName, "RGBA16")) return(GL_RGBA16);

	else if (!strcmp(szEnumName, "R16F")) return(GL_R16F);
	else if (!strcmp(szEnumName, "RG16F")) return(GL_RG16F);
	else if (!strcmp(szEnumName, "RGBA16F")) return(GL_RGBA16F);

	else if (!strcmp(szEnumName, "R32F")) return(GL_R32F);
	else if (!strcmp(szEnumName, "RG32F")) return(GL_RG32F);
	else if (!strcmp(szEnumName, "RGBA32F")) return(GL_RGBA32F);

	else if (!strcmp(szEnumName, "R8I")) return(GL_R8I);
	else if (!strcmp(szEnumName, "RG8I")) return(GL_RG8I);
	else if (!strcmp(szEnumName, "RGBA8I")) return(GL_RGBA8I);

	else if (!strcmp(szEnumName, "R16I")) return(GL_R16I);
	else if (!strcmp(szEnumName, "RG16I")) return(GL_RG16I);
	else if (!strcmp(szEnumName, "RGBA16I")) return(GL_RGBA16I);

	else if (!strcmp(szEnumName, "R32I")) return(GL_R32I);
	else if (!strcmp(szEnumName, "RG32I")) return(GL_RG32I);
	else if (!strcmp(szEnumName, "RGBA32I")) return(GL_RGBA32I);

	else if (!strcmp(szEnumName, "R8UI")) return(GL_R8UI);
	else if (!strcmp(szEnumName, "RG8UI")) return(GL_RG8UI);
	else if (!strcmp(szEnumName, "RGBA8UI")) return(GL_RGBA8UI);

	else if (!strcmp(szEnumName, "R16UI")) return(GL_R16UI);
	else if (!strcmp(szEnumName, "RG16UI")) return(GL_RG16UI);
	else if (!strcmp(szEnumName, "RGBA16UI")) return(GL_RGBA16UI);

	else if (!strcmp(szEnumName, "R32UI")) return(GL_R32UI);
	else if (!strcmp(szEnumName, "RG32UI")) return(GL_RG32UI);
	else if (!strcmp(szEnumName, "RGBA32UI")) return(GL_RGBA32UI);

	else if (!strcmp(szEnumName, "RGB10_A2")) return(GL_RGB10_A2);
	else if (!strcmp(szEnumName, "RGB10_A2UI")) return(GL_RGB10_A2UI);
	else if (!strcmp(szEnumName, "R11F_G11F_B10F")) return(GL_R11F_G11F_B10F);
	else if (!strcmp(szEnumName, "SRGB8_ALPHA8")) return(GL_SRGB8_ALPHA8);

	else if (!strcmp(szEnumName, "DEPTH_COMPONENT16")) return(GL_DEPTH_COMPONENT16);
	else if (!strcmp(szEnumName, "DEPTH_COMPONENT24")) return(GL_DEPTH_COMPONENT24);
	else if (!strcmp(szEnumName, "DEPTH_COMPONENT32F")) return(GL_DEPTH_COMPONENT32F);
	else if (!strcmp(szEnumName, "DEPTH24_STENCIL8")) return(GL_DEPTH24_STENCIL8);
	else if (!strcmp(szEnumName, "DEPTH32F_STENCIL8")) return(GL_DEPTH32F_STENCIL8);
	else if (!strcmp(szEnumName, "STENCIL_INDEX8")) return(GL_STENCIL_INDEX8);

	return(0);
}

const char * GLenumToString(unsigned int e)
{
	switch(e)
	{
	case GL_R8: return("R8"); break;
	case GL_RG8: return("RG8"); break;
	case GL_RGBA8: return("RGBA8"); break;

	case GL_R16: return("R16"); break;
	case GL_RG16: return("RG16"); break;
	case GL_RGBA16: return("RGBA16"); break;

	case GL_R16F: return("R16F"); break;
	case GL_RG16F: return("RG16F"); break;
	case GL_RGBA16F: return("RGBA16F"); break;

	case GL_R32F: return("R32F"); break;
	case GL_RG32F: return("RG32F"); break;
	case GL_RGBA32F: return("RGBA32F"); break;

	case GL_R8I: return("R8I"); break;
	case GL_RG8I: return("RG8I"); break;
	case GL_RGBA8I: return("RGBA8I"); break;

	case GL_R16I: return("R16I"); break;
	case GL_RG16I: return("RG16I"); break;
	case GL_RGBA16I: return("RGBA16I"); break;

	case GL_R32I: return("R32I"); break;
	case GL_RG32I: return("RG32I"); break;
	case GL_RGBA32I: return("RGBA32I"); break;

	case GL_R8UI: return("R8UI"); break;
	case GL_RG8UI: return("RG8UI"); break;
	case GL_RGBA8UI: return("RGBA8UI"); break;

	case GL_R16UI: return("R16UI"); break;
	case GL_RG16UI: return("RG16UI"); break;
	case GL_RGBA16UI: return("RGBA16UI"); break;

	case GL_R32UI: return("R32UI"); break;
	case GL_RG32UI: return("RG32UI"); break;
	case GL_RGBA32UI: return("RGBA32UI"); break;

	case GL_RGB10_A2: return("RGB10_A2"); break;
	case GL_RGB10_A2UI: return("RGB10_A2UI"); break;
	case GL_R11F_G11F_B10F: return("R11F_G11F_B10F"); break;
	case GL_SRGB8_ALPHA8: return("SRGB8_ALPHA8"); break;

	case GL_DEPTH_COMPONENT16: return("DEPTH_COMPONENT16"); break;
	case GL_DEPTH_COMPONENT24: return("DEPTH_COMPONENT24"); break;
	case GL_DEPTH_COMPONENT32F: return("DEPTH_COMPONENT32F"); break;
	case GL_DEPTH24_STENCIL8: return("DEPTH24_STENCIL8"); break;
	case GL_DEPTH32F_STENCIL8: return("DEPTH32F_STENCIL8"); break;
	case GL_STENCIL_INDEX8: return("STENCIL_INDEX8"); break;
	}

	return("");
}

/**
* @brief Constructor
* @param Main Window
*/
NodeEditorWindow::NodeEditorWindow(MainWindow * pMainWindow)
	: QDialog(pMainWindow)
	, ui(new Ui::NodeEditorWindow)
	, m_pView(nullptr)
	, m_pScene(nullptr)
	, m_lastContextMenuPos(0, 0)
{
	ui->setupUi(this); // created with Qt Designer

	// Custom Widgets
	m_pMenuBar				= new QMenuBar(this); // can't add a Menu bar from Designer
	m_pStatusBar			= new QStatusBar(this); // can't add a Status bar from Designer

	m_pWidgetGL				= new QOpenGLWidget(this);

	m_pScene				= new QGraphicsScene(this);
	m_pView					= new GraphWidget::View(m_pScene, this);

	ui->layout->addWidget(m_pMenuBar);
	ui->layout->addWidget(m_pView);
	ui->layout->addWidget(m_pStatusBar);

	// Context Menus
	m_pContextMenuScene		= new QMenu(this);
	m_pContextMenuNode		= new QMenu(this);

	// Window
	setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint);

	// Node Widget
	m_pScene->setItemIndexMethod(QGraphicsScene::NoIndex);
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
	m_pContextMenuScene->addAction(ui->actionCreateComparisonNode);
	m_pContextMenuScene->addAction(ui->actionCreateFloatNode);
	m_pContextMenuNode->addAction(ui->actionRemoveNode);

	addAction(ui->actionRemoveNode);
	addAction(ui->actionSave);

	connect(m_pNodeOperationCreationWindow, SIGNAL(accepted()), this, SLOT(createOperationNodeFromDialog()));
	connect(m_pNodeTextureCreationWindow, SIGNAL(accepted()), this, SLOT(createTextureNodeFromDialog()));

	connect(pMainWindow->m_pDrawable, &QOpenGLWidget::frameSwapped, this, &NodeEditorWindow::onSceneFrameSwapped);
	connect(pMainWindow->m_pDrawable, &DrawableSurface::postResizeGL, this, &NodeEditorWindow::updateTextures);

	QSettings settings;
	{
		QVariant posVariant = settings.value("nodeWindowPos");
		if (!posVariant.isNull())
		{
			move(posVariant.toPoint());
		}

		QVariant sizeVariant = settings.value("nodeWindowSize");
		if (!sizeVariant.isNull())
		{
			resize(sizeVariant.toSize());
		}
	}
}

/**
* @brief Destructor
*/
NodeEditorWindow::~NodeEditorWindow(void)
{
	// nothing here
}

/**
* @brief NodeEditorWindow::init
*/
void NodeEditorWindow::init(void)
{
	// Load current Render Graph
	{
		if (loadGraph())
		{
			Graph G;
			createGraph(G);
			emit graphLoaded(G);

			updateTextures();
		}
		else
		{
			createPresentNode();
		}
	}

	QDialog::show();
}

/**
* @brief NodeEditorWindow::closeEvent
* @param event
*/
void NodeEditorWindow::closeEvent(QCloseEvent * pEvent)
{
	(void)pEvent;

	QSettings settings;
	settings.setValue("nodeWindowPos", pos());
	settings.setValue("nodeWindowSize", size());
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
			case GraphWidget::Node::Type:
			{
				item->setSelected(true);

				m_pContextMenuNode->exec(pEvent->globalPos());
			}
			break;
		}
	}
	else
	{
		m_lastContextMenuPos = mapFromGlobal(pEvent->globalPos());
		m_pContextMenuScene->exec(pEvent->globalPos());
	}
}

/**
* @brief NodeEditorWindow::loadNodeDescriptors
*/
void NodeEditorWindow::loadNodeDescriptors(void)
{
	bool successful = true;

	extern RendererWrapper g_RendererWrapper;

	const QVector<QString> & aNodesDir = g_RendererWrapper.GetNodesDirectories();

	for (const QString & strNodesDir : aNodesDir)
	{
		QDir dir(strNodesDir);
		QStringList list = dir.entryList(QDir::Files);

		for (QString & filename : list)
		{
			NodeDescriptor desc;
			bool loading = desc.loadFromFile(dir.filePath(filename));

			if (loading)
			{
				m_aNodeDescriptors.push_back(desc);
			}
			else
			{
				successful = false;
			}
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

	m_pView->setTitle(QString::fromStdString(G.getLabel()));

	assert(G.getType() == "RenderGraph");

	std::map<uintptr_t, GraphWidget::Node*> mapUID;

	for (Node * pNode : G.getNodes())
	{
		// ID
		const std::string & strUID = pNode->getId();
		uintptr_t uid = atoll(strUID.c_str());
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
		GraphWidget::Node * n = nullptr;

		if ("pass" == strType)
		{
			const std::string & strSubType = pNode->getMetaData("subtype");
			const char * szSubType = strSubType.c_str();

			std::vector<NodeDescriptor>::iterator it = std::find(m_aNodeDescriptors.begin(), m_aNodeDescriptors.end(), szSubType);
			assert(it != m_aNodeDescriptors.end());

			n = createOperationNode(*it);
		}
		else if ("comparison" == strType)
		{
			n = createComparisonNode();
		}
		else if ("texture" == strType)
		{
			const std::string & strFormat = pNode->getMetaData("format");
			unsigned int iFormat = StringToGLenum(strFormat.c_str());

			n = createTextureNode(iFormat, 1024, 1024);
		}
		else if ("float" == strType)
		{
			const std::string & strValue = pNode->getMetaData("value");
			n = createFloatNode(atof(strValue.c_str()));
		}
		else if ("present" == strType)
		{
			n = createPresentNode();
		}
		else
		{
			assert(false);
		}

		n->setPos(QPointF(atof(strNodePosX.c_str()), atof(strNodePosY.c_str())));
#if 0
		n->setSize(atof(strNodeSizeX.c_str()), atof(strNodeSizeY.c_str()));
#endif // 0

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
		GraphWidget::Node * pGraphicsSourceNode = mapUID[source_uid];
		assert(nullptr != pGraphicsSourceNode);

		GraphWidget::Node * pGraphicsTargetNode = mapUID[target_uid];
		assert(nullptr != pGraphicsTargetNode);

		GraphWidget::ConnectorOutput * pConnectorSource = pGraphicsSourceNode->getOutputConnector(iSourceId);
		assert(nullptr != pConnectorSource);

		GraphWidget::ConnectorInput * pConnectorDestination = pGraphicsTargetNode->getInputConnector(iTargetId);
		assert(nullptr != pConnectorDestination);

		GraphWidget::Edge * e = new GraphWidget::Edge(pConnectorSource, pConnectorDestination);
		m_pScene->addItem(e);
	}

	return(true);
}

/**
* @brief NodeEditorWindow::createGraph
* @param G
* @return
*/
bool NodeEditorWindow::createGraph(Graph & G)
{
	G.setType("RenderGraph");
	G.setLabel("Unnamed Render Graph");

	std::map<uintptr_t, Node *> mapUID;

	QList<QGraphicsItem*> items = m_pScene->items();

	for (QGraphicsItem * item : items)
	{
		switch (item->type())
		{
		case GraphWidget::Node::Type:
		{
			GraphWidget::Node * pNodeItem = static_cast<GraphWidget::Node*>(item);
			assert(nullptr != pNodeItem);

			char szId [16];
			sprintf(szId, "%lld", uintptr_t(pNodeItem));

			Node * pNode = new Node(szId);
			assert(nullptr != pNode);
			G.addNode(pNode);

			std::string & strNodeType = m_mapNodeType[pNodeItem];

			pNode->setType(strNodeType.c_str());

			if (strNodeType == "pass")
			{
				pNode->setLabel(((GraphWidget::NodePass*)pNodeItem)->getTitle().toLocal8Bit());
				pNode->addMetaData("subtype", (const char*)m_mapOperationNodes[pNodeItem]->identifier.toLocal8Bit());
			}
			else if (strNodeType == "texture")
			{
				pNode->addMetaData("format", GLenumToString(m_mapTextureNodes[pNodeItem]));
			}
			else if (strNodeType == "float")
			{
				pNode->addMetaData("value", ((GraphWidget::NodeFloat*)pNodeItem)->getValue());
			}

			char szPosX [16];
			sprintf(szPosX, "%f", pNodeItem->pos().x());
			pNode->addMetaData("xloc", szPosX);

			char szPosY [16];
			sprintf(szPosY, "%f", pNodeItem->pos().y());
			pNode->addMetaData("yloc", szPosY);

			char szWidth [16];
			sprintf(szWidth, "%f", pNodeItem->getWidth());
			pNode->addMetaData("width", szWidth);

			char szHeight [16];
			sprintf(szHeight, "%f", pNodeItem->getHeight());
			pNode->addMetaData("height", szHeight);

			mapUID[uintptr_t(pNodeItem)] = pNode;
		}
		break;
		}
	}

	for (QGraphicsItem * item : items)
	{
		switch (item->type())
		{
			case GraphWidget::Edge::Type:
			{
				GraphWidget::Edge * pEdgeItem = static_cast<GraphWidget::Edge*>(item);
				assert(nullptr != pEdgeItem);

				// Source
				Node * pNodeSource = mapUID[uintptr_t(pEdgeItem->getSourceNode())];
				assert(nullptr != pNodeSource);

				// Target
				Node * pNodeTarget = mapUID[uintptr_t(pEdgeItem->getDestinationNode())];
				assert(nullptr != pNodeTarget);

				Edge * pEdge = new Edge(pNodeSource, pNodeTarget);
				assert(nullptr != pEdge);
				G.addEdge(pEdge);

				pEdge->setDirected(true);

				char szSourceId [16];
				sprintf(szSourceId, "%d", pEdgeItem->getSourceIndex());
				pEdge->addMetaData("source_id", szSourceId);

				char szTargetId [16];
				sprintf(szTargetId, "%d", pEdgeItem->getDestinationIndex());
				pEdge->addMetaData("target_id", szTargetId);
			}
			break;
		}
	}

	return(true);
}

/**
* @brief NodeEditorWindow::createDefaultNodes
*/
GraphWidget::Node * NodeEditorWindow::createPresentNode(void)
{
	GraphWidget::NodeFinal * n = new GraphWidget::NodeFinal();

	m_pScene->addItem(n);

	m_mapNodeType.insert(std::pair<const GraphWidget::Node*, std::string>(n, "present"));

	return(n);
}

/**
* @brief NodeEditorWindow::createNode
* @param desc
*/
GraphWidget::Node * NodeEditorWindow::createOperationNode(const NodeDescriptor & desc)
{
	GraphWidget::NodePass * n = new GraphWidget::NodePass();

	n->setTitle(desc.name);

	m_pScene->addItem(n);

	int input_index = 0;

	for (const NodeDescriptor::Input & input : desc.inputs)
	{
		if (input.type == NodeDescriptor::Texture)
		{
			n->addInput(input.name, TYPE_TEXTURE_BIT); //, nullptr, input_index
		}
		else if (input.type == NodeDescriptor::Float)
		{
			n->addInput(input.name, TYPE_FLOAT_BIT); //, nullptr, input_index
		}
		else
		{
			assert(false);
		}

		++input_index;
	}

	int output_index = 0;

	for (const NodeDescriptor::Output & output : desc.outputs)
	{
		if (output.type == NodeDescriptor::Texture)
		{
			n->addOutput(output.name, TYPE_TEXTURE_BIT); //, nullptr, output_index
		}
		else if (output.type == NodeDescriptor::Float)
		{
			n->addOutput(output.name, TYPE_FLOAT_BIT); //, nullptr, output_index
		}
		else
		{
			assert(false);
		}

		++output_index;
	}

	m_mapOperationNodes.insert(std::pair<const GraphWidget::Node*, const NodeDescriptor*>(n, &desc));
	m_mapNodeType.insert(std::pair<const GraphWidget::Node*, std::string>(n, "pass"));

	return(n);
}

/**
* @brief NodeEditorWindow::createNode
* @param desc
*/
GraphWidget::Node * NodeEditorWindow::createTextureNode(unsigned int format, unsigned int width, unsigned int height)
{
	GraphWidget::Node * n = new GraphWidget::NodeTexture();

	m_pScene->addItem(n);

	m_mapTextureNodes.insert(std::pair<const GraphWidget::Node*, unsigned int>(n, format));
	m_mapNodeType.insert(std::pair<const GraphWidget::Node*, std::string>(n, "texture"));

	return(n);
}

/**
 * @brief NodeEditorWindow::createComparisonNode
 * @return
 */
GraphWidget::Node *	NodeEditorWindow::createComparisonNode(void)
{
	GraphWidget::Node * n = new GraphWidget::NodeComparison();

	m_pScene->addItem(n);

	m_mapNodeType.insert(std::pair<const GraphWidget::Node*, std::string>(n, "comparison"));

	return(n);
}

/**
 * @brief NodeEditorWindow::createFloatNode
 * @param value
 * @return
 */
GraphWidget::Node *	NodeEditorWindow::createFloatNode(float value)
{
	GraphWidget::Node * n = new GraphWidget::NodeFloat(value);

	m_pScene->addItem(n);

	m_mapNodeType.insert(std::pair<const GraphWidget::Node*, std::string>(n, "float"));

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
	Graph G;

	createGraph(G);

	if (G.saveToFile(GRAPH_FILE_PATH))
	{
		emit graphSaved(G);

		updateTextures();
	}
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
 * @brief NodeEditorWindow::on_actionCreateComparisonNode_triggered
 */
void NodeEditorWindow::on_actionCreateComparisonNode_triggered(void)
{
	GraphWidget::Node * node = createComparisonNode();
	QPointF pos = m_pView->mapToScene(m_lastContextMenuPos);
	node->setPos(pos);
}

/**
 * @brief NodeEditorWindow::on_actionCreateFloatNode_triggered
 */
void NodeEditorWindow::on_actionCreateFloatNode_triggered()
{
	GraphWidget::Node * node = createFloatNode();
	QPointF pos = m_pView->mapToScene(m_lastContextMenuPos);
	node->setPos(pos);
}

/**
* @brief NodeEditorWindow::on_actionRemoveNode_triggered
*/
void NodeEditorWindow::on_actionRemoveNode_triggered(void)
{
	QList<QGraphicsItem *> list = m_pScene->selectedItems();

	for (QGraphicsItem * item : list)
	{
		if (item->type() == GraphWidget::Node::Type)
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
		GraphWidget::Node * node = createOperationNode(*pDesc);
		QPointF pos = m_pView->mapToScene(m_lastContextMenuPos);
		node->setPos(pos);
	}
}

/**
* @brief NodeEditorWindow::createNodeFromWindow
*/
void NodeEditorWindow::createTextureNodeFromDialog(void)
{
	unsigned int format = m_pNodeTextureCreationWindow->getCurrentFormat();

	GraphWidget::Node * node = createTextureNode(format, 1024, 1024);
	QPointF pos = m_pView->mapToScene(m_lastContextMenuPos);
	node->setPos(pos);
}

/**
* @brief NodeEditorWindow::updateTextures
*/
void NodeEditorWindow::updateTextures(void)
{
	extern RendererWrapper g_RendererWrapper;

	QList<QGraphicsItem*> items = m_pScene->items();

	for (QGraphicsItem * item : items)
	{
		switch (item->type())
		{
			case GraphWidget::Node::Type:
			{
				GraphWidget::Node * pNodeItem = static_cast<GraphWidget::Node*>(item);
				assert(nullptr != pNodeItem);

				std::string & strNodeType = m_mapNodeType[pNodeItem];

				if (strNodeType == "texture")
				{
					GraphWidget::NodeTexture * pNodeTextureItem = (GraphWidget::NodeTexture*)pNodeItem;

					char szId [16];
					sprintf(szId, "%lld", uintptr_t(pNodeItem));

					GLuint textureId = static_cast<MainWindow*>(parent())->m_pDrawable->getRenderQueue()->getRenderTexture(szId);

					pNodeTextureItem->setTexture(textureId);
				}
			}
			break;
		}
	}
}
