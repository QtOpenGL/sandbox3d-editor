#pragma once

#include <string>

#include "MetaDataContainer.h"

class Node;

class Edge : public MetaDataContainer
{

public:

	Edge(Node * pSource, Node * pTarget);
	~Edge(void);

	//
	// Setters
	void					getSource			(Node * pSource);
	void					getTarget			(Node * pTarget);
	void					setRelation			(const char * szRelation);
	void					setDirected			(bool bDirected);
	void					setLabel			(const char * szLabel);

	//
	// Getters
	Node *					getSource			(void) const;
	Node *					getTarget			(void) const;
	const std::string &		getRelation			(void) const;
	bool					isDirected			(void) const;
	const std::string &		getLabel			(void) const;

private:

	Node * source;
	Node * target;
	std::string relation;
	bool directed;
	std::string label;

};
