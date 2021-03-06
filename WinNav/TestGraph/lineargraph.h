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
    LinearGraph (LinearGraph &copy);
    LinearGraph & operator= (LinearGraph &copy);
    PODNode &getNode(size_t index);
    PODEdge &getEdge(size_t index);
    size_t numEdges() { return edges.size(); }
    size_t numNodes()  { return nodes.size(); }
    void insertNode(const PODNode &node);
    void insertNodes(const std::vector<PODNode> &nodeVect);
    void insertEdge(const PODEdge &edge);
    void insertEdges(const std::vector<PODEdge> &edgeVect);
    void reset();
    void merge (LinearGraph &brother);
    void setEdgeType(edgeType_t type);
protected:
    std::vector<PODNode> nodes;
    std::vector<PODEdge> edges;
};

#endif // LINEARGRAPH_H
