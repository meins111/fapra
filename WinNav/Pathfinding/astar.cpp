#include "astar.h"

AStar::AStar(NavGraph &navGraph) : graph(navGraph)
{
    errorCode=0;
    curMedium=CAR;
    isMaxRangeSet=false;
    routeIsBuild=false;
    maxRange=0.0;
    timeIsPrio=true;
    keepNavNodes=false;
    curMaxSpeed=MAXSPEED_CAR_MPS;
}
void AStar::setMedium (TravelMedium medium) {
    curMedium=medium;
    //Set maxspeed value according to set travel medium
    if (curMedium == CAR) {
        curMaxSpeed=MAXSPEED_CAR_MPS;
    }
    else if (curMedium==BIKE) {
        curMaxSpeed==MAXSPEED_BIKE_MPS;
    }
    else {
        curMaxSpeed==MAXSPEED_FOOT_MPS;
    }
    return;
}

void AStar::getNavNodesOnRoute(std::vector<size_t> &nodes) {
    if (errorCode<0 || route.numNodes() == 0) {
        //No route yet
        return;
    }
    nodes.clear();
    //Add all node to the vector
    for (size_t i=0; i<route.numNodes(); i++) {
        nodes.emplace_back();
    }

}

void AStar::calculateProgress(const OpenNode_t &cur, size_t start, size_t target, CondWait_t *updateStruct) {
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
    double curProgress = (curDist/startDist)*100;
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
    //initi infos
    size_t curNodeId = target,
            predNodeId=0xFFFFFFF;
    bool done=false;
    NodeInfo curNode = graph.nodeInfo.nodeData[curNodeId];
    NodeInfo predNode;
    totalDistance=0.0;
    totalTime=0.0;

    //Insert start node
    route.insertNode(PODNode(curNode.longitude, curNode.latitude));
    if (keepNavNodes) {
        routeNavNodes.clear();
        routeNavEdges.clear();
        routeNavNodes.emplace_back(curNodeId);
    }
    //Start == target?
    if(start==target) {
        done=true;
    }
    try {
        //Node counter, we start with the target node
        size_t nodes=1;
        while(!done) {
            //current node has a predecessor? Then we are ether done or the map is broken!
            if (predecessorMap.find(curNodeId)==predecessorMap.end()) {
                //Are we done? This is: current = start node
                if (curNodeId == start) {
                    done = true;
                    continue;
                }
                //This is not good. We are not yet done but the node has to pred entry
                errorCode=PREDECESSOR_MAP_BROKEN;
                if (updateStruct) {
                    updateStruct->updateProgress(errorCode);
                }
                //Clear broken graph
                route.reset();
                //and exit
                return;
            }
            //So there is a predecessor!
            //Fetch its ID
            predNodeId=predecessorMap[curNodeId];
            //Fetch the pred Node
            predNode = graph.nodeInfo.nodeData[predNodeId];
            //Insert the predecessor into the route
            route.insertNode(PODNode(predNode.longitude, predNode.latitude));
            BasicEdge &curEdge = graph.getEdgeBetweenNodes(predNodeId, curNodeId);
            //calcuate costs: distance & travel time
            totalDistance+=graph.edgeCost[curEdge.index];
            totalTime+=graph.getEdgeTravelTime(curEdge, curMaxSpeed);
            //Store node ids along the way for later use
            if (keepNavNodes) {
                routeNavNodes.emplace_back(predNodeId);
                routeNavEdges.emplace_back(curNodeId);
            }
            nodes++;
            //Set cur = pred and continue
            curNodeId=predNodeId;
            curNode = predNode;
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
        /* Debug Print to check edge properties, espacially the speed tag which was problematic for some time*/
        //Print the edge infos of all path edges
        /*
        for (long i=routeNavEdges.size()-1; i>0; i--) {
            BasicEdge &cur = graph.connectGraph.edges[routeNavEdges[i]];
            graph.edgeInfo[cur.edgeInfoId].print();
        }
        */

    }
    catch (std::invalid_argument e) {
        //An edge is missing
        routeIsBuild=false;
        errorCode=BUILDING_THE_PATH_FAILED;
        route.reset();
        if (updateStruct)
            updateStruct->updateProgress(BUILDING_THE_PATH_FAILED);
        return;

    }
    //Exception free -> build successful -> notify
    errorCode=0;
    //Route building complete: update
    if (updateStruct) {
        updateStruct->updateProgress(100);
    }
    routeIsBuild=true;
    return;
}

void AStar::getRoute(LinearGraph &retRoute) {
    if (routeIsBuild) {
        retRoute=route;
        return;
    }
}


void AStar::findRoute (const size_t &start, const size_t &target, CondWait_t *condStruct, bool emitDoneSignal) {
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
    //Sanity check of maxrange
    if (maxRange < 0.5) {
        isMaxRangeSet=false;
    }
    el::Logger* logger = el::Loggers::getLogger("default");
    //Initialize sets to empty
    closedSet.clear();
    prioQueue.clear();
    openSet.clear();
    predecessorMap.clear();
    route.reset();
    routeIsBuild=false;
    //Create start 'OpenNode' with the local node ID, the heuristic cost-to-target and the total cost to this target (0 for start node)
    Handle_Type_t startHandle=prioQueue.emplace(start, h(start, target), 0);
    //Add the start ID and its handle to the open list
    openSet.emplace(start, startHandle);
    expandedNodes.emplace_back(graph.nodeInfo.nodeData[start]);
    //Progress counters we'll use to time update notifications
    double counterSteps=h(start, target)/100;
    double counterNext=h(start,target)-counterSteps;
    int progress = 0;
    //We will continue to search until we run out of unexpanded nodes
    while (!openSet.empty()) {
        //Fetch the most promising node
        OpenNode_t current = prioQueue.top();
        //And remove it - as we only have to expand each node once
        prioQueue.pop();
        openSet.erase(current.id);
        //Add its node ID to closed set
        closedSet.emplace (current.id);
        //Is this the target node?
        if (current.id == target) {
            errorCode=0;
            if (condStruct) {
                //Almost done, just have to reconstruct the path, then we're done!
                condStruct->updateProgress(99);
            }
            //Build up the shortest path recursively
            constructPath(start, target, NULL);
            //Store the total cost of the calculated path
            totalCost = current.gScore;
            if (condStruct && emitDoneSignal) {
                //Make sure to emit 'Parse Done' Signal, but only if we are allowed to do so
                condStruct->updateProgress(100);
            }
            return;
        }
        //Send a progress notification according to the counters
        if (condStruct && (h(current.id,target) < counterNext)) {
            counterNext -= counterSteps;
            ++progress;
            logger->info("Routing :: Update Progress :: %v", progress);
            condStruct->updateProgress(progress);
        }
        //Number of adjacent edges of the current node
        size_t adjacentCnt = graph.connectGraph.nodes[current.id].numberOfEdges();
        //Check all adjacent nodes of the expanded node
        for (size_t i=0; i<adjacentCnt; i++) {
            //Fetch the next adjacent node
            const NodeInfo &adjacentNode = graph.getAdjacentNode(current.id, i);
            //Is this node already closed?
            if (closedSet.find(adjacentNode.localID)!= closedSet.end()) {
                //Check the next adjacent node
                continue;
            }
            BasicEdge &adjacentEdge = graph.getAdjacentEdge(adjacentNode.localID, i);
            EdgeInfo &adjacentEdgeInfo = graph.edgeInfo[adjacentEdge.edgeInfoId];
            //Check if the current travel medium is allowed to travel on this edge at all
            if ((adjacentEdgeInfo.allowance & curMedium) == 0) {
                //Medium not allowed on this edge -> skip
                continue;
            }
            //Fetch the actual cost to reach the adjacent node from the current node
            ///TODO: edgeCost testen/überarbeiten!!!
            double curCost = current.gScore + graph.getEdgeCost(adjacentEdge, timeIsPrio, curMaxSpeed);
            //Now we check the following:
            //A) if the adjacent node is unknown, we add it to the open list
            //B) if it was already known but we do know a better way to reach it already we just skip that node
            //C) if it is known but at a higher cost, we update the entry with this cheaper path!

            //This is case A): Node was not yet visited, so create a new entry in the lists
            if (openSet.find(adjacentNode.localID)==openSet.end()) {
                //Node is unknown: create entries in: openlist, predecessor map and prioQueue
                predecessorMap[adjacentNode.localID]=current.id;
                Handle_Type_t handle = prioQueue.emplace(adjacentNode.localID,
                                                          h(adjacentNode.localID, target)+curCost,
                                                            curCost);
                openSet.emplace(adjacentNode.localID, handle);
                ///DEBUG!!!
                expandedNodes.emplace_back(graph.nodeInfo.nodeData[adjacentNode.localID]);
                continue;
            }
            //Node was visited before ...
            Handle_Type_t handle = openSet[adjacentNode.localID];
            if ( (*handle).gScore <= curCost ) {
                //... and we know a better path already, so this is case B)
                continue;
            }
            //... and we just found a better way to reach it, so this is case C)!
            //Update the predecessor map entry of the adjacent node so it points to the current node
            predecessorMap[adjacentNode.localID]=current.id;
            //Create an update node to refresh the old entry of the adjacent node
            //We'll keep the id but update gScore and fScore with their new values
            OpenNode_t updateNode (adjacentNode.localID,
                                   h(adjacentNode.localID, target)+curCost,
                                    curCost);
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
    //Each node has to have a corresponding edgeCost entry
    if (graph.edgeCost.size() != graph.connectGraph.edges.size()) {
        return false;
    }
    //nonnect graph nodes must have the same length as the node info vector, otherwise something is fishy!
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


void AStar::getVisitedNodes (std::vector<PODNode> &visited) {
    for (size_t i=0; i<expandedNodes.size(); i++) {
        visited.emplace_back(expandedNodes.back().longitude, expandedNodes.back().latitude);
        expandedNodes.pop_back();
    }
}
