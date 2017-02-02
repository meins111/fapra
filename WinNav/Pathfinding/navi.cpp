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
    ALLOW_ALL,		//Default
    ALLOW_CAR, ALLOW_CAR, ALLOW_CAR, ALLOW_ALL, ALLOW_ALL,
    ALLOW_ALL, ALLOW_ALL, ALLOW_ALL,
    ALLOW_CAR, ALLOW_CAR, ALLOW_CAR, ALLOW_ALL, ALLOW_ALL,
    ALLOW_ALL, ALLOW_FOOT, ALLOW_ALL,
    ALLOW_ALL, ALLOW_CAR, ALLOW_ALL,
    ALLOW_FOOT | ALLOW_BIKE, ALLOW_ALL, ALLOW_FOOT, ALLOW_FOOT | ALLOW_BIKE,
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
            logger->info("Opened OSM-File: %v.", path);
        }

        /** Create toolbox to parse **/
        //Primitive Input Block adapter - able to read one protobuf primitive block at a time
        osmpbf::PrimitiveBlockInputAdaptor pbi;
        //Data structure to store osm-highway information between parse step 1 and 2 - tradeoff memory for speed as we save one parse-through
        std::vector<EdgeInfo> osmWays;
        //Mapping of OSM-IDs to local offset IDs, used to check if a node was already parsed
        std::unordered_map<int64_t, size_t> osmIdMap;
        //Key Only Tag filter are able to dismiss any entry that does not match the given Tag, e.g. "highway" for streets
        osmpbf::KeyOnlyTagFilter highwayFilter ("highway");
        //Key Value Tag Filter are ablte to dismiss any entry that does not match the given Tag-Value pair, e.g. amenity: charge_station
        osmpbf::KeyValueTagFilter chargeStationFilter ("amenity", "charging_station");
        //Speed filter allows the extraction of the allowed max speed on that edge
        osmpbf::KeyOnlyTagFilter speedFilter ("maxspeed");
        //Check if the route is a oneway: this is it is marked as one, or it's type is implicitly oneway such as motorways
        osmpbf::OrTagFilter onewayFilter ({ new osmpbf::KeyValueTagFilter("oneway", "yes"),
                                            new osmpbf::KeyMultiValueTagFilter("highway",{ "motorway", "motorway_link" }),
                                            new osmpbf::KeyMultiValueTagFilter("junction",{ "roundabout" })
                                          });
        //Check if foot-access is forbidden or it's type implicitly denies foot-access such as motorways
        osmpbf::OrTagFilter footDenied ({   new osmpbf::KeyValueTagFilter("foot", "no"),
                                            new osmpbf::KeyMultiValueTagFilter("highway", {"motorroad", "trunk", "primary", "motorroad_link", "trunk_link", "primary_link"})
                                        });
        //Check if bike-access is forbidden or it's street type implicitly denies bike-access such as motorways
        osmpbf::OrTagFilter bikeDenied ({   new osmpbf::KeyValueTagFilter("bicycle", "no"),
                                            new osmpbf::KeyMultiValueTagFilter("highway", {"motorroad", "trunk", "primary", "motorroad_link", "trunk_link", "primary_link"})
                                        });
        //Check if car-access is forbidden or it's type implicitly denies car access such as footways
        osmpbf::OrTagFilter carDenied ({    new osmpbf::KeyValueTagFilter ("motorcar", "no"),
                                            new osmpbf::KeyMultiValueTagFilter("highway", {"footway", "steps", "path", "cycleway", "platform"})
                                        });

        /** Parse Step 1: Parse WayStream, collect 'interesting' osm-nodes and ways **/
        logger->info("Parse Step 1: Parse WayStream, collect 'interesting' osm-node-IDs and way infos.");
        uint32_t addWays=0, addNodes=0, invalidWays=0;
        //Loop through all primitve blocks of the file ...
        while (osmFile.parseNextBlock(pbi)) {
            if (pbi.isNull()) {
                //Empty Block, skip
                continue;
            }
            else {
                highwayFilter.assignInputAdaptor(&pbi);
                //Get a waystream - containing all ways of the current primitve block
                osmpbf::IWayStream ways(pbi.getWayStream());
                //Loop over all those ways ...
                while (!ways.isNull()) {
                    //... and take all ways taged as 'highway' and featuring at least two nodes - others are invalid
                    if (highwayFilter.matches(ways) && ways.refsSize()>=2) {
                        //Assign Filters
                        onewayFilter.assignInputAdaptor(&pbi);
                        speedFilter.assignInputAdaptor(&pbi);
                        footDenied.assignInputAdaptor(&pbi);
                        bikeDenied.assignInputAdaptor(&pbi);
                        carDenied.assignInputAdaptor(&pbi);
                        //Is the way of a supported type?
                        const std::string &edgeTypeString = ways.value(highwayFilter.matchingTag());
                        auto streetType = streetTypeLookup.find(edgeTypeString);
                        if( streetType == streetTypeLookup.end() ) {
                            //Unknown / Invalid way type -> skip
                            ways.next();
                            invalidWays++;
                            continue;
                        }
                        //Store the relevant way info for later use
                        osmWays.emplace_back();
                        //Set the street type
                        EdgeInfo &info = osmWays.back();
                        info.type= *streetType;
                        //Parse the way infos:
                        if (onewayFilter.matches(ways)) {
                            //This is a oneway edge
                            info.isOneWay=true;
                        }
                        if (!footDenied.matches(ways)) {
                            //Foot access is NOT forbidden, so set foot allowance mask
                            info.allowance |= ALLOW_FOOT;
                        }
                        if (!bikeDenied.matches(ways)) {
                            //Bike access is NOT forbidden, so set the bike allowance mask
                            info.allowance |= ALLOW_BIKE;
                        }
                        if(!carDenied.matches(ways)) {
                            //Car access is NOT forbidden, so set the car allowance mask
                            info.allowance |= ALLOW_CAR;
                        }
                        if (speedFilter.matches(ways)) {
                            //A explicit way speed limit is stated: parse it
                            info.speed=parseOsmSpeedTag(ways.value(speedFilter.matchingTag()));
                        }
                        else {
                            //Else use the default speed limit by way-type
                            info.speed=speedLookup[info.type];
                        }
                        info.isMetaEdge=true;


                        //...then add all nodes along the way to the set of navigation nodes and to the stored way
                        osmpbf::IWayStream::RefIterator wayNodesIt(ways.refBegin());
                        osmpbf::IWayStream::RefIterator endIt(ways.refEnd());
                        ++addWays;
                        while (wayNodesIt!=endIt) {
                            //Store the OSM ID of all 'interesting' nodes in the map, but set the reference to uninitialized
                            osmIdMap[*wayNodesIt]=0xFFFFFFFF;
                            ++wayNodesIt;
                            ++addNodes;
                            //Store the osm-ID of the node in the subEdge array of the osmWay
                            info.subEdges.emplace_back(*wayNodesIt);
                        }

                    }
                    //Check the next way
                    ways.next();
                }
            }
        }
        logger->info("Found %v highways, %v invalid/uninteresting ways and %v total nodes.", osmWays.size(), invalidWays, osmIdMap.size());

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
            else {
                //Get a nodestream - containing all the nodes of the current primitive block
                osmpbf::INodeStream nodes (pbi.getNodeStream());
                //Assign the charge station filter to the node block adapter
                chargeStationFilter.assignInputAdaptor(&pbi);
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
                        if (chargeStationFilter.matches(nodes)) {
                            //maybe this node even is a charge station?
                            nodeInfo.isChargeStation = true;
                            chargeNodes++;
                        }
                        //Keep track for statistic reasons
                        navNodes++;
                    }
                    //Also fetch all nodes marked as charge station nodes to build the e-routing meta graph
                    //wether they are part of the highway network or not
                    //it is assumed that charging_stations always could be reached from the closest naviagtion node for sake of simplicity
                    if (chargeStationFilter.matches(nodes)) {
                        //Insert one connection node per charge station
                        metaGraph.connectGraph.nodes.emplace_back();
                        //And one data node
                        metaGraph.nodeInfo.nodeData.emplace_back();

                        NodeInfo &nodeInfo = metaGraph.nodeInfo.nodeData.back();
                        //And edit the stored data
                        nodeInfo.longitude = nodes.lond();
                        nodeInfo.latitude = nodes.latd();
                        nodeInfo.osmID = nodes.id();
                        nodeInfo.localID = metaGraph.nodeInfo.nodeData.size()-1;
                        nodeInfo.isChargeStation = true;
                        chargeNodes++;
                    }
                    nodes.next();
                }
            }
        }
        logger->info("Fetched %v street nodes, %v charging stations.", navNodes, chargeNodes);

        //Update progress if a updateStruct is provided
        if ( updateStruct )
            updateStruct->updateProgress( 60 );

        /** Parse Step 3: Build graph edges from stored way data **/
        logger->info("Parse Step 3: Build the graph edges from stored way data.");
        //Allocate memory for the least number of edges for the number of stored nodes: n nodes => >n/2 edges
        fullGraph.connectGraph.edges.reserve(navNodes*2);
        fullGraph.edgeInfo.reserve(navNodes*2);
        size_t srcOffset=0xFFFFFFFF, tarOffset=0xFFFFFFFF;
        //For all stored ways...
        for (size_t i=0; i<osmWays.size(); i++) {
            EdgeInfo &edge = osmWays[i];
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
                    throw std::runtime_exception( idStream.str() );
                }

                if( nextLocalIdIt == osmIdMap.end() ) {
                    std::stringstream idStream;
                    idStream << "nextLocalId does not exist for edge " << i << "," << j;
                    throw std::runtime_exception( idStream.str() );
                }

                //Get the local IDs of those nodes
                srcOffset= *currentLocalIdIt;
                tarOffset= *nextLocalIdIt;
                //And insert a connection from the current node to the next stored node
                fullGraph.connectGraph.edges.emplace_back(srcOffset, tarOffset, fullGraph.connectGraph.edges.size());
                //Also add the respective edge info using the edge infos parsed in step 1
                fullGraph.edgeInfo.emplace_back(edge);

                // TODO: Copy construct des vectors nur um ihn dann zu clearen erzeugt viele unnoetige Speicheroperationen.
                //			Verbessern: Edge erzeugen und copy ohne die subEdges. Am besten als Subfunktion der EdgeInfo

                //Remove the meta entries in the newly created edge
                fullGraph.edgeInfo.back().isMetaEdge=false;
                fullGraph.edgeInfo.back().subEdges.clear();
                //Check if oneway - if not then add the back-edge as well
                if(!edge.isOneWay) {
                    //Create back-edge with switched src/tar nodes
                    fullGraph.connectGraph.edges.emplace_back(tarOffset, srcOffset, fullGraph.connectGraph.edges.size());
                    fullGraph.edgeInfo.emplace_back(edge);
                    fullGraph.edgeInfo.back().isMetaEdge=false;
                    fullGraph.edgeInfo.back().subEdges.clear();
                }
            }
        }

        //Update progress if a updateStruct is provided
        if ( updateStruct )
            updateStruct->updateProgress( 80 );

        /** Parse Step 4: Sort the edges by src/tar to complete the offset-graph-representation **/
        //We'll need an index array so we can keep the relation between connectivity-edges and edgeInfo entries
        std::vector<size_t> indizes;
        indizes.reserve(fullGraph.edgeInfo.size());
        indizes.resize(fullGraph.edgeInfo.size());
        logger->info("Parse Step 4: Sort the edges.");
        //Sort the connect-edges (by source/target nodes)
        std::sort(fullGraph.connectGraph.edges.begin(), fullGraph.connectGraph.edges.end());
        //Build the target-index vector from the old/new indizes of the connect-edges
        for(size_t i=0,
            edgeCount = fullGraph.connectGraph.edges.size() ; i < edgeCount ; i++) {
            //Use the old index to conclude where the matching edgeInfo entry has to be moved to
            //indizes[fullGraph.connectGraph.edges[i].index]=i;
            indizes[i]=fullGraph.connectGraph.edges[i].index;
            //Update the index entry to the new location
            fullGraph.connectGraph.edges[i].index=i;
        }
        //Now that we have build the target-index vector, we can use the inplace-reorder operation to sort the edgeInfo-vector
        //such that the offsets of connectivity edge and edgeInfo-entry do match again

        inplaceReorder(fullGraph.edgeInfo, indizes);

        //Free the helper vector and the stored osm ways
        indizes.clear();
        osmWays.clear();

        //Update progress if a updateStruct is provided
        if ( updateStruct )
            updateStruct->updateProgress( 90 );

        /** Parse Step 5: Add Node->Edge offsets **/
        logger->info("Parse Step 5: Add Node->Edge offsets.");
        size_t curSrc=fullGraph.nodeInfo.nodeData[0].localID;
        fullGraph.connectGraph.nodes[0].firstEdge=0;
        for (size_t i=0; i<fullGraph.connectGraph.edges.size(); i++) {
            if (fullGraph.connectGraph.edges[i].startNode != curSrc) {
                //We reached the point where the adjacent edges of the next node starts ...
                //... so set the end of the previous node, and the start of the new
                fullGraph.connectGraph.nodes[curSrc].lastEdge=i;
                fullGraph.connectGraph.nodes[curSrc+1].firstEdge=i;
                curSrc=fullGraph.nodeInfo.nodeData[i].localID;
            }
            //Else we have nothing to do
        }
        //At the end, insert the end of the last nodes' edges
        fullGraph.connectGraph.nodes.back().lastEdge=fullGraph.connectGraph.edges.size()-1;


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
        logger->info("Self Check successful!");

        //Update progress if a updateStruct is provided
        if ( updateStruct )
            updateStruct->updateProgress( 100 );

    } catch(StopWorkingException e) {
        logger->info("Process was aborted");
    }
}

bool Navi::selfCheck() {
    el::Logger* logger = el::Loggers::getLogger("default");
    //check existence of all nodes/edges
    uint32_t nc = fullGraph.connectGraph.nodes.size();
    uint32_t ec = fullGraph.connectGraph.edges.size();
    logger->info("Starting self check of parsed graph ...");
    for(uint32_t i=0; i < nc; i++) {
        const BasicNode n = fullGraph.connectGraph.nodes[i];
        if (n.firstEdge == 0xFFFFFFFF || n.lastEdge == 0xFFFFFFFF) {
            logger->warn("Node[%v] BROKEN. Uninitialized or half initialized: First=%v, Last=%v", i, n.firstEdge, n.lastEdge);
            return false;
        }
        else if (n.firstEdge >= ec || n.lastEdge < n.firstEdge) {
            logger->warn("Node[%v] BROKEN. Offset out of bounds: First=%v, Last=%v.");
            return false;
        }
       //Know check adjacent edges of this node for validness
       size_t edgeCnt=n.lastEdge-n.firstEdge;
       BasicEdge *curEdge=NULL;
       for (size_t j=0; j<edgeCnt; j++) {
           curEdge = & fullGraph.connectGraph.edges[n.firstEdge+j];
           if(curEdge->startNode != i) {
               //An adjacent edge whose start node does not equal the node we are currently checking is INVALID!
               logger->warn("Edge[%v] BROKEN. Start Node ID %v does not match this node's ID %v.", n.firstEdge+j, curEdge->startNode, i);
               return false;
           }
           if(curEdge->endNode == 0xFFFFFFFF) {
               //Uniinitialized Target Offset!
               logger->warn("Edge[%v] BORKEN. End Offset uninitialized!", n.firstEdge+j);
               return false;
           }
           else if (curEdge->endNode >= fullGraph.connectGraph.edges.size()) {
               //Index out of bounds error!
               logger->warn("Edge[%v] BROKEN. End Offset out of bounds.", n.firstEdge+j);
               return false;
           }
       }
    }
    //Seems alright!
    logger->info("Seems alright!\n");
    return true;
}


LinearGraph & Navi::shortestPath(const Node &start, const Node &target) {

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
    if (speedTag.find("m") != std::string::npos) {
            speed *= MPH_TO_MPS;
    }
    else {
            //Default Speed Unit is km/h (see: http://wiki.openstreetmap.org/wiki/Key:maxspeed)
            speed *= KMH_TO_MPS;
    }
    return speed;
}

void Navi::reset() {
    fullGraph.connectGraph.edges.clear();
    fullGraph.connectGraph.nodes.clear();
    fullGraph.edgeInfo.clear();
    fullGraph.nodeInfo.nodeData.clear();
    metaGraph.connectGraph.edges.clear();
    metaGraph.connectGraph.nodes.clear();
    metaGraph.edgeInfo.clear();
    metaGraph.nodeInfo.nodeData.clear();
}
