#pragma once

#include <string>

#include "MetaDataContainer.h"

class Node : public MetaDataContainer
{

public:

	Node(const char * szId);
	~Node(void);

	//
	// Setters
	void					setId			(const char * szId);
	void					setType			(const char * szType);
	void					setLabel		(const char * szLabel);

	//
	// Getters
	const std::string &		getId			(void) const;
	const std::string &		getType			(void) const;
	const std::string &		getLabel		(void) const;

private:

	std::string id;
	std::string type;
	std::string label;
};
