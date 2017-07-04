#include "Graph.h"

#include "Node.h"
#include "Edge.h"

#include <algorithm>

#include <jansson.h>

#include <assert.h>

/**
 * @brief Graph::Graph
 */
Graph::Graph(void)
{
	// ...
}

/**
 * @brief Graph::~Graph
 */
Graph::~Graph(void)
{
	// ...
}

/**
 * @brief Graph::loadFromFile
 * @param filename
 * @return
 */
bool Graph::loadFromFile(const std::string & filename)
{
	json_error_t err;
	json_t * pRoot = json_load_file(filename.c_str(), 0, &err);

	if (!pRoot)
	{
		return(false);
	}

	// Get the graph
	json_t * pGraph = json_object_get(pRoot, "graph");
	if (nullptr == pGraph)
	{
		return(false);
	}

	// Directed
	json_t * pGraphDirected = json_object_get(pGraph, "directed");
	if (nullptr != pGraphDirected)
	{
		bool bDirected = json_is_true(pGraphDirected);
		setDirected(bDirected);
	}

	// Type
	json_t * pGraphType = json_object_get(pGraph, "type");
	if (nullptr != pGraphType)
	{
		const char * szType = json_string_value(pGraphType);
		setType(szType);
	}

	// Label
	json_t * pGraphLabel = json_object_get(pGraph, "label");
	if (nullptr != pGraphLabel)
	{
		const char * szLabel = json_string_value(pGraphLabel);
		setLabel(szLabel);
	}

	// Metadata
	json_t * pGraphMetadata = json_object_get(pGraph, "metadata");
	if (nullptr != pGraphMetadata)
	{
		const char * key;
		json_t * value;

		json_object_foreach(pGraphMetadata, key, value)
		{
			if (json_is_string(value))
			{
				const char * szValue = json_string_value(value);
				addMetaData(key, szValue);
			}
			else if (json_is_real(value))
			{
				double fValue = json_real_value(value);
				addMetaData(key, fValue);
			}
			else if (json_is_integer(value))
			{
				json_int_t iValue = json_integer_value(value);
				addMetaData(key, iValue);
			}
		}
	}

	//
	// Load Nodes
	json_t * pGraphNodes = json_object_get(pGraph, "nodes");
	if (nullptr != pGraphNodes)
	{
		size_t index;
		json_t * pNode;

		json_array_foreach(pGraphNodes, index, pNode)
		{
			// ID
			json_t * pNodeID = json_object_get(pNode, "id");
			const char * szId = json_string_value(pNodeID);

			if (nullptr != szId)
			{
				Node * pNewNode = new Node(szId);
				nodes.push_back(pNewNode);

				// Type
				json_t * pNodeType = json_object_get(pNode, "type");
				if (nullptr != pNodeType)
				{
					const char * szType = json_string_value(pNodeType);
					pNewNode->setType(szType);
				}

				// Label
				json_t * pNodeLabel = json_object_get(pNode, "label");
				if (nullptr != pNodeLabel)
				{
					const char * szLabel = json_string_value(pNodeLabel);
					pNewNode->setLabel(szLabel);
				}

				// Metadata
				json_t * pNodeMetada = json_object_get(pNode, "metadata");
				if (nullptr != pNodeMetada)
				{
					const char * key;
					json_t * value;

					json_object_foreach(pNodeMetada, key, value)
					{
						if (json_is_string(value))
						{
							const char * szValue = json_string_value(value);
							pNewNode->addMetaData(key, szValue);
						}
						else if (json_is_real(value))
						{
							double fValue = json_real_value(value);
							pNewNode->addMetaData(key, fValue);
						}
						else if (json_is_integer(value))
						{
							json_int_t iValue = json_integer_value(value);
							pNewNode->addMetaData(key, iValue);
						}
					}
				}
			}
		}
	}

	//
	// Load Edges
	json_t * pGraphEdges = json_object_get(pGraph, "edges");
	if (nullptr != pGraphEdges)
	{
		size_t index;
		json_t * pNode;

		json_array_foreach(pGraphEdges, index, pNode)
		{
			// Source
			json_t * pEdgeSource = json_object_get(pNode, "source");
			const char * szSource = json_string_value(pEdgeSource);

			// Target
			json_t * pEdgeTarget = json_object_get(pNode, "target");
			const char * szTarget = json_string_value(pEdgeTarget);

			if (nullptr != szSource && nullptr != szTarget)
			{
				Node * pSource = findNode(szSource);
				Node * pTarget = findNode(szTarget);

				Edge * pNewEdge = new Edge(pSource, pTarget);
				edges.push_back(pNewEdge);

				// Relation
				json_t * pEdgeRelation = json_object_get(pNode, "relation");
				if (nullptr != pEdgeRelation)
				{
					const char * szRelation = json_string_value(pEdgeRelation);
					pNewEdge->setRelation(szRelation);
				}

				// Directed
				json_t * pEdgeDirected = json_object_get(pNode, "directed");
				if (nullptr != pEdgeDirected)
				{
					bool bDirected = json_is_true(pEdgeDirected);
					pNewEdge->setDirected(bDirected);
				}

				// Label
				json_t * pEdgeLabel = json_object_get(pNode, "label");
				if (nullptr != pEdgeLabel)
				{
					const char * szLabel = json_string_value(pEdgeLabel);
					pNewEdge->setLabel(szLabel);
				}

				// Metadata
				json_t * pEdgeMetada = json_object_get(pNode, "metadata");
				if (nullptr != pEdgeMetada)
				{
					const char * key;
					json_t * value;

					json_object_foreach(pEdgeMetada, key, value)
					{
						if (json_is_string(value))
						{
							const char * szValue = json_string_value(value);
							pNewEdge->addMetaData(key, szValue);
						}
						else if (json_is_real(value))
						{
							double fValue = json_real_value(value);
							pNewEdge->addMetaData(key, fValue);
						}
						else if (json_is_integer(value))
						{
							json_int_t iValue = json_integer_value(value);
							pNewEdge->addMetaData(key, iValue);
						}
					}
				}
			}
		}
	}

	return(true);
}

/**
 * @brief Graph::saveToFile
 * @param filename
 * @return
 */
bool Graph::saveToFile(const std::string & filename)
{
	json_t * pRoot = json_object();

	// Add the graph
	json_t * pGraph = json_object();
	json_object_set(pRoot, "graph", pGraph);

	// Directed
	json_t * pGraphDirected = directed ? json_true() : json_false();
	json_object_set(pGraph, "directed", pGraphDirected);

	// Type
	json_t * pGraphType = json_string(type.c_str());
	json_object_set(pGraph, "type", pGraphType);

	// Label
	json_t * pGraphLabel = json_string(label.c_str());
	json_object_set(pGraph, "label", pGraphLabel);

	// Metadata
	json_t * pGraphMetadata = json_object();
	json_object_set(pGraph, "metadata", pGraphMetadata);

	// Add the nodes/edges
	json_t * pGraphNodes = json_array();
	json_object_set(pGraph, "nodes", pGraphNodes);

	json_t * pGraphEdges = json_array();
	json_object_set(pGraph, "edges", pGraphEdges);

	for (Node * node : nodes)
	{
		json_t * pNode = json_object();
		json_array_append(pGraphNodes, pNode);

		// ID
		json_t * pNodeID = json_string(node->getId().c_str());
		json_object_set(pNode, "id", pNodeID);

		// Type
		json_t * pNodeType = json_string(node->getType().c_str());
		json_object_set(pNode, "type", pNodeType);

		// Label
		json_t * pNodeLabel = json_string(node->getLabel().c_str());
		json_object_set(pNode, "label", pNodeLabel);

		// Metadata
		json_t * pNodeMetada = json_object();
		json_object_set(pNode, "metadata", pNodeMetada);

		for (auto pair : *node)
		{
			json_t * pMeta = json_string(pair.second.c_str());
			json_object_set(pNodeMetada, pair.first.c_str(), pMeta);
		}
	}

	for (Edge * edge : edges)
	{
		json_t * pEdge = json_object();
		json_array_append(pGraphEdges, pEdge);

		// Source
		json_t * pEdgeSource = json_string(edge->getSource()->getId().c_str());
		json_object_set(pEdge, "source", pEdgeSource);

		// Target
		json_t * pEdgeTarget = json_string(edge->getTarget()->getId().c_str());
		json_object_set(pEdge, "target", pEdgeTarget);

		// Directed
		json_t * pEdgeDirected = edge->isDirected() ? json_true() : json_false();
		json_object_set(pEdge, "directed", pEdgeDirected);

		// Metadata
		json_t * pEdgeMetada = json_object();
		json_object_set(pEdge, "metadata", pEdgeMetada);

		for (auto pair : *edge)
		{
			json_t * pMeta = json_string(pair.second.c_str());
			json_object_set(pEdgeMetada, pair.first.c_str(), pMeta);
		}
	}

	int success = json_dump_file(pRoot, filename.c_str(), 0);

	return(0 == success);
}

/**
 * @brief Graph::findNode
 * @param id
 * @return
 */
Node * Graph::findNode(const std::string & id)
{
	for (Node * pNode : nodes)
	{
		if (pNode->getId() == id)
		{
			return(pNode);
		}
	}

	return(nullptr);
}

/**
 * @brief Graph::getEdgeTo
 * @param node
 * @param outEdges
 * @return
 */
bool Graph::getEdgeTo(Node * node, std::vector<Edge*> & outEdges) const
{
	for (Edge * edge : edges)
	{
		if (edge->getTarget() == node)
		{
			outEdges.push_back(edge);
		}
	}

	return(true);
}

/**
 * @brief Graph::getEdgeFrom
 * @param node
 * @param outEdges
 * @return
 */
bool Graph::getEdgeFrom(Node * node, std::vector<Edge*> & outEdges) const
{
	for (Edge * edge : edges)
	{
		if (edge->getSource() == node)
		{
			outEdges.push_back(edge);
		}
	}

	return(true);
}

/**
 * @brief Graph::setDirected
 * @param bDirected
 */
void Graph::setDirected(bool bDirected)
{
	directed = bDirected;
}

/**
 * @brief Graph::setType
 * @param szType
 */
void Graph::setType(const char * szType)
{
	type = szType;
}

/**
 * @brief Graph::setLabel
 * @param szLabel
 */
void Graph::setLabel(const char * szLabel)
{
	label = szLabel;
}

/**
 * @brief Graph::addNode
 * @param pNode
 */
void Graph::addNode(Node * pNode)
{
	nodes.push_back(pNode);
}

/**
 * @brief Graph::addEdge
 * @param pEdge
 */
void Graph::addEdge(Edge * pEdge)
{
	edges.push_back(pEdge);
}

/**
 * @brief Graph::removeNode
 * @param pNode
 */
void Graph::removeNode(Node * pNode)
{
	std::vector<Node*>::iterator it = std::find(nodes.begin(), nodes.end(), pNode);

	if (it != nodes.end())
	{
		nodes.erase(it);
	}
}

/**
 * @brief Graph::removeEdge
 * @param pEdge
 */
void Graph::removeEdge(Edge * pEdge)
{
	std::vector<Edge*>::iterator it = std::find(edges.begin(), edges.end(), pEdge);

	if (it != edges.end())
	{
		edges.erase(it);
	}
}

/**
 * @brief Graph::isDirected
 * @return
 */
bool Graph::isDirected(void) const
{
	return(directed);
}

/**
 * @brief Graph::getType
 * @return
 */
const std::string & Graph::getType(void) const
{
	return(type);
}

/**
 * @brief Graph::getLabel
 * @return
 */
const std::string & Graph::getLabel(void) const
{
	return(label);
}

/**
 * @brief Graph::getNodes
 * @return
 */
const std::vector<Node*> & Graph::getNodes(void) const
{
	return(nodes);
}

/**
 * @brief Graph::getEdges
 * @return
 */
const std::vector<Edge*> & Graph::getEdges(void) const
{
	return(edges);
}
