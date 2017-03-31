#include "Navigator.h"


Navigator::Navigator() {

}
RoutingNode::RoutingNode(const RoutingNode &copy) {
    mID = copy.mID;
    mPredecessorID=copy.mPredecessorID;
    mNodeWeight=copy.mNodeWeight;
    mDistToTarget=copy.mDistToTarget;
}

Navigator::~Navigator()
{
}

/*
Node& Navigator::getRoutingNodePosition (const double lat, const double lon) {
    //search the closest osm node for the given coordinates to display on the map
    uint32_t offset = mGraph.searchClosestNode(lat, lon);
    return mGraph.mNodes[offset];
}*/

Route& Navigator::route(const double lat_A, const double lon_A, const double lat_B, const double lon_B) {
    //Search the closest osm nodes to start and end
    uint32_t startOsmNode = mGraph.searchClosestNode(lat_A, lon_A);
    uint32_t endOsmNode = mGraph.searchClosestNode(lat_B, lon_B);
    ///Test Graph run
    //return route(0, 3);
    ///
    return route(startOsmNode, endOsmNode);
}


Route& Navigator::route(const uint32_t sourceID, const uint32_t targetID)
{
    LogWriter log(false);
    log.logFile<<"[Navigator::route():]\n";
    log.logFile<<"Starting A*-Algorithmus. Routing from Node " << sourceID << " to " << targetID<<".\n";
    if (mGraph.mNodes.size()==0) {
        log.logFile<<"[ERROR:] Navigation Graph is empty. Parsing failed?\n";
        throw("NavGraph seems to be empty. Did you forget to parse a OSM-File?\n");
    }
    else if (sourceID>mGraph.mNodes.size() || targetID>mGraph.mNodes.size()) {
        log.logFile<<"[ERROR:] Start or Target Node out of bounds!\n";
        throw("Source/Target ID out of bounds!\n");
    }
    //Compute the closest OsmNode to the given coordinates of Start and End

	RoutingNode start(sourceID, NULL, 0, 0);
    RoutingNode target(0, NULL, 0, 0);
	bool foundSolution=false;
	//Initialize openList with the start node
    openList.emplace(start);
	while (!openList.empty()) {
        // RoutingNode cur(sourceID, NULL, 0, 0);			//Current node, initialised with the start node
        RoutingNode cur = openList.top();                   //Take the most promising node from the open list for further expansion
        openList.pop();                                     //And remove it from the list
		if (cur.mID == targetID) {
			//Found the shortest path *yeah*
			//The path can be reconstructed if one "walks" back via predecessor until the start node
			foundSolution = true;
            target=cur;
			break;
		}
        //Expand current node, and store it in the expanded-List for later recreation of the path
        closedList.insert(cur.mID);
        closedNodeObjects.push_back(cur);
        expandNode(closedNodeObjects.back(), targetID);
	}
	if (!foundSolution) {
        //Throw an exception, indicating failure
        log.logFile<<"No valid path found! Maybe target is only reachable via unauthorized roads?\n";
		throw("No possible/allowed/satisfying Path could be found.\n");
	}
	else {
		//Build the found shortest path, starting with the final node walking backwards
        log.logFile <<"Shortes Path found!";
        buildPath(start, target);
		return path;
	}
}

void Navigator::buildPath(const RoutingNode &startNode, const RoutingNode & finalNode)
{
    LogWriter log (false);
    log.logFile<<"[Navigator::buildPath():]\n"<<"Fetching all edges along the shortest Path.\n";
    std::vector<RoutingNode> backwards;
	//First fetch all route nodes from final->start, via back_edges
    backwards.push_back(finalNode);
    RoutingNode cur(finalNode);
    while(cur.mID != startNode.mID) {
        cur=closedNodeObjects[cur.mPredecessorID];
        backwards.push_back(cur);
    }

	//Then build the Route
	path.routeNodes.clear();
	path.routeEdges.clear();
	path.routeNodes.reserve(backwards.size());
	//First the Nodes
	for (int32_t i = backwards.size()-1; i >=0;i--) {
		//Fill in backwards - makes the route start at the start node
        path.routeNodes.push_back(mGraph.mOsmNodes.OsmNodes[backwards[i].mID]);
	}
	//Then the edges
	path.routeEdges.reserve(backwards.size()-1);
    for (int32_t i = backwards.size() - 1; i > 0; i--) {
		//Fill in backwards - makes the route start at the start node
		path.routeEdges.push_back(mGraph.getEdge(backwards[i].mID, backwards[i-1].mID));
	}
}

void Navigator::setTravelMedium(travelMedium medium)
{
	medium = medium;
}

void Navigator::setPriority(bool timePriorityFlag)
{
	timeIsPriority=timePriorityFlag;
}

void Navigator::expandNode(const RoutingNode &current, const uint32_t targetID)
{
    //Get the IDs if all edges of the current node
    uint32_t curEdge = mGraph.mNodes[current.mID].firstEdge;     //first edge id
    uint32_t lastEdge = mGraph.mNodes[current.mID].lastEdge;     //last edge id
    uint32_t edgeCnt = (lastEdge - curEdge) + 1;
    uint32_t curSuc=0;
    for (uint32_t i = 0; i < edgeCnt; i++, curEdge++) {		//For all edges of the current node
        //Check the successor node
         curSuc = mGraph.mEdges[curEdge].getEnd();          //This will return the ID of the i-th successor node of the current node
        if (closedList.find(curSuc)!= closedList.end()) {
            //This node was already completely checked
			continue;
		}
		//Which NavEdge are we talking about here?
        const NavEdge edge(mGraph.mEdges[curEdge]);
        if (!((edge.allowance & medium) > 0)) {
			//If this edge does NOT allow the used medium - skip this edge
            //=>Bitmask check if associated bit mask is set or not
			continue;

		}
		RoutingNode successor;
		successor.mID = curSuc;
		//Weigth depends on the optimizations criteria: time or distance
        double sucNodeWeight = current.mNodeWeight;		//=Distance traveled until the predecessor
		if (timeIsPriority) {
			//Travel Time is optimization target
			sucNodeWeight += edge.travelTime;
		}
		else {
			//Travel Distance is optimization target
			sucNodeWeight += edge.travelDistance;
		}
		/*************************************************************** 
		Check if the node is already contained in the openList 
				- if so check its weight and possibly update the entry 
				- else simply add the sucessor node to the list
		****************************************************************/
		if (handleMap.count(successor.mID)>0) {
			//Fetch the handle to the successor node in the openList
			OpenList::handle_type found = handleMap[successor.mID];
			if ((*found).mNodeWeight <= sucNodeWeight) {
				//Nothing to do here, as we found a shorter way to this successor already
				continue;
			}
			else {
                //Assign the pre-calculated NodeWeight
                successor.mNodeWeight=sucNodeWeight;
                //Set predecessor link = the current back of closedList as it is currently expanded
                successor.mPredecessorID = closedNodeObjects.size()-1;
				//Calculate Heuristic approximation the distance to target based on current weight + haversine distance to the target
				successor.mDistToTarget = sucNodeWeight + mGraph.pointDistance(successor.mID, targetID);
				//Update the openList-entry of the node
				openList.increase(found, successor);
			}
		}
		else {
			//Node is not contained in the openList yet, so we add it:
			//Set predecessor link
            successor.mNodeWeight=sucNodeWeight;
            //Set predecessor link = the current back of closedList as it is currently expanded
            successor.mPredecessorID = closedNodeObjects.size()-1;;
			//Calculate Heuristic approximation the distance to target based on current weight + haversine distance to the target
			successor.mDistToTarget = sucNodeWeight + mGraph.pointDistance(successor.mID, targetID);
			openList.push(successor);
		}
	}
}

void Navigator::buildNavGraph(std::string &path, uint8_t *progress) {
    mGraph.buildFromFile(path, progress);
}


bool lesser::operator()(const RoutingNode & compA, const RoutingNode & compB) const
{
	if(compA.mNodeWeight < compB.mNodeWeight) {
		//A is stricly smaller than B
		return true;
	}
	else {
		//A is equal to b or larger
		return false;
	}
}

RoutingNode & RoutingNode::operator=(const RoutingNode & copy)
{
	mID = copy.mID;
    mPredecessorID = copy.mPredecessorID;
	mNodeWeight = copy.mNodeWeight;
	mDistToTarget = copy.mDistToTarget;
	return *this;
}
