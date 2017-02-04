#include "astar.h"

AStar::AStar(NavGraph &navGraph) : graph(navGraph)
{
    errorCode=0;
    curMedium=CAR;
    isMaxRangeSet=false;
    maxRange=0.0;
    timeIsPrio=true;
    curMaxSpeed=MAXSPEED_CAR;
}
void AStar::setMedium (TravelMedium medium) {
    curMedium=medium;
    //Set maxspeed value according to set travel medium
    if (curMedium == CAR) {
        curMaxSpeed=MAXSPEED_CAR;
    }
    else if (curMedium==BIKE) {
        curMaxSpeed==MAXSPEED_BIKE;
    }
    else {
        curMaxSpeed==MAXSPEED_FOOT;
    }
    return;
}

void AStar::updateProgress(const OpenNode_t &cur, size_t start, size_t target, CondWait_t *updateStruct) {
    if (!updateStruct) {
        //No update struct set => no update possible
        return;
    }
    //Calculate total (heuristic) distance
    NodeInfo targetNode = graph.nodeInfo.nodeData[target];
    NodeInfo startNode = graph.nodeInfo.nodeData[start];
    double startDist = startNode.getHaversineDistanceTo(targetNode);
    //Now calculate current (heursitic) distance
    double curDist = startNode.getHaversineDistanceTo(graph.nodeInfo.nodeData[cur.id]);
    //Normalize Distance to get percentage
    double curProgress = (curDist/startDist)*95;
    //Final Update (if target was found) will end with 95%, leaving the last 5% for the reconstruction of the graph
    updateStruct->updateProgress(std::round(curProgress));
    return;
}

void AStar::constructPath (size_t start, size_t target, CondWait_t *updateStruct) {
    //Reset the current route
    route.reset();
    //Pred Map empty?
    if (predecessorMap.size()==0) {
        errorCode=PREDECESSOR_MAP_EMPTY;
        if (updateStruct) {
            updateStruct->updateProgress(errorCode);
        }
        return;
    }
    //Start == target?
    if(start==target) {
        //Insert single-point
        route.insertNode(PODNode(graph.nodeInfo.nodeData[target].longitude, graph.nodeInfo.nodeData[target].latitude));
        //Signal finish
        errorCode=0;
        if (updateStruct) {
            updateStruct->updateProgress(100);
        }
        return;
    }

    bool done=false;
    size_t nodes=0;
    size_t curNode = target;
    //Insert first node
    route.insertNode(PODNode(graph.nodeInfo.nodeData[curNode].longitude, graph.nodeInfo.nodeData[curNode].latitude));
    nodes++;
    while(!done) {
        //current Target node has a predecessor?
        if (predecessorMap.count(target)==0) {
            errorCode=PREDECESSOR_MAP_BROKEN;
            if (updateStruct) {
                updateStruct->updateProgress(errorCode);
            }
            //Clear broken graph
            route.reset();
            return;
        }
        //Fetch pred ID
        size_t pred=predecessorMap[curNode];
        //Insert the predecessor into the route
        route.insertNode(PODNode(graph.nodeInfo.nodeData[pred].longitude, graph.nodeInfo.nodeData[pred].latitude));
        nodes++;
        //Check if pred = start
        if (pred == start) {
            //Finished!
            done = true;
        }
        curNode=pred;
    }
    //All path nodes added, now construct the edges backwards
    if (updateStruct) {
        updateStruct->updateProgress(97);
    }
    //insert edges : the first edge connects the last and the pre-last node (the start node and his first successor)
    for (size_t i=0; i<nodes-1; i++) {
        size_t start=nodes-1-i;
        size_t end = nodes-1-i-1;
        route.insertEdge(PODEdge(start, end));
    }
    errorCode=0;
    //Route building complete: update
    if (updateStruct) {
        updateStruct->updateProgress(100);
    }
    return;
}


void AStar::findRoute (const size_t &start, const size_t &target, CondWait_t *condStruct) {
    //Check the provided graph
    if (!checkGraph()) {
        errorCode=EMPTY_GRAPH_ERROR;
        if (condStruct) {
            //Signal Error
            condStruct->updateProgress(errorCode);
        }
        return;
    }
    //Check start/end offset for out-of-bounds
    if(start >= graph.connectGraph.nodes.size() || target >= graph.connectGraph.nodes.size()) {
        errorCode=PARAMETER_OUT_OF_BOUNDS;
        if (condStruct) {
            //Signal Error
            condStruct->updateProgress(errorCode);
        }
        return;
    }
    //Initialize sets to empty
    closedSet.clear();
    prioQueue.clear();
    openSet.clear();
    predecessorMap.clear();
    route.reset();
    //Create start 'OpenNode' with the local node ID, the heuristic cost-to-target and the total cost to this target (0 for start node)
    Handle_Type_t handle=prioQueue.emplace(start, h(start, target), 0);
    //Add the start ID and its handle to the open list
    openSet.emplace(start, handle);

    //We will continue to search until we run out of unexpanded nodes
    while (!openSet.empty()) {
        //Fetch&remove the most promising node
        OpenNode_t current = prioQueue.top();
        //Is this the target node?
        if (current.id == target) {
            errorCode=0;
            //TODO: construct path
            //Store the total cost of the calculated path
            totalCost = current.gScore;
            //TODO: send update progress
            return;
        }
        //Update progress

        //Add node ID to closed set
        closedSet.emplace (current.id);
        size_t adjacentCnt = graph.connectGraph.nodes[current.id].numberOfEdges();
        //Check all adjacent nodes of the expanded node
        for (size_t i=0; i<adjacentCnt; i++) {
            //Fetch the next adjacent node
            const NodeInfo &adjacentNode = graph.getAdjacentNode(current.id, i);
            //Is this node already closed?
            if (closedSet.find(adjacentNode.localID)== closedSet.end()) {
                //Check the next adjacent node
                continue;
            }
            EdgeInfo &adjacentEdge = graph.getAdjacentEdge(adjacentNode.localID, i);
            //Fetch the actual cost to reach the adjacent node from the current node
            double curCost = current.gScore + adjacentEdge.getEdgeCost(timeIsPrio);
            //Is the adjacent node already know with a cost lower or equal than this one?
            if (openSet.find(adjacentNode.localID) != openSet.end()){
                    //openSet[adjacentNode.localID].gScore <= curCost)

                //Yes, adjacent node already visitied with lower costs, so skip this node
                return;
            }
            //The Node is was not yet visited, so create a new entry in the lists
            if (openSet.find(adjacentNode.localID)==openSet.end()) {
                //Node is unknown: create entries in: openlist, predecessor map and prioQueue
                predecessorMap[adjacentNode.localID]=current.id;
                Handle_Type_t handle = prioQueue.emplace(adjacentNode.localID,
                                                                                    h(adjacentNode.localID, target),
                                                                                    curCost);
                openSet.emplace(adjacentNode.localID, handle);
                continue;
            }
            //Node was visited before ...
            Handle_Type_t handle = openSet[adjacentNode.localID];
            if ( (*handle).gScore <= curCost ) {
                //So we know a better path already!
                // => we can skip this node
                continue;
            }
            //... and we just found a better way to reach it (from start)!
            //Update the predecessor map entry of the adjacent node so it points to the current node
            predecessorMap[adjacentNode.localID]=current.id;
            //Create an update node to refresh the old entry of the adjacent node
            //We'll keep the id but update gScore and fScore with their new values
            OpenNode_t updateNode (adjacentNode.localID, h(adjacentNode.localID, target), curCost);
            //Update the respective node inside the prioQueue via the stored handler: update method will reconstruct
            //fib-heap to ensure correctness if necessary
            prioQueue.update(handle, updateNode);
        }
    }
    //If we end here, the openList is empty thus every node reachable from the start node was checked
    //but no (valid) path to the target node was found
    errorCode=TARGET_UNREACHABLE_ERROR;
    if(condStruct) {
        condStruct->updateProgress(errorCode);
    }
    return;
}


bool AStar::checkGraph() {
    //A graph must at least have two nodes ...
    if (graph.connectGraph.nodes.size()<2) {
        return false;
    }
    //Connected by one edge
    if (graph.connectGraph.edges.size()<1) {
        return false;
    }
    //EdgeInfo size must equal edge size
    if (graph.edgeInfo.size() != graph.connectGraph.edges.size()) {
        return false;
    }
    //Same goes for node and nodeInfo size
    if(graph.nodeInfo.nodeData.size() != graph.connectGraph.nodes.size()) {
        return false;
    }
    //Basic checks successful
    return true;
}


double AStar::h(const size_t &n1, const size_t &n2) {
    double cost = 0.0;
    cost = graph.nodeInfo.nodeData[n1].getHaversineDistanceTo(graph.nodeInfo.nodeData[n2]);
    if (timeIsPrio) {
        //Time heuristic: haversineDist divided by the max speed on any edge (130km/h)
        cost /= curMaxSpeed;
    }
    return cost;
}
