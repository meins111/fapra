#include "navgraph.h"


Node & NavGraph::getNode(size_t index) {
    //Index Check
    if (index > nodeInfo.nodeData.size()) {
        throw (std::invalid_argument("Index out of bounds!"));
    }
    else {
        return nodeInfo.nodeData[index];
    }
}

Edge & NavGraph::getEdge(size_t index) {
    //Index Check
    if (index > edgeInfo.size()) {
        throw (std::invalid_argument("Index out of bounds!"));
    }
    else {
        return connectGraph.edges[index];
    }
}

size_t NavGraph::numEdges() {
    return edgeInfo.size();
}


const NodeInfo& NavGraph::getClosestNode (const NodeInfo &curPos) {
    size_t closestNodeOffset=0xFFFFFFFF;
    double origin [2] = {curPos.latitude, curPos.latitude};
    double distToClosest=0.0;
    //Search the closest node of the nodeInfo vector and store its index and distance to the given target
    closenessTree.knnSearch(&origin[0], 1, &closestNodeOffset, &distToClosest);
    return nodeInfo.nodeData[closestNodeOffset];
}

const NodeInfo& NavGraph::getClosestNode (const double &lon, const double &lat) {
    NodeInfo cur;
    cur.longitude = lon;
    cur.latitude = lat;
    return getClosestNode(cur);
}

void NavGraph::buildClosenessTree() {
    closenessTree.buildIndex();
}
