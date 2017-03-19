#include "Edge.h"

/**
 * @brief Constructor
 */
Edge::Edge(Node * pSource, Node * pTarget) : source(pSource), target(pTarget)
{
	// ...
}

/**
 * @brief Destructor
 */
Edge::~Edge(void)
{
	// ...
}

/**
 * @brief Edge::getSource
 * @param pSource
 */
void Edge::getSource(Node * pSource)
{
	source = pSource;
}

/**
 * @brief Edge::getTarget
 * @param pTarget
 */
void Edge::getTarget(Node * pTarget)
{
	target = pTarget;
}

/**
 * @brief Edge::setType
 * @param szRelation
 */
void Edge::setRelation(const char * szRelation)
{
	relation = szRelation;
}

/**
 * @brief Edge::setDirected
 * @param szType
 */
void Edge::setDirected(bool bDirected)
{
	directed = bDirected;
}

/**
 * @brief Edge::setLabel
 * @param szLabel
 */
void Edge::setLabel(const char * szLabel)
{
	label = szLabel;
}

/**
 * @brief Edge::getSource
 * @return
 */
Node * Edge::getSource(void) const
{
	return(source);
}

/**
 * @brief Edge::getTarget
 * @return
 */
Node * Edge::getTarget(void) const
{
	return(target);
}

/**
 * @brief Edge::getRelation
 * @param szType
 * @return
 */
const std::string & Edge::getRelation(void) const
{
	return(relation);
}

/**
 * @brief Edge::isDirected
 * @param bDirected
 * @return
 */
bool Edge::isDirected(void) const
{
	return(directed);
}

/**
 * @brief Edge::getLabel
 * @param szLabel
 * @return
 */
const std::string & Edge::getLabel(void) const
{
	return(label);
}
