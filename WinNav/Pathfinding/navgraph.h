#ifndef NAVGRAPH_H
#define NAVGRAPH_H

#include "../GraphInterface/Graph.h"
#include "edgeinfo.h"
#include "closenesstree.h"
#include <nanoflann.hpp>
#include "basicgraph.h"
#include <vector>

class NavGraph : public Graph
{
public:
    NavGraph() : closenessTree(2, nodeInfo, nanoflann::KDTreeSingleIndexAdaptorParams(10)) {}
    NavGraph(const NavGraph &copy) : connectGraph(copy.connectGraph), edgeInfo(copy.edgeInfo), nodeInfo(copy.nodeInfo),
                                     closenessTree(2, nodeInfo, nanoflann::KDTreeSingleIndexAdaptorParams(10)) {}

    // The basic graph with only the most basic informations to represent a directed graph in offset array representation
    BasicGraph connectGraph;

    // vector<NodeInfo> wrapper including nanoflann closeness tree interface for closeness-lookups
    NodeCloud nodeInfo;

    //KD-Tree holding closeness relations of the nodeInfo
    ClosenessTree_t closenessTree;

    // Additional edge informations such as speed limits, travel medium constraints etc.
    std::vector<EdgeInfo> edgeInfo;


    /* Graph Interface */
    Node &getNode(size_t index);
    Edge &getEdge(size_t index);
    size_t numEdges();

    /* Use this method to get the closest Navigation Node for a arbitrary nodeInfo node */
    NodeInfo& getClosestNode (const NodeInfo &curPos);

    /* Arbitrary node wrapper of the getClosestNode-method */
    NodeInfo& getClosestNode (const double &lon, const double &lat);

    NodeInfo& getAdjacentNode (const size_t curNode, size_t offset);

    EdgeInfo& getAdjacentEdge (const size_t curNode, size_t offset);

    size_t getEdgeBetweenNodes (const size_t start, size_t target);

    /* Closeness Tree preparation method: must be called once, before calling getClosestNode methods */
    void buildClosenessTree();


};

#endif // NAVGRAPH_H
