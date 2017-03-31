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

double NavGraph::getEdgeCost (BasicEdge &edge, bool timeIsPrio, const double &mediumMaxSpeed) {
    EdgeInfo &info=edgeInfo[edge.edgeInfoId];
    double curSpeed=info.speed;
    //If a medium max speed is set, we'll take the lower of the medium max speed
    // and the allowed speed of this edge for the actual speed for the calculation
    //NOTE: This is important to make sure the cost for an edge are not cheaper than possible
    if(mediumMaxSpeed>0.0) {
        curSpeed = std::min<double> (mediumMaxSpeed, curSpeed);
    }
    //Calculate the cost according to the priority selection
    if (timeIsPrio && curSpeed > 0.0) {
        return edgeCost[edge.index]/curSpeed;
    }
    else if (timeIsPrio && curSpeed <= 0.0) {
        //Invalid speed tag
        return 0xFFFFFFFF;
    }
    else {
        //Return the travel distance
        return edgeCost[edge.index];
    }
}

double NavGraph::getEdgeTravelTime(BasicEdge &edge, const double &mediumMaxSpeed) {
    double distance = edgeCost[edge.index];
    double speed =edgeInfo[edge.edgeInfoId].speed;
    //Take minimum from given maxspeed and the maxspeed value of the edge
    if(mediumMaxSpeed>0.0) {
        speed = std::min<double> (mediumMaxSpeed, speed);
    }
    //Travel time = distance/speed
    return distance/speed;
}

NodeInfo& NavGraph::getClosestNode (const NodeInfo &curPos) {
    size_t closestNodeOffset=0xFFFFFFFF;
    double origin [2] = {curPos.longitude, curPos.latitude};
    double distToClosest=0.0;
    nanoflann::KNNResultSet<double> resSet(1);
    resSet.init(&closestNodeOffset, &distToClosest);
    closenessTree.findNeighbors(resSet, &origin[0], nanoflann::SearchParams(10));
    if (closestNodeOffset >= connectGraph.nodes.size()) {
        throw (std::runtime_error("Computed Neighbour out-of-bounds!"));
    }
    else {
        return nodeInfo.nodeData[closestNodeOffset];
    }
}

NodeInfo& NavGraph::getClosestNode(const double &lon, const double &lat, TravelMedium accessible) {
    NodeInfo curPos;
    curPos.longitude=lon;
    curPos.latitude=lat;
    return getClosestNode(curPos, accessible);
}

NodeInfo& NavGraph::getClosestNode(const NodeInfo &curPos, TravelMedium accessible) {
    bool foundNode=false;
    double radius=10;   //m
    long int resCnt=0, resOld=0;
    std::vector <size_t> results;
    while (!foundNode) {
        resCnt=getNodesWithinRadius(curPos, radius, results);
        if (resCnt==resOld) {
            //No new nodes were found in that last radius increment, so retry with a doubled radius
            results.clear();
            radius *=2;
            continue;
        }
        else if (resCnt == connectGraph.nodes.size()) {
            //No node in the graph is accessible by the medium, so throw a exception
            throw (std::logic_error("getClosestNode: No Node found which is accessible by the provided Travel Medium!"));
        }
        //There are some new points to analyze!
        //REMEMBER: Nodes are ordered by distance, so first match = searched point
        for (size_t i=resOld; i<resCnt; i++ ) {
            //Fetch the currents node
            size_t curNodeId = results[i];
            BasicNode &curBasicNode = connectGraph.nodes[curNodeId];
            NodeInfo &curNode = nodeInfo.nodeData[curNodeId];
            //Check each of it's adjazent nodes, whether it is reachable by the given medium or not
            for (size_t j=0; j<curBasicNode.numberOfEdges(); j++) {
                BasicEdge &curBasicEdge = connectGraph.edges[curBasicNode.firstEdge+j];
                EdgeInfo &curEdgeInfo = edgeInfo[curBasicEdge.edgeInfoId];
                if ((curEdgeInfo.allowance & accessible) != 0) {
                    //Yep, this node has at least one adjacent edge, that allows the given medium, so we have a hit!
                    return curNode;
                }
            }
        }
        //If we ever end here, none of the newly fetched nodes, can be accessed by the medium -> retry with doubled radius!
        results.clear();
        radius *=2;
        resOld=resCnt;
        continue;
    }
}

NodeInfo& NavGraph::getClosestNode (const double &lon, const double &lat) {
    NodeInfo cur;
    cur.longitude = lon;
    cur.latitude = lat;
    return getClosestNode(cur);
}

void NavGraph::buildClosenessTree() {
    closenessTree.buildIndex();
}


NodeInfo& NavGraph::getAdjacentNode (const size_t curNode, size_t offset) {
    //The node we are looking at
    BasicNode &cur = connectGraph.nodes[curNode];
    //The edge we are interested in
    BasicEdge &edge =connectGraph.edges[cur.firstEdge+offset];
    //The adjacent node = the end node of the respective edge
    return nodeInfo.nodeData[edge.endNode];
}

BasicEdge& NavGraph::getAdjacentEdge (const size_t curNode, size_t offset) {
    //The node we are looking at
    BasicNode &cur = connectGraph.nodes[curNode];
    //Index check
    if (cur.firstEdge+offset >= connectGraph.edges.size()) {
        throw (std::invalid_argument("Index+Offset out of bounds!"));
    }
    //The edge we are interested in
    return connectGraph.edges[cur.firstEdge+offset];
}

EdgeInfo& NavGraph::getAdjacentEdgeInfo (const size_t curNode, size_t offset) {
    BasicEdge &cur = getAdjacentEdge(curNode, offset);
    return edgeInfo[cur.edgeInfoId];
}



size_t NavGraph::getEdgeOffsetBetweenNodes (const size_t start, size_t target) {
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

BasicEdge& NavGraph::getEdgeBetweenNodes(const size_t start, size_t target) {
    size_t offset = getEdgeOffsetBetweenNodes(start, target);
    if (offset==0xFFFFFFFF) {
        throw(std::invalid_argument("No node between the given nodes!"));
    }
    return connectGraph.edges[offset];
}

EdgeInfo& NavGraph::getEdgeInfoBetweenNodes(const size_t start, size_t target) {
    BasicEdge &basicEdge = getEdgeBetweenNodes(start, target);
    return edgeInfo[basicEdge.edgeInfoId];
}

void NavGraph::calculateEdgeDistances() {
    //One loop to calculate all the distances
    for (size_t i=0; i<connectGraph.edges.size(); i++) {
        //The edge we are currently looking at
        BasicEdge &curEdge = connectGraph.edges[i];
        //The nodes it connects
        NodeInfo &startNode = nodeInfo.nodeData[curEdge.startNode];
        NodeInfo &targetNode = nodeInfo.nodeData[curEdge.endNode];
        //Calculate the distance this edge covers
        double dist=startNode.getHaversineDistanceTo(targetNode);
        edgeCost.emplace_back(dist);
    }
}


void NavGraph::clear() {
    connectGraph.nodes.clear();
    connectGraph.edges.clear();
    edgeInfo.clear();
    nodeInfo.nodeData.clear();
}


/*
const num_t search_radius = static_cast<num_t>(0.1);
std::vector<std::pair<size_t,num_t> >   ret_matches;

nanoflann::SearchParams params;
//params.sorted = false;
const size_t nMatches = index.radiusSearch(&query_pt[0],search_radius, ret_matches, params);
*/

long int NavGraph::getNodesWithinRadius (const NodeInfo &curPos, double radius, std::vector<size_t> &closeNodeIds) const{
    std::vector< std::pair<size_t, double> > matches;
    //We want to get the nodes sorted by their distance to the target, starting with the closest one
    nanoflann::SearchParams params;
    double origin[2] = {curPos.longitude, curPos.latitude};
    params.sorted=true;
    size_t matchCnt = closenessTree.radiusSearch(origin, radius, matches, params);
    if (matchCnt>0) {
        closeNodeIds.clear();
        auto it=matches.begin();
        //Copy out the node IDs of the closest nodes
        for (;it!=matches.end(); ++it) {
            closeNodeIds.emplace_back(it->first);
        }
        return matchCnt;
    }
    //No matches found (within given radius)
    return 0;

}
