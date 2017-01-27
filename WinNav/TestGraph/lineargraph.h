#ifndef LINEARGRAPH_H
#define LINEARGRAPH_H

#include "GraphInterface/Edge.h"
#include "GraphInterface/Graph.h"
#include "GraphInterface/Node.h"
#include "TestGraph/PODEdge.h"
#include "TestGraph/PODNode.h"
#include <vector>
#include <stdexcept>

class LinearGraph : public Graph
{
public:
    LinearGraph();

    PODNode &getNode(size_t index);
    PODEdge &getEdge(size_t index);
    size_t numEdges();
    void insertNode(const PODNode &node);
    void insertEdge(const PODEdge &edge);
    void reset();

protected:
    std::vector<PODNode> nodes;
    std::vector<PODEdge> edges;
};

#endif // LINEARGRAPH_H
