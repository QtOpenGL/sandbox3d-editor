#include "Node.h"

/**
 * @brief Constructor
 */
Node::Node(const char * szId) : id(szId)
{
	// ...
}

/**
 * @brief Destructor
 */
Node::~Node(void)
{
	// ...
}

/**
 * @brief Node::setId
 * @param szId
 */
void Node::setId(const char * szId)
{
	id = szId;
}

/**
 * @brief Node::setType
 * @param szType
 */
void Node::setType(const char * szType)
{
	type = szType;
}

/**
 * @brief Node::setLabel
 * @param szLabel
 */
void Node::setLabel(const char * szLabel)
{
	label = szLabel;
}


/**
 * @brief Node::getId
 * @return
 */
const std::string & Node::getId(void) const
{
	return(id);
}

/**
 * @brief Node::getType
 * @return
 */
const std::string & Node::getType(void) const
{
	return(type);
}

/**
 * @brief Node::getLabel
 * @return
 */
const std::string & Node::getLabel(void) const
{
	return(label);
}
