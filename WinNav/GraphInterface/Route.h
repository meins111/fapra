#pragma once
#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include <vector>



class Route :
	public Graph
{
public:
	Route();
	~Route();
	Node &getNode(size_t index);
	Edge &getEdge(size_t index);
	size_t numEdges();

	//TODO: Should be made private + access mechanism -> if some time remains ..
	std::vector<OsmNode> routeNodes;
	std::vector<NavEdge> routeEdges;

};

