#pragma once

#include "NodeDescription.h"

class Graph;

bool SerializeGraph(const Graph & graph, const std::vector<NodeDescriptor> & descriptors, std::string & strFinalTextureId);
