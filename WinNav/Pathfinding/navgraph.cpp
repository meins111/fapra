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

const NodeInfo& NavGraph::getAdjacentNode (const size_t curNode, size_t offset) {
    //The node we are looking at
    BasicNode &cur = connectGraph.nodes[curNode];
    //The edge we are interested in
    BasicEdge &edge =connectGraph.edges[cur.firstEdge+offset];
    //The adjacent node = the end node of the respective edge
    return nodeInfo.nodeData[edge.endNode];
}

const EdgeInfo& NavGraph::getAdjacentEdge (const size_t curNode, size_t offset) {
    //The node we are looking at
    BasicNode &cur = connectGraph.nodes[curNode];
    //The edge we are interested in
    return edgeInfo[cur.firstEdge+offset];
}

const size_t NavGraph::getEdgeBetweenNodes (const size_t start, size_t target) {
    BasicNode startNode = connectGraph.nodes[start];
    size_t firstEdge = connectGraph.nodes[start].firstEdge;
    size_t edgeCnt = startNode.numberOfEdges();
    //Loop over all adjacent edges of the start node ...
    for (size_t i=firstEdge; i<firstEdge+edgeCnt; i++) {
        //Check every edge if it connects start and target
        if (connectGraph.edges[i].endNode == target) {
            //Found the correct edge: return its offset
            return i;
        }
    }
    //If we end here, we did not find a connection between start and target: return max value
    return 0xFFFFFFFF;
}
