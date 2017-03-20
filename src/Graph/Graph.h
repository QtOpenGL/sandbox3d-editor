#pragma once

#include <vector>
#include <string>

#include "MetaDataContainer.h"

class Node;
class Edge;

class Graph : public MetaDataContainer
{

public:

	Graph(void);
	~Graph(void);

	bool loadFromFile(const std::string & filename);
	bool saveToFile(const std::string & filename);

	Node * findNode(const std::string & id);
	bool getEdgeTo(Node * node, std::vector<Edge*> & outEdges) const;
	bool getEdgeFrom(Node * node, std::vector<Edge*> & outEdges) const;

	//
	// Setters
	void						setDirected		(bool bDirected);
	void						setType			(const char * szType);
	void						setLabel		(const char * szLabel);

	void						addNode			(Node * pNode);
	void						addEdge			(Edge * pEdge);

	void						removeNode		(Node * pNode);
	void						removeEdge		(Edge * pEdge);

	//
	// Getters
	bool						isDirected		(void) const;
	const std::string &			getType			(void) const;
	const std::string &			getLabel		(void) const;

	const std::vector<Node*> &	getNodes		(void) const;
	const std::vector<Edge*> &	getEdges		(void) const;

private:

	bool directed;
	std::string type;
	std::string label;
	std::vector<Node*> nodes;
	std::vector<Edge*> edges;

};
