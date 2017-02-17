#ifndef _TESTGRAPH_H_
#define _TESTGRAPH_H_

#include "../GraphInterface/Graph.h"
#include "PODNode.h"
#include "PODEdge.h"

/**
 * @brief 
 */
class TestGraph : public Graph {
	PODNode nodes[4];
	PODEdge edges[3];
public:
	TestGraph();
	virtual ~TestGraph();
	virtual Node &getNode(size_t index);
	virtual Edge &getEdge(size_t index);
	virtual size_t numEdges();
};
#endif
