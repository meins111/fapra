#ifndef NAVGRAPH_H
#define NAVGRAPH_H

#include "../GraphInterface/Graph.h"
#include "edgeinfo.h"
#include "closenesstree.h"
#include <nanoflann.hpp>
#include "basicgraph.h"
#include <stdexcept>
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

    //Partial Edge distance vector, NOTE: This has to be 1:1 match to the connectGraph.edges in contrast to the edgeInfo-data structure
    std::vector<double> edgeCost;


    /* Graph Interface */
    Node &getNode(size_t index);
    Edge &getEdge(size_t index);
    size_t numEdges();
    size_t numNodes () { return connectGraph.nodes.size(); }

    /* Utility method to get the
     *
     */
    double getEdgeCost (BasicEdge &edge, bool timeIsPrio, const double &mediumMaxSpeed);

    double getEdgeTravelTime(BasicEdge &edge, const double &mediumMaxSpeed);

    /* Use this method to get the closest Navigation Node for a arbitrary nodeInfo node */
    NodeInfo& getClosestNode (const NodeInfo &curPos);

    long int getNodesWithinRadius (const NodeInfo &curPos, double radius, std::vector<size_t> &closeNodeIds) const;

    /* Arbitrary node wrapper of the getClosestNode-method */
    NodeInfo& getClosestNode (const double &lon, const double &lat);

    /* Utility method to fetch the NodeInfo of the offset's adjacent node
     * \param curNode The node to search adjacent nodes from.
     * \param offset This indicates the offset of the adjacent nodes.
     * \return The NodeInfo of the adjacent node.
     * Throws invalid_argument exception if the offset is out of bounds.
     */
    NodeInfo& getAdjacentNode (const size_t curNode, size_t offset);


    /* Utility method to fetch the edgeInfo of the x-th adjacent edge of a node
     * \returns The edge info of the offset's edge of the curNode
     * THROWS invalid_argument exception upon index violation
     */
    EdgeInfo& getAdjacentEdgeInfo (const size_t curNode, size_t offset);

    /* returns the basic edge of the offset's edge of the curNode.
     * THROWS invalid_argument exception upon index violation
     */
    BasicEdge& getAdjacentEdge (const size_t curNode, size_t offset);

    /* Utility method to to get the offset ID of the basic edge connecting the start and target node.
     * \Returns The Offset of the BasicEdge connecting the nodes. 0xFFFFFFFF if there is no edge between the nodes.
     */
    size_t getEdgeOffsetBetweenNodes (const size_t start, size_t target);

    /* Utiliy method to get the BasicEdge connecting the start and target node
     * \return The BasicEdge connecting the two nodes.
     * Throws: invalid_input if no edge connects the two nodes.
     */
    BasicEdge& getEdgeBetweenNodes(const size_t start, size_t target);

    /* Utiliy method to fetch the Edge info of the edge connecting the start and target node
     * \return The EdgeInfo instance of the edge connecting the two nodes.
     * Throws: invalid_input if no edge connects the two nodes.
     */
    EdgeInfo& getEdgeInfoBetweenNodes(const size_t start, size_t target);



    /* Closeness Tree preparation method: must be called once, before calling getClosestNode methods */
    void buildClosenessTree();

    /* Calculate the edge distances - Call this method after any sorting of the connectGraph.edges happened */
    void calculateEdgeDistances();


    void clear();


};

#endif // NAVGRAPH_H
