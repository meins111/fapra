#pragma once

#include "Node.h"
#include "Edge.h"
#include <cstddef>

class Graph
{
public:
	Graph();
	virtual ~Graph();

	virtual Node &getNode(size_t index) = 0;
	virtual Edge &getEdge(size_t index) = 0;
    virtual size_t numEdges() = 0;
};

