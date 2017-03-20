#include "navi.h"
#include "stopworkingexception.h"
#include <stdexcept>
#include <sstream>

//Use EdgeType value of the street as an index toi lookup the default speed limit of the street type
const uint16_t Navi::speedLookup[]{	//in km/h
    0,		//Default
    130, 120, 100, 100, 80,
    50, 30, 10,
    130, 120, 100, 100, 80,
    5, 5, 10,
    5, 300, 10,
    5, 5, 5, 5,
    15,	5, 5
};

//Use EdgeType value of the street as an index in this aray to lookup its allowance
const  uint8_t Navi::allowanceLookup[]{
    ALLOW_NONE,		//Default
    ALLOW_CAR, ALLOW_CAR, ALLOW_CAR, ALLOW_ALL, ALLOW_ALL,
    ALLOW_ALL, ALLOW_ALL, ALLOW_ALL,
    ALLOW_CAR, ALLOW_CAR, ALLOW_CAR, ALLOW_ALL, ALLOW_ALL,
    ALLOW_ALL, ALLOW_FOOT, ALLOW_FOOT|ALLOW_BIKE,
    ALLOW_ALL, ALLOW_CAR, ALLOW_ALL,
    ALLOW_FOOT | ALLOW_BIKE, ALLOW_FOOT | ALLOW_BIKE, ALLOW_FOOT, ALLOW_FOOT | ALLOW_BIKE,
    ALLOW_BIKE,	ALLOW_ALL, ALLOW_FOOT
};

const std::unordered_map<std::string, EdgeType> Navi::streetTypeLookup = {
    { "invalid" , INVALID},		//Default=Invalid
    {"motorway", MOTORWAY}, {"trunk", TRUNK}, {"primary", PRIMARY}, {"secondary", SECONDARY}, {"tertiary", TERTIARY},
    {"unclassified", UNCLASSIFIED}, {"residential", RESIDENTIAL}, {"service", SERVICE},
    {"motorway_link", MOTORWAY_LINK}, {"trunk_link", TRUNK_LINK}, {"primary_link", PRIMARY_LINK}, {"secondary_link", SECONDARY_LINK}, {"tertiary_link", TERTIARY_LINK},
    {"living_street", LIVING_STREET}, {"pedestrian", PEDESTRIAN}, {"track", TRACK},
    {"bus_guide_way", BUS_GUIDE_WAY}, {"raceway", RACEWAY}, {"road", ROAD},
    {"footway", FOOTWAY}, {"bridleway", BRIDLEWAY}, {"steps", STEPS}, {"path", PATH},
    {"cycleway", CYCLEWAY}, {"bus_stop", BUS_STOP}, {"platform", PLATORM}
};

void Navi::parsePbfFile(const std::string &path, CondWait_t *updateStruct) {

    //Create a logwriter instance to store logfile entries
    el::Logger* logger = el::Loggers::getLogger("default");
    try {
        logger->info("ParsePbfFile...");

        //Update progress if a updateStruct is provided
        if( updateStruct )
            updateStruct->updateProgress(0);

        /** Open osm-File to parse **/
        osmpbf::OSMFileIn osmFile(path);
        if (!osmFile.open()) {
            //Failed to open the file
            logger->error("Failed to open osm file. Check path!");
            throw (std::invalid_argument ("Invalid Path or bad file."));
        }
        else {
            logger->info("Opened OSM-File: %v with total size: %vmb.", path, osmFile.totalSize()/1048576);
        }
        //Clear previously parsed data
        fullGraph.clear();
        parkingGraph.clear();
        /** Create toolbox to parse **/
        //Primitive Input Block adapter - able to read one protobuf primitive block at a time
        osmpbf::PrimitiveBlockInputAdaptor pbi;
        //Temporary structure keeping parking spot-ids
        //Mapping of OSM-IDs to local offset IDs, used to check if a node was already parsed
        std::unordered_map<int64_t, size_t> osmIdMap;
        //Map to store parking areas IDs
        std::unordered_map<int64_t, size_t> parkingAreaMap;
        //Highway parsing filters
        osmRoutingFilter_t routingFilter;
        osmDedicatedParkingFilter_t dedicatedParkingFilter;
        osmParkingLaneFilter_t parkingLaneFilter;
        //Assign pbi to the filters
        routingFilter.assignInputAdapter(&pbi);
        dedicatedParkingFilter.assignInputAdapter(&pbi);
        parkingLaneFilter.assignInputAdapter(&pbi);
        


        /** Parse Step 1: Parse WayStream, collect 'interesting' osm-nodes and ways **/
        logger->info("Parse Step 1: Parse WayStream, collect 'interesting' osm-node-IDs and way infos.");
        if (updateStruct)
                updateStruct->updateProgress(1);
        uint32_t invalidWays=0;
        //Loop through all primitve blocks of the file ...
        while (osmFile.parseNextBlock(pbi)) {
            if (pbi.isNull()) {
                //Empty Block, skip
                continue;
            }
            //Update filters here, after the pbi changed and NOT inside the following loop, to save MUCH computation time!
            routingFilter.refreshAllFilter();
            parkingLaneFilter.refreshAllFilter();
            dedicatedParkingFilter.refreshAllFilter();
            //Get a waystream - containing all ways of the current primitve block
            osmpbf::IWayStream ways(pbi.getWayStream());
            //Loop over all those ways ...
            while (!ways.isNull()) {
                //Update highway filter

                //Check for highway-tagged ways with more than 2 nodes (others are of course invalid!)
                if (routingFilter.highwayFilter.matches(ways) && ways.refsSize()>=2) {
                    //So, this way is a highway connecting 2 or more nodes: fetch way data and node id's
                    //Is the way of a supported type?
                    const std::string &edgeTypeString = ways.value(routingFilter.highwayFilter.matchingTag());
                    auto streetType = streetTypeLookup.find( edgeTypeString);
                    if( streetType == streetTypeLookup.end() ) {
                        //Unknown or Invalid way type -> skip
                        ways.next();
                        invalidWays++;
                        continue;
                    }
                    //Store the relevant way info for later use
                    fullGraph.edgeInfo.emplace_back();
                    //Parse all the necessary edge infos and store them
                    EdgeInfo &info = fullGraph.edgeInfo.back();
                    info.type=(*streetType).second;
                    //Parse the way infos:
                    if (routingFilter.onewayFilter.matches(ways)) {
                        //This is a oneway edge
                        info.isOneWay=true;
                    }
                    if (!(routingFilter.footDenied.matches(ways))) {
                        //Foot access is NOT forbidden, so set foot allowance mask
                        info.allowance |= ALLOW_FOOT;
                    }
                    if (!(routingFilter.bikeDenied.matches(ways))) {
                        //Bike access is NOT forbidden, so set the bike allowance mask
                        info.allowance |= ALLOW_BIKE;
                    }
                    if(!(routingFilter.carDenied.matches(ways))) {
                        //Car access is NOT forbidden, so set the car allowance mask
                        info.allowance |= ALLOW_CAR;
                    }
                    if (routingFilter.speedFilter.matches(ways)) {
                        //A explicit way speed limit is stated: parse it
                        info.speed=parseOsmSpeedTag(ways.value(routingFilter.speedFilter.matchingTag()));
                    }
                    //Check if the speed still is set to 0 thus the parsing failed or no speed value is given
                    // => use default speed limit by street type
                    if (info.speed == 0.0) {
                        info.speed=speedLookup[info.type] * KMH_TO_MPS;
                    }
                    else if (info.speed > (MAXSPEED_CAR_MPS+0.5)) {
                        //If the speed limit is set to a value higher than 130, normalize it to 130
                        //This is necessary so that the A* heuristic fulfills certain criteria
                        info.speed = MAXSPEED_CAR_MPS;
                    }
                    //Indicate that this is a meta edge and contains several osm node ids, which all are connected by this way!
                    info.isMetaEdge=true;

                    //Parking along the Road allowed?
                    if (parkingLaneFilter.parkingLaneFilter.matches(ways)) {
                        //There is possible parking along this route!
                        info.parking=true;
                        //Add a new parking info node
                        parkingNodeInfo.emplace_back();
                        ParkingSolution &parkInfo=parkingNodeInfo.back();
                        info.parkingInfoID=parkingNodeInfo.size()-1;
                        //Parse it's settings:
                        parkInfo.buildFromParkingLaneFilter(parkingLaneFilter, ways);
                    }

                    //...then add all nodes along the way to the set of navigation nodes and to the stored way
                    osmpbf::IWayStream::RefIterator wayNodesIt(ways.refBegin());
                    osmpbf::IWayStream::RefIterator endIt(ways.refEnd());
                    for (;wayNodesIt!=endIt; ++wayNodesIt) {
                        //Store the OSM ID of all 'interesting' nodes in the map, but set the reference to uninitialized
                        osmIdMap[*wayNodesIt]=0xFFFFFFFF;
                        //Store the osm-ID of the node in the subEdge array of the osmWay
                        info.subEdges.emplace_back(*wayNodesIt);
                        //If edge allows lane-parking, add parking offsets for each node along the edge as well
                        if (info.parking) {
                            parkingAreaMap[*wayNodesIt]=parkingNodeInfo.size()-1;
                        }
                    }
                    //Check the next way
                    ways.next();
                    continue;
                }
                //So no highway ... maybe it's a parking area mapped as a closed way?
                if (dedicatedParkingFilter.parkingAreaFilter.matches(ways)) {
                    //It is! Parse and store its properties
                    //Add a new parking info node
                    parkingNodeInfo.emplace_back();
                    ParkingSolution &parkInfo=parkingNodeInfo.back();
                    //Parse and insert parking properties
                    parkInfo.buildFromDedicatedParkingFilter(dedicatedParkingFilter, ways);

                    osmpbf::IWayStream::RefIterator wayNodesIt(ways.refBegin());
                    osmpbf::IWayStream::RefIterator endIt(ways.refEnd());
                    for (;wayNodesIt!=endIt; ++wayNodesIt) {
                        //Store the OSM ID of all 'parking' nodes in the map, and set their reference
                        //to the just added parkInfo
                        parkingAreaMap[*wayNodesIt]=parkingNodeInfo.size()-1;
                    }
                }
                //check the next ways
                ways.next();
                continue;
            }
        }
        logger->info("Found %v highways, %v invalid ways, %v parking nodes and %v total nodes.", fullGraph.edgeInfo.size(), invalidWays, parkingNodeInfo.size(), osmIdMap.size());

        //Update progress if a updateStruct is provided
        if ( updateStruct )
            updateStruct->updateProgress( 30 );

        /** Parse Step 2: Parse NodeStream, collect actual osm-node data of those fetched in step 1 **/
        logger->info("Parse Step 2: Parse NodeStream, collect actual osm-node data of those fetched in step 1.\n");
        //reset the osm file
        osmFile.reset();
        size_t navNodes=0, chargeNodes=0;
        //Pre-Allocate  enough space for the number of nodes
        fullGraph.connectGraph.nodes.reserve(osmIdMap.size());
        fullGraph.nodeInfo.nodeData.reserve(osmIdMap.size());
        //Loop through all primitve blocks of the file ...
        while (osmFile.parseNextBlock(pbi)) {
            if (pbi.isNull()) {
                //Empty Block, skip
                continue;
            }
            //Update the filters to the node block adapter
            parkingLaneFilter.refreshAllFilter();
            dedicatedParkingFilter.refreshAllFilter();
            //Get a nodestream - containing all the nodes of the current primitive block
            osmpbf::INodeStream nodes (pbi.getNodeStream());
            //Loop through all nodes of this block ...
            while (!nodes.isNull()) {
                //Fetch all Navigation nodes - those are the nodes whose IDs are stored in the map
                //Note: Step 1 already ensured that we do not have duplicate nodes
                auto it = osmIdMap.find(nodes.id());
                if( it != osmIdMap.end() ) {
                    //So the ID of this node is contained in the map, thus an 'interesting' node
                    //Add an uninitialized node entry in the connectivity graph
                    fullGraph.connectGraph.nodes.emplace_back();
                    //And an entry in the nodeInfo vector
                    fullGraph.nodeInfo.nodeData.emplace_back();
                    //Fill in the the info we can get from the OSM-Node
                    NodeInfo &nodeInfo = fullGraph.nodeInfo.nodeData.back();
                    nodeInfo.longitude = nodes.lond();
                    nodeInfo.latitude = nodes.latd();
                    nodeInfo.osmID = nodes.id();
                    //The local ID is the offset of the node inside of the vecor
                    nodeInfo.localID = fullGraph.nodeInfo.nodeData.size()-1;
                    //Update the ID map with the local index of the node
                    osmIdMap[nodes.id()]=fullGraph.nodeInfo.nodeData.size()-1;
                    /*if (chargeStationFilter.matches(nodes)) {
                        //maybe this node even is a charge station?
                        nodeInfo.isChargeStation = true;
                    }*/
                    if (dedicatedParkingFilter.parkingAreaFilter.matches(nodes)) {
                        //Ohhh, it's a parking node (on a routing way)!
                        nodeInfo.allowsParking=true;
                        //Create a new entry into the parking spot info list
                        parkingNodeInfo.emplace_back();
                        ParkingSolution &spot=parkingNodeInfo.back();
                        //Parse its properties
                        spot.buildFromDedicatedParkingFilter(dedicatedParkingFilter, nodes);
                        nodeInfo.parkingPropertyID=parkingNodeInfo.size()-1;
                        //Store a copy of this node in the parkingGraph as well!
                        parkingGraph.nodeInfo.nodeData.emplace_back(nodeInfo);
                        parkingGraph.nodeInfo.nodeData.back().localID=parkingGraph.nodeInfo.nodeData.size()-1;
                    }
                    //Keep track for statistic reasons
                    navNodes++;
                }
                //Also fetch all parking area nodes, that were fetched in step 1
                auto parkIt = parkingAreaMap.find(nodes.id());
                if (parkIt != parkingAreaMap.end()) {
                    //create an entry for the parking node in the parking graph
                    parkingGraph.nodeInfo.nodeData.emplace_back();
                    NodeInfo &info = parkingGraph.nodeInfo.nodeData.back();
                    info.longitude = nodes.lond();
                    info.latitude = nodes.latd();
                    info.osmID = nodes.id();
                    info.localID=parkingGraph.nodeInfo.nodeData.size()-1;
                    info.allowsParking=true;
                    info.parkingPropertyID=parkingAreaMap[info.osmID];
                    parkingAreaMap[info.osmID]=info.localID;
                }
                //And any sole parking area nodes - these are those who are whether part of a way nor connected to a routing node!
                if (dedicatedParkingFilter.parkingAreaFilter.matches(nodes)) {
                    //Add a new entry to the parking info vector
                    parkingNodeInfo.emplace_back();
                    ParkingSolution &parkInfo=parkingNodeInfo.back();
                    //and parse its properties
                    parkInfo.buildFromDedicatedParkingFilter(dedicatedParkingFilter, nodes);
                    //Also add a node to the parking graph
                    parkingGraph.nodeInfo.nodeData.emplace_back();
                    NodeInfo &info = parkingGraph.nodeInfo.nodeData.back();
                    info.longitude = nodes.lond();
                    info.latitude = nodes.latd();
                    info.osmID = nodes.id();
                    info.localID=parkingGraph.nodeInfo.nodeData.size()-1;
                    info.allowsParking=true;
                }

                //Check the next node
                nodes.next();
            }
        }
        logger->info("Fetched %v street nodes, %v parking nodes.", navNodes, parkingGraph.nodeInfo.nodeData.size());

        //Update progress if a updateStruct is provided
        if ( updateStruct )
            updateStruct->updateProgress( 60 );

        /** Parse Step 3: Build graph edges from stored way data **/
        logger->info("Parse Step 3: Build the graph edges from stored way data.");
        //Allocate memory 3 times the number of nodes: most ways are not one ways, so we will have at the very least two edges
        //for each point-to-point edge, also there are many nodes which have multiple outgoing edges.
        //Allocate three times should mostly be enough and ensures, that no reallocation occurs which would result in a heavy memory
        //overuse by multiplying the current allocated space, we'll shrink to fit the vecor afterwards
        fullGraph.connectGraph.edges.reserve(navNodes*3);
        size_t srcOffset=0xFFFFFFFF, tarOffset=0xFFFFFFFF;
        //For all stored ways...
        for (size_t i=0; i<fullGraph.edgeInfo.size(); i++) {
            EdgeInfo &edge = fullGraph.edgeInfo[i];
            //... walk over all stored osmIDs ...
            for(size_t j=0;j<edge.subEdges.size()-1; j++) {
                size_t currentNodeId = edge.subEdges[j],
                        nextNodeId = edge.subEdges[j+1];

                //Check if the nodes are fetched, otherwise something is fishy ...
                auto currentLocalIdIt = osmIdMap.find(currentNodeId);
                auto nextLocalIdIt = osmIdMap.find(nextNodeId);
                if( currentLocalIdIt == osmIdMap.end() ) {
                    std::stringstream idStream;
                    idStream << "currentLocalId does not exist for edge " << i << "," << j;
                    throw std::runtime_error( idStream.str() );
                }

                if( nextLocalIdIt == osmIdMap.end() ) {
                    std::stringstream idStream;
                    idStream << "nextLocalId does not exist for edge " << i << "," << j;
                    throw std::runtime_error( idStream.str() );
                }

                //Get the local IDs of those nodes
                srcOffset= (*currentLocalIdIt).second;
                tarOffset= (*nextLocalIdIt).second;
                //Quickly set parking flag according to the way infomrations on all those nodes
                if (edge.parking) {
                    fullGraph.nodeInfo.nodeData[srcOffset].allowsParking=true;
                    fullGraph.nodeInfo.nodeData[tarOffset].allowsParking=true;
                }
                //And insert a connection from the current node to the next stored node, with a reference to this edge information
                fullGraph.connectGraph.edges.emplace_back(srcOffset, tarOffset, fullGraph.connectGraph.edges.size(), i);
                //Check if oneway - if not then add the back-edge as well
                if(!edge.isOneWay) {
                    //Create back-edge with switched src/tar nodes
                    fullGraph.connectGraph.edges.emplace_back(tarOffset, srcOffset, fullGraph.connectGraph.edges.size(), i);
                }
            }
        }
        //Shrink to fit on the vectors
        fullGraph.connectGraph.edges.shrink_to_fit();

        //Update progress if a updateStruct is provided
        if ( updateStruct )
            updateStruct->updateProgress( 80 );

        /** Parse Step 4: Sort the edges by src/tar**/
        logger->info("Parse Step 4: Sort the edges, calculate partial distances.");
        //Sort the connect-edges (by source/target nodes)
        std::sort(fullGraph.connectGraph.edges.begin(), fullGraph.connectGraph.edges.end());
        //Refresh the stored local id's of the edges!
        for (size_t i=0; i<fullGraph.connectGraph.edges.size(); ++i) {
            BasicEdge &cur=fullGraph.connectGraph.edges[i];
            cur.index=i;
        }


        //Update progress if a updateStruct is provided
        if ( updateStruct )
            updateStruct->updateProgress( 85 );

        /** Parse Step 5: Add Node->Edge offsets, then calculate the edge distance of each of the edges **/
        logger->info("Parse Step 5: Add Node->Edge offsets.");
        auto edgeIt = fullGraph.connectGraph.edges.begin();
        size_t edgeId=0;
        auto nodeIt = fullGraph.connectGraph.nodes.begin();
        size_t nodeId=0;
        nodeIt->firstEdge=0;
        //Loop over all stored edges ...
        while (edgeIt != fullGraph.connectGraph.edges.end()) {
            size_t curEdgeStartId = edgeIt->startNode;
            if (curEdgeStartId == nodeId) {
                //Still an adjacent edge of our current node, go on
                ++edgeId;
                ++edgeIt;
                continue;
            }
            //Another start node, so enter the end node offset
            nodeIt->lastEdge=edgeId;
            //We'll observe the next node
            ++nodeIt;
            ++nodeId;
            //set the firstEdge of this new node to the current edge id
            nodeIt->firstEdge=edgeId;
            //NOTE: we do NOT increment the edge iterator in this case
            //this will help with the correct detection of nodes without outgoing edges!
        }
        //At the end, insert the end of the last nodes' edges
        ///TODO: Letzter 'Last' Eintrag nochmal durchdenken ob dies nicht .size() sein müsste, da one-past-the-end!
        nodeIt->lastEdge = fullGraph.connectGraph.edges.size()-1;
        //Calculate the partial distances of each edge
        fullGraph.calculateEdgeDistances();
        //Update progress if a updateStruct is provided
        if ( updateStruct )
            updateStruct->updateProgress( 90 );

        /** Parse Step 6: Build up the closeness tree **/
        fullGraph.closenessTree.buildIndex();
        parkingGraph.closenessTree.buildIndex();
        //Update progress if a updateStruct is provided
        if ( updateStruct )
            updateStruct->updateProgress( 95 );


        /** Test the Graph for errors **/
        if (!selfCheck()) {
            logger->info("Self Check failed! End parsing!");
            //Update failure if a updateStruct is provided
            if ( updateStruct )
                updateStruct->updateProgress(0xFE);
            else {
                throw(std::runtime_error("Self Check of parsed Graph failed!"));
            }
        }
        else {
            logger->info("Self Check successful!");
            fullGraphParsed=true;
            //Update progress if a updateStruct is provided
            if ( updateStruct )
                updateStruct->updateProgress( 100 );
        }

    }
    catch(StopWorkingException e) {
        //Delete half parsed graph
        fullGraph.clear();
        parkingGraph.clear();
        fullGraphParsed=false;
        logger->info("Process was aborted");
    }
    catch(std::invalid_argument e) {
        //Delete half parsed graph
        fullGraph.clear();
        parkingGraph.clear();
        fullGraphParsed=false;
        logger->info("Parsing failed. Wrong file?");
        if (updateStruct) {
            std::unique_lock <std::mutex> uLock (updateStruct->mtx);
            //Lock
            updateStruct->progress=INVALID_INPUT;
            updateStruct->cond.notify_all();
            //Implicit Unlock
        }
    }
}

bool Navi::selfCheck() {
    //Setup the logger
    el::Logger* logger = el::Loggers::getLogger("default");
    logger->info("Checking the graph now ...");
    //Get length informations
    size_t nodeCount = fullGraph.connectGraph.nodes.size();
    size_t edgeCount = fullGraph.connectGraph.edges.size();
    //Loop over all nodes
    for (size_t i=0; i<nodeCount; i++) {
        //Fetch the current node
        BasicNode &curNode=fullGraph.connectGraph.nodes[i];
        //Check if the node is fully initialized
        if (curNode.firstEdge == 0xFFFFFFFF ||
                curNode.lastEdge == 0xFFFFFFFF) {
            logger->error("Node[%v]: BROKEN. Uninitialized or half initialized: FirstEdge=%v, LastEdge=%v",
                          i, curNode.firstEdge, curNode.lastEdge);
            return false;
        }
        //Check if inserted Edge offsets are out-of-bounds or invalid
        if (curNode.firstEdge >= edgeCount ||
                curNode.lastEdge >= edgeCount ||
                curNode.lastEdge < curNode.firstEdge) {
            logger->error("Node[%v]: Edge Offset Informations BROKEN. FirstEdge=%v, LastEdge=%v",
                          i, curNode.firstEdge, curNode.lastEdge);
            return false;
        }
        //Now loop over all its associated edges and check them for errors
        size_t adjacentEdgeCnt = curNode.numberOfEdges();
        for (size_t j=0; j<adjacentEdgeCnt; j++) {
            //Get the current edge we are looking at
            BasicEdge &curEdge = fullGraph.connectGraph.edges[curNode.firstEdge+j];
            //Check if the edges are initialized
            if (curEdge.startNode == 0xFFFFFFFF ||
                    curEdge.endNode == 0xFFFFFFFF) {
                logger->error("Edge[%v] of Node [%v] (Edge-ID[%v]) BROKEN. Uninitialized or half initialized: StartNode=%v, EndEdge=%v",
                              j, i, curEdge.index, curEdge.startNode, curEdge.endNode);
                return false;
            }
            //Check if the edges are within the index bounds
            if (curEdge.startNode >= nodeCount ||
                    curEdge.endNode >= nodeCount) {
                logger->error("Edge[%v] of Node [%v] (Edge-ID[%v]) BROKEN. Index out of bounds! StartNode=%v, EndEdge=%v",
                              j, i, curEdge.index, curEdge.startNode, curEdge.endNode);
                return false;
            }
            //Check its start node offset: it must equal the id of the current node!
            if (curEdge.startNode != i) {
                logger->error("Edge[%v] of Node [%v] (Edge-ID[%v]) BROKEN. startNode[%v] does not match nodeID!",
                              j, i, curEdge.index, curEdge.startNode);
                return false;
            }
            //Check if th edge info offset is initialized
            if (curEdge.edgeInfoId == 0xFFFFFFFF) {
                logger->error("Edge[%v] of Node [%v] (Edge-ID[%v]) BROKEN. Uninitialized edge info offset!",
                              j, i, curEdge.index);
                return false;
            }
        }

    }
    //Seems alright!
    logger->info("Seems alright!\n");

    return true;
}


void Navi::shortestPath(PODNode start, PODNode target, CondWait_t *updateStruct) {
    //Find the closest nav node for start and target nodes
    NodeInfo startNode = fullGraph.getClosestNode(start.getLongitude(), start.getLatitude());
    NodeInfo targetNode = fullGraph.getClosestNode(target.getLongitude(), target.getLatitude());
    ///NOTE: This is only the most simplest case
    /// TODO: Rethink strategy for E-Routing case
    //Redirect call to AStar
    ///DEBUG:
    pathfinder.setNavNodeKeepFlag(true);
    pathfinder.findRoute(startNode.localID, targetNode.localID, updateStruct);
}


double Navi::parseOsmSpeedTag(const std::string & speedTag) {
    //Check for walk speed tag
    if (speedTag.find("walk") != std::string::npos) {
            return 10*KMH_TO_MPS;      //10 kmh
    }

    //First: Parse the numerical value - if any
    std::string numericalSpeed;
    for (uint32_t i = 0; i < speedTag.length(); i++) {
            if (speedTag.at(i)>='0' && speedTag.at(i)<='9') {
                    numericalSpeed += speedTag.at(i);
            }
            else {
                    //No more numericals
                    break;
            }
    }
    //Parse the numerical
    double speed = atof(numericalSpeed.c_str());

    //Second: Check for possible unit value at the end (km/h, mp/h) and convert to meter/second
    if (speedTag.find("mph") != std::string::npos) {
            speed *= MPH_TO_MPS;
    }
    else {
            //Default Speed Unit is km/h (see: http://wiki.openstreetmap.org/wiki/Key:maxspeed)
            speed *= KMH_TO_MPS;
    }
    //Check the parsed value for nonsense
    if (speed < 0.0) {
        //If negative value: return 0
        speed = 0.0;
    }
    else if (speedLookup[MOTORWAY]*KMH_TO_MPS +1 < speed) {
        //Parsed value is higher than the maximum
        // => possible parse error: return 0 thus go with default speed by way type value
        speed = 0.0;
    }
    return speed;
}

void Navi::setTravelMedium (const TravelMedium medium) {
    this->medium=medium;
    pathfinder.setMedium(medium);
}

void Navi::setMaxRange(const size_t range) {
    this->maxRange=range;
    pathfinder.setMaxDistance(range);
}

void Navi::setStartRange(const uint8_t range) {
    this->startRange=range;
    ///TODO: add a* start range if time is left!
}

void Navi::setRoutingPriority(bool travelTimePriority) {
    timeIsPrio=travelTimePriority;
    pathfinder.setRoutingPrio(travelTimePriority);
}


void Navi::getFullGraph (LinearGraph &graph) {
    graph.reset();
    //Copy over nodes
    for (size_t i=0; i<fullGraph.nodeInfo.nodeData.size(); ++i) {
        NodeInfo &cur = fullGraph.nodeInfo.nodeData[i];
        graph.insertNode(PODNode(cur.longitude, cur.latitude));
    }
    //Copy over edges
    for (size_t i=0; i<fullGraph.connectGraph.edges.size(); ++i) {
        BasicEdge &cur = fullGraph.connectGraph.edges[i];
        graph.insertEdge(PODEdge(cur.startNode, cur.endNode));
    }
}

void Navi::getShortestRouteGraph(LinearGraph &graph) {
    graph.reset();
    pathfinder.getRoute(graph);
}

void Navi::reset() {
    fullGraph.connectGraph.edges.clear();
    fullGraph.connectGraph.nodes.clear();
    fullGraph.edgeInfo.clear();
    fullGraph.nodeInfo.nodeData.clear();
    parkingGraph.connectGraph.edges.clear();
    parkingGraph.connectGraph.nodes.clear();
    parkingGraph.edgeInfo.clear();
    parkingGraph.nodeInfo.nodeData.clear();
}
