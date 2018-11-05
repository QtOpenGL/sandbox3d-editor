#include "SerializeGraph.h"

#include "Graph.h"
#include "Node.h"
#include "Edge.h"

#include <algorithm>

#include <assert.h>

#define SERIALIZE_FOR_QT 1

static void createGraphAlias(const Graph & graph, Graph & outGraph)
{
//	outGraph.setDirected(graph.isDirected());
//	outGraph.setLabel(graph.getLabel());
//	outGraph.setType(graph.getType());

	for (Node * node : graph.getNodes())
	{
		outGraph.addNode(node);
	}

	for (Edge * edge : graph.getEdges())
	{
		outGraph.addEdge(edge);
	}
}

static bool ApplyTopologicalOrdering(const Graph & graph, std::vector<Node*> & L, Node * n)
{
	Graph G;

	createGraphAlias(graph, G);

	std::vector<Node*> S; S.push_back(n); // S ← Set of all nodes with no incoming edges

	while (!S.empty())
	{
		Node * n = S.back(); S.pop_back(); // remove a node n from S

		L.push_back(n); // add n to tail of L

		std::vector<Edge*> edges;
		G.getEdgeTo(n, edges);

		for (Edge * e : edges)
		{
			Node * m = e->getSource(); // for each node m with an edge e from n to m do

			G.removeEdge(e); // remove edge e from the graph

			std::vector<Edge*> tmp;
			G.getEdgeFrom(m, tmp);

			if (tmp.empty()) //	if m has no other incoming edges then
			{
				S.push_back(m); // insert m into S
			}
		}
	}

	//
	// Check for cycles
	{
		std::vector<Edge*> edges = G.getEdges();

		if (!edges.empty()) // (graph has edges)
		{
			return(false); // error (graph has at least one cycle)
		}
	}

	return(true);
}

bool SerializeGraph(const Graph & graph, const std::vector<NodeDescriptor> & descriptors, std::string & strFinalTextureId)
{
	FILE * f = fopen("/tmp/render.xml", "w");

	Node * pNodePresent = nullptr;
	std::vector<Node*> aNodesTexture;

	int operations = 1; // at least present

	for (Node * node : graph.getNodes())
	{
		if (node->getType() == "present")
		{
			assert(nullptr == pNodePresent);
			pNodePresent = node;
		}
		else if (node->getType() == "texture")
		{
			aNodesTexture.push_back(node);
		}
		else if (node->getType() == "pass")
		{
			++operations;
		}
		else
		{
			assert(false);
		}
	}

	assert(nullptr != pNodePresent);


	// ----------------------------------------------------------------------------------------

	std::vector<Node*> queue;
	queue.reserve(operations);

	if (!ApplyTopologicalOrdering(graph, queue, pNodePresent))
	{
		return(false);
	}

	// ----------------------------------------------------------------------------------------

	std::vector<Edge*> edges;
	graph.getEdgeTo(pNodePresent, edges);

	if (1 != edges.size())
	{
		return(false);
	}

	Node * pDefaultFramebuffer = edges[0]->getSource();

	strFinalTextureId = pDefaultFramebuffer->getId();

	// ----------------------------------------------------------------------------------------

	std::map<Node*, int> mapNodeFB;

	// ----------------------------------------------------------------------------------------

	fprintf(f, "<?xml version=\"1.0\"?>\n");
	fprintf(f, "<!DOCTYPE render SYSTEM \"render.dtd\">\n");
	fprintf(f, "<render>\n");

	// ----------------------------------------------------------------------------------------

	fprintf(f, "\t<targets>\n");

	for (Node * node : aNodesTexture)
	{
#if !SERIALIZE_FOR_QT
		if (node != pDefaultFramebuffer)
#endif // !SERIALIZE_FOR_QT
		{
			fprintf(f, "\t\t<texture name=\"%s\" format=\"%s\" />\n", node->getId().c_str(), node->getMetaData("format").c_str());
		}
	}

	fprintf(f, "\t</targets>\n");


	// ----------------------------------------------------------------------------------------

	fprintf(f, "\t<framebuffers>\n");

	int id = 1;

	for (Node * node : queue)
	{
		if (node->getType() == "pass")
		{
			std::vector<Edge*> outEdges;
			graph.getEdgeFrom(node, outEdges);

#if !SERIALIZE_FOR_QT
			bool bDefaultFB = true;

			for (Edge * edge : outEdges)
			{
				Node * target = edge->getTarget();

				if (target != pDefaultFramebuffer)
				{
					bDefaultFB = false;
				}
			}

			if (bDefaultFB)
			{
				mapNodeFB[node] = 0;
			}
			else
#endif // !SERIALIZE_FOR_QT
			{
				fprintf(f, "\t\t<framebuffer name=\"%d\">\n", id);

				for (Edge * edge : outEdges)
				{
					Node * target = edge->getTarget();
					assert(target->getType() == "texture");
					fprintf(f, "\t\t\t<output texture=\"%s\" />\n", target->getId().c_str());
				}

				fprintf(f, "\t\t</framebuffer>\n");

				mapNodeFB[node] = id;
				++id;
			}
		}
	}

	fprintf(f, "\t</framebuffers>\n");

	// ----------------------------------------------------------------------------------------

	fprintf(f, "\t<queue>\n");

	for (std::vector<Node*>::reverse_iterator it = queue.rbegin(); it != queue.rend(); ++it)
	{
		Node * node = *it;

		if (node->getType() == "pass")
		{
			const std::string & strSubType = node->getMetaData("subtype");

			int fbId = mapNodeFB[node];

#if !SERIALIZE_FOR_QT
			if (0 == fbId)
			{
				fprintf(f, "\t\t<element type=\"%s\" framebuffer=\"default\">\n", strSubType.c_str());
			}
			else
#endif // !SERIALIZE_FOR_QT
			{
				fprintf(f, "\t\t<element type=\"%s\" framebuffer=\"%d\">\n", strSubType.c_str(), fbId);
			}

			std::vector<Edge*> inEdges;
			graph.getEdgeTo(node, inEdges);

			for (Edge * edge : inEdges)
			{
				std::vector<NodeDescriptor>::const_iterator it = std::find(descriptors.begin(), descriptors.end(), strSubType.c_str());

				const NodeDescriptor & descriptor = *it;

				if (it != descriptors.end())
				{
					const std::string & strTargetId = edge->getMetaData("target_id");
					const NodeDescriptor::Input & input = descriptor.inputs[atoi(strTargetId.c_str())];

					Node * source = edge->getSource();
					assert(source->getType() == "texture"); // remove this when constant parameters will be implemented
					fprintf(f, "\t\t\t<param name=\"%s\" value=\"%s\" />\n", (const char*)input.identifier.toLocal8Bit(), source->getId().c_str());
				}
			}

			fprintf(f, "\t\t</element>\n");
		}
	}

	fprintf(f, "\t</queue>\n");

	// ----------------------------------------------------------------------------------------

	fprintf(f, "</render>\n\n");

	// ----------------------------------------------------------------------------------------

	fclose(f);

	return(true);
}
