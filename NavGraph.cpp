#include "NavGraph.h"
//STD Includes
#include <stdio.h>

//OSMPBF includes below
#include <osmpbf/parsehelpers.h>
#include <osmpbf/filter.h>
#include <osmpbf/iway.h>
#include <osmpbf/inode.h>

//Log file include
#include "../logwriter.h"



NavGraph::~NavGraph()
{
}

//Static Lookup Array Initilizations
const uint8_t NavGraph::allowanceLookup[]{
	ALLOW_ALL,		//Default
	ALLOW_CAR, ALLOW_CAR, ALLOW_CAR, ALLOW_ALL, ALLOW_ALL, 
	ALLOW_ALL, ALLOW_ALL, ALLOW_ALL,
	ALLOW_CAR, ALLOW_CAR, ALLOW_CAR, ALLOW_ALL, ALLOW_ALL,
	ALLOW_ALL, ALLOW_FOOT, ALLOW_ALL, 
	ALLOW_ALL, ALLOW_CAR, ALLOW_ALL,
	ALLOW_FOOT | ALLOW_BIKE, ALLOW_ALL, ALLOW_FOOT, ALLOW_FOOT | ALLOW_BIKE, 
	ALLOW_BIKE,	ALLOW_ALL, ALLOW_FOOT
};

/** Speed Lookup Array, matches max-speed values to edge types
 * Note 1: This is only an estimation and provides speed estimates according to edge type. 
 *		Use explicitly stated max speed for edges were possible!
 * Note 2: These are the edge speeds for german streets only. Other countries might have very different 
 *		Max-Speed regulations per edge type!
 */
const uint16_t NavGraph::speedLookup[]{	//in km/h
	0,		//Default
	130, 120, 100, 100, 80, 
	50, 30, 10, 
	130, 120, 100, 100, 80,
	5, 5, 10, 
	5, 300, 10,
	5, 5, 5, 5, 
	15,	5, 5
};

//Constant Hashmap: mapping osm street type strings to enum type representation
const std::unordered_map<std::string, EdgeType> NavGraph::streetTypeMap{
	{ "invalid" , INVALID},		//Default=Invalid
	{"motorway", MOTORWAY}, {"trunk", TRUNK}, {"primary", PRIMARY}, {"secondary", SECONDARY}, {"tertiary", TERTIARY},
	{"unclassified", UNCLASSIFIED}, {"residential", RESIDENTIAL}, {"service", SERVICE},
	{"motorway_link", MOTORWAY_LINK}, {"trunk_link", TRUNK_LINK}, {"primary_link", PRIMARY_LINK}, {"secondary_link", SECONDARY_LINK}, {"tertiary_link", TERTIARY_LINK},
	{"living_street", LIVING_STREET}, {"pedestrian", PEDESTRIAN}, {"track", TRACK}, 
	{"bus_guide_way", BUS_GUIDE_WAY}, {"raceway", RACEWAY}, {"road", ROAD},
	{"footway", FOOTWAY}, {"bridleway", BRIDLEWAY}, {"steps", STEPS}, {"path", PATH},
	{"cycleway", CYCLEWAY}, {"bus_stop", BUS_STOP}, {"platform", PLATORM}
};



bool NavEdge::operator<(const NavEdge & operand)
{
	if (this->source!=operand.source) {
		//Sort by Source-ID is more important than sorting by target ID
		return (this->source < operand.source);
	}
	else {
		//If both edges have the same source, sort by comparing their target node IDs
		return (this->target < operand.target);
	}
}

size_t NavEdge::getStart()
{
    return source;
}

size_t NavEdge::getEnd()
{
    return target;
}

void NavGraph::buildFromFile(const std::string &osmPath, uint8_t *progress)
{
    LogWriter log(false);
    log.logFile<<"[NavGraph::buildFromFile()]\n";
    /////Make own graph
    /*
    mNodes.reserve(4);
    mOsmNodes.OsmNodes.reserve(4);
    OsmNode zero(0, 49.607, 10.301, false);
     OsmNode one(1, 49.729, 10.334, false);
      OsmNode two(2, 49.701, 10.413, false);
       OsmNode three(3, 49.724, 10.438, false);
    mOsmNodes.OsmNodes.push_back(zero);
    mOsmNodes.OsmNodes.push_back(one);
    mOsmNodes.OsmNodes.push_back(two);
    mOsmNodes.OsmNodes.push_back(three);
    mEdges.reserve(10);
    NavEdge a(0, 1, PRIMARY, ALLOW_ALL, 22, false, 0, 0);
    a.travelDistance=pointDistance(0,1);
    a.travelTime=a.travelDistance/a.speed;
    NavEdge b(0, 2, PRIMARY, ALLOW_ALL, 27, false, 0, 0);
    b.travelDistance=pointDistance(0,2);
    b.travelTime=b.travelDistance/b.speed;
    NavEdge c(1, 0, PRIMARY, ALLOW_ALL, 22, false, 0, 0);
    c.travelDistance=pointDistance(1,0);
    c.travelTime=c.travelDistance/c.speed;
    NavEdge d(1, 2, PRIMARY, ALLOW_ALL, 27, false, 0, 0);
    d.travelDistance=pointDistance(1,2);
    d.travelTime=d.travelDistance/d.speed;
    NavEdge e(1, 3, PRIMARY, ALLOW_ALL, 8, false, 0, 0);
    e.travelDistance=pointDistance(1,3);
    e.travelTime=e.travelDistance/e.speed;
    NavEdge f(2, 0, PRIMARY, ALLOW_ALL, 22, false, 0, 0);
    f.travelDistance=pointDistance(2,0);
    f.travelTime=f.travelDistance/f.speed;
    NavEdge g(2, 1, PRIMARY, ALLOW_ALL, 27, false, 0, 0);
    g.travelDistance=pointDistance(2,1);
    g.travelTime=g.travelDistance/g.speed;
    NavEdge h(2, 3, PRIMARY, ALLOW_ALL, 27, false, 0, 0);
    h.travelDistance=pointDistance(2,3);
    h.travelTime=h.travelDistance/h.speed;
    NavEdge i(3, 1, PRIMARY, ALLOW_ALL, 8, false, 0, 0);
    i.travelDistance=pointDistance(3,1);
    i.travelTime=i.travelDistance/i.speed;
    NavEdge j(3, 2, PRIMARY, ALLOW_ALL, 27, false, 0, 0);
    j.travelDistance=pointDistance(3,2);
    j.travelTime=j.travelDistance/j.speed;
    mEdges.push_back(a);
    mEdges.push_back(b);
    mEdges.push_back(c);
    mEdges.push_back(d);
    mEdges.push_back(e);
    mEdges.push_back(f);
    mEdges.push_back(g);
    mEdges.push_back(h);
    mEdges.push_back(i);
    mEdges.push_back(j);
    NavNode n0, n1, n2, n3;
    n0.firstEdge=0;
    n0.lastEdge=2;
    n1.firstEdge=2;
    n1.lastEdge=5;
    n2.firstEdge=5;
    n2.lastEdge=8;
    n3.firstEdge=8;
    n3.lastEdge=10;
    mNodes.push_back(n0);
    mNodes.push_back(n1);
    mNodes.push_back(n2);
    return;
    */


	// TODO: File Check
    if (progress!=NULL) {
        *progress=0;
    }
	//Open the File
	osmpbf::OSMFileIn inFile(osmPath);
	//Open the file
	if (!inFile.open()) {
		printf("ERROR: Could not open file: %s\n", osmPath.c_str());
        log.logFile<<"ERROR: Could not open file: " + osmPath <<"\n";
		return;
	}
	else {
		printf("Successfully opened file: %s\n", osmPath.c_str());
        log.logFile<<"Successfully opened file: " + osmPath << "\n";
	}
	/********** Parse Preparations *********/
	//Create a PrimitiveInputAdapter to handle primitive blocks read by the FileInput Adapter
	osmpbf::PrimitiveBlockInputAdaptor pbi;
    //Highway Filter - able tio dismiss any OSM-Edges that does not have the highway type
	osmpbf::KeyOnlyTagFilter highwayFilter("highway");
    //Charging Station Filter - will be used to fetch charging station nodes
    osmpbf::KeyValueTagFilter chargeStationFilter("amenity", "charging_station");
	//Hashmap to track fetched OSM-nodes and matching NavGraph-Nodes
	std::unordered_map<int64_t, uint32_t> fetchedNodes;
	uint32_t possibleWayCount = 0;

	/***************************************************************
	** First Step: Collect 'interesting' OSM nodes via WayStream **
	***************************************************************/
	printf("Parse Step 1: Looking for interersting OSM-Nodes... ");		//DEBUG Print
    log.logFile<<"Parse Step 1: Looking for interersting OSM-Nodes...\n";
	while (inFile.parseNextBlock(pbi)) {
		//Check if the parsed Block is empty
		if (pbi.isNull()) {
			continue;
		}
		else {
			highwayFilter.assignInputAdaptor(&pbi);
			highwayFilter.rebuildCache();		//Rebuild Cache 
			osmpbf::IWayStream wayStream(pbi.getWayStream());		//Get a wayStream from the input adapter
			while (!wayStream.isNull()) {							//Loop through all highways of this primitiv block
				//Look for highways with at least two nodes - those are valid osm streets
				if (highwayFilter.matches(wayStream) && wayStream.refsSize() >= 2) {
					possibleWayCount++;		//Found a interesting osm-highway
					osmpbf::IWayStream::RefIterator wayNodesIt(wayStream.refBegin());
					osmpbf::IWayStream::RefIterator endIt(wayStream.refEnd());
					while (wayNodesIt!=endIt) {						//Loop through the nodes of the way
                        //Add found OSM-Node ID and unmatched Graph-Node Symbol 0xFF..F
                        //Note: Repeated
                        fetchedNodes[*wayNodesIt] = 0xFFFFFFFF;
						wayNodesIt++;
					}
				}
				//Done with this Block, parse the next one
				wayStream.next();
			}
		}
	}
    if (progress!=NULL) {
        *progress=1;
    }
    printf("Found %d interersting OSM-Nodes.\n", possibleWayCount);		//DEBUG Print
	/***************************************************************
	 ********** Second Step: Fetch interesting osm-nodes ***********
	 ***************************************************************/
	printf("Parse Step 2: Fetch OSM-Nodes and build NavGraph-Nodes ... ");
    log.logFile<<"Parse Step 2: Fetch OSM-Nodes and build NavGraph-Nodes ...\n";
    inFile.reset();
	while (inFile.parseNextBlock(pbi)) {
		//Check if the parsed Block is empty
		if (pbi.isNull()) {
			continue;
		}
		else {
			//Get a nodeStream from the input adapter
			osmpbf::INodeStream nodeStream(pbi.getNodeStream());	
			chargeStationFilter.assignInputAdaptor(&pbi);
			//Loop through all nodes and check if they are "interesting", if so extract them
			while (!nodeStream.isNull()) {
				if (fetchedNodes.count(nodeStream.id())) {
					//Current node is contained in the interesting list
                    //So add a new entry to the osmNode-Vector
                    mOsmNodes.OsmNodes.emplace_back(nodeStream.id(), nodeStream.lond(), nodeStream.latd(), false);
					//Store the position in the vector in the hashmap
                    fetchedNodes[nodeStream.id()] = mOsmNodes.OsmNodes.size() - 1;
				}
				//Check the next Node
				nodeStream.next();
			}
		}
    }
    printf("Fetched %ld nodes.\n", mOsmNodes.OsmNodes.size());
    if (progress!=NULL) {
        *progress=2;
    }

	/**************************************************************
	 ***************** Third Step: Fetch the edges ****************
	 **************************************************************/
	printf("Parse Step 3: Fetching Edges ... ");
    log.logFile<<"Parse Step 3: Fetching Edges ... \n";
	inFile.reset();
	//We'll need additional filters for allowance, street type and speed
	osmpbf::KeyOnlyTagFilter speedFilter("maxspeed");
	//One-Way filter: Check if the highway is tagged oneway or the wayType is inherent oneway
	osmpbf::OrTagFilter onewayFilter({
		new osmpbf::BoolTagFilter("oneway", true),
		new osmpbf::KeyMultiValueTagFilter("highway",{ "motorway", "motorway_link" }),
		new osmpbf::KeyMultiValueTagFilter("junction",{ "roundabout" })
	});
	//Foot-Denied filter: Check if the highway is tagged as motorway (foot&bike forbidden) or explicitly foot=false
	osmpbf::OrTagFilter footDeniedFilter({
		new osmpbf::BoolTagFilter("motorroad", true),
		new osmpbf::AndTagFilter({ new osmpbf::KeyOnlyTagFilter("foot"), new osmpbf::BoolTagFilter("foot", false)})
	});
	//Bike-Denied filter: Check if the highway is tagged as motorway (foot&bike forbidden) or explicitly bike=false
	osmpbf::OrTagFilter bikeDeniedFilter({
		new osmpbf::BoolTagFilter("motorroad", true),
		new osmpbf::AndTagFilter({new osmpbf::KeyOnlyTagFilter("bicycle"), new osmpbf::BoolTagFilter("bicycle", false)})
	});
    uint32_t invEdgeCount = 0;      //Counting the encountered invalid edge types
    uint32_t invSrcTarNode=0;       //Counting the number of invalid source/target nodes
	while (inFile.parseNextBlock(pbi)) {
		//Check if the parsed Block is empty
		if (pbi.isNull()) {
			continue;
		}
		else {
			//Get a wayStream from the input adapter
			osmpbf::IWayStream wayStream(pbi.getWayStream());
			if (wayStream.isNull()) {
				continue;
			}
			//Assign pbi and rebuild Cache after each block
			speedFilter.assignInputAdaptor(&pbi);
			onewayFilter.assignInputAdaptor(&pbi);
			footDeniedFilter.assignInputAdaptor(&pbi);
			bikeDeniedFilter.assignInputAdaptor(&pbi);
			highwayFilter.assignInputAdaptor(&pbi);
			speedFilter.rebuildCache();
			onewayFilter.rebuildCache();
			footDeniedFilter.rebuildCache();
			bikeDeniedFilter.rebuildCache();
			highwayFilter.rebuildCache();
			//Loop through the ways, built up edge array and node array accordingly
			while (!wayStream.isNull()) {
				//Valid OSM-Street way? If not, next way and continue
				if (highwayFilter.matches(wayStream) && wayStream.refsSize() >= 2) {
					/************* Fetch & Store OSM Edge properties ****************/
					//Get the OSM-edge type String
					const std::string & edgeTypeString = wayStream.value(highwayFilter.matchingTag());	
					//Dummy NavEdge to store the properties of the osm edge
					NavEdge edge;
					//Set highway type of the edge
					if (streetTypeMap.count(edgeTypeString)) {
						//valid edge type
						edge.type = streetTypeMap.at(edgeTypeString);
					}
					else {
						//Invalid edge type
						edge.type = INVALID;
                        //log.logFile<<"Invalid Edge Type: " << edgeTypeString << ". Continue with next Edge.\n";
                        invEdgeCount++;
                        wayStream.next();
                        continue;
					}
					//Setting edgeSpeed
					if (speedFilter.matches(wayStream)) {
						//Explicitly stated max speed for this edge
						edge.speed=parseOsmSpeedTag(wayStream.value(speedFilter.matchingTag()));
					}
					else {
						//Implicit speed limit according to edge type (and also state, currently GERMAN speeds only)
						edge.speed = speedLookup[edge.type];
						//Convert kmh to meter per s
						edge.speed *= KMH_TO_MPS;
					}
					//Check if the edge is a explicit or implicit oneway edge
					if (onewayFilter.matches(wayStream)) {
						edge.isOneway = true;
					}

					//Set allowance depending on street type
					edge.allowance = allowanceLookup[edge.type];
					//Check Foot-Denied / Bike-Denied filters for additional disallowance
					if (footDeniedFilter.matches(wayStream)) {
						edge.allowance &= DISALLOW_FOOT;		//Explicitly Disallow Foot-Access
					}
					if (bikeDeniedFilter.matches(wayStream)) {
						edge.allowance &= DISALLOW_BIKE;		//Explicitly Disallow Bike-Access
					}
					/*** Build intermittend NavGraph edges with stored properties  **/
                    osmpbf::IWayStream::RefIterator cur(wayStream.refBegin());
                    osmpbf::IWayStream::RefIterator next(wayStream.refBegin()+1);
                    osmpbf::IWayStream::RefIterator last(wayStream.refEnd());
                    while (cur!=last) {
                        int64_t src=*cur;
                        int64_t tar=*next;
                        //Source/Target Node actually fetched?
                        if(fetchedNodes.count(src) && fetchedNodes.count(tar)) {
                            edge.source=fetchedNodes[src];
                            edge.target=fetchedNodes[tar];
                            //Calculate the distance between start and end node of the edge in metres
                            edge.travelDistance=pointDistance(edge.source, edge.target)*1000;
                            //Calculate travel time (in s) on this edge according to the distance and the allowed speed
                            edge.travelTime=edge.travelDistance / edge.speed;
                            //Insert the localy created edge into the edge vector
                            mEdges.push_back(edge);
                            //Also add the reverse edge if it is nor labeled one-way
                            if (!edge.isOneway) {
                                uint32_t tmp=edge.source;
                                edge.source=edge.target;
                                edge.target=tmp;
                                mEdges.push_back(edge);
                            }
                        }
                        else {
                            //This is bad. The edge starts/ends at a osm node that was not fetched previously!
                            invSrcTarNode++;
                        }
                        cur++;
                        next++;
                    }

                    /*osmpbf::IWayStream::RefIterator cur(wayStream.refBegin() + 1);	//Second Node of the edge
					osmpbf::IWayStream::RefIterator prev(wayStream.refBegin());		//First Node of the Edge
					osmpbf::IWayStream::RefIterator last(wayStream.refEnd());		//Last Node of the edge
                    while (cur!=last) {
						int64_t src = *prev;
						int64_t tar = *cur;
                        //Check if prev+last node of the current edge are already stored and initialized in fetchedNode vector
						if (fetchedNodes.count(src) && fetchedNodes[src] != 0xFFFFFFFF && fetchedNodes.count(tar) && fetchedNodes[tar]!=0xFFFFFFFF) {
							edge.source = fetchedNodes[src];	//The graphNode source of the edge
							edge.target = fetchedNodes[tar];	//The graphNode target of the edge
							//Calculate travel distance and time on this edge
							edge.travelDistance = pointDistance(edge.source, edge.target)*1000;		//in meters
							edge.travelTime = edge.travelDistance / edge.speed;
							//Insert edge to the edge vector
							mEdges.push_back(edge);
							if (!edge.isOneway) {
								//Also add backwards edge if it is not a oneway edge
								edge.source = edge.target;
								edge.target = fetchedNodes[src];
							}
                        }
						else {
							//prev or last is invalid => skip this edge 
						}
						cur++;
						prev++;
                    }*/
				}
				wayStream.next();
			}
		}
    }
    printf("Fetched %ld edges. Start sorting ... ", mEdges.size());
    if (progress!=NULL) {
        *progress=4;
    }
    log.logFile<<"Fetched " << mEdges.size() << " edges. Encountered " << invEdgeCount <<  " invalid Edges. Start sorting ... ";
	std::sort(mEdges.begin(), mEdges.end());
	printf("Sorting completed.\n");
    log.logFile<<" ... done.\n";
    if (progress!=NULL) {
        *progress=5;
    }

	/*****************************************************************
	 *** Fourth Step: Fill in NavGraph Node (completing the Graph) ***
	 *****************************************************************/
	printf("Parse Step 4: Building the graph ... ");
    log.logFile<<"Parse Step 4: Building the graph ... \n";
    mNodes.reserve(mOsmNodes.OsmNodes.size());		//Reserve enough memory - will be faster than just resize
    mNodes.resize(mOsmNodes.OsmNodes.size());		//Fill the vecor with initialized nodes
	uint32_t curNodeOffset = 0;
	mNodes[0].firstEdge = 0;				//firstEdge of the first Node starts at the front of the mEdge-Vector
	for (uint32_t i = 0; i < mEdges.size(); i++) {
		//Loop through the sorted edge list and fill in GraphNode offset informations
		if (mEdges[i].source!=curNodeOffset) {
            //This is the first edge with a new source node:
            //-> update the 'lastEdge' of the previous Node with current position - 1
            mNodes[curNodeOffset].lastEdge = i-1;
            //We finished the previous node, so go on to the next
			curNodeOffset++;
            //And set the 'firstEdge' to the current position
			mNodes[curNodeOffset].firstEdge = i;
		}
    }
    //Last Node extra handler: lastEdge is the last Edge parsed!
    mNodes[curNodeOffset].lastEdge=mEdges.size()-1;
    printf("Created Graph consists of: %ld Nodes and %ld Edges.\n", mNodes.size(), mEdges.size());
    log.logFile<<"Final Graph stats: " << mNodes.size() << " Nodes and " << mEdges.size() << " Edges.\n";
    if (progress!=NULL) {
        *progress=6;
    }

    /***** Fifth Step:  Build the Closeness lookup Graph *******/
    printf("Parse Step 5: Building the closeness tree ... ");
    log.logFile << "Parse Step 5: Build closeness tree ...";
    //built up the tree after all nodes were added
    closenessTree.buildIndex();
    printf(" success.\n");
    log.logFile<<" ... Done.\n";

    //Do Self Check
    printf("Self Check ...");
    log.logFile << "Parse Step 6: Perform self check ...\n";
    log.~LogWriter();
    if(!selfCheck()) {
        throw("Broken Graph!");
   }
    if (progress!=NULL) {
        *progress=7;
    }
}

const NavNode & NavGraph::getNavNode(uint32_t index)
{
	// TODO: hier Rückgabeanweisung eingeben
	return mNodes[index];
}

const OsmNode & NavGraph::getOsmNode(uint32_t index)
{
	// TODO: hier Rückgabeanweisung eingeben
    return mOsmNodes.OsmNodes[index];
}

Node & NavGraph::getNode(size_t index)
{
    if (index >= mOsmNodes.OsmNodes.size()) {
		throw("Index out of Bounds.");
	}
    return mOsmNodes.OsmNodes[index];
}

Edge & NavGraph::getEdge(size_t index)
{
	if (index >= mRouteEdges.size()) {
		throw("Index out of Bounds.");
	}
	return *(mRouteEdges[index]);
	// TODO: hier Rückgabeanweisung eingeben
}

const NavEdge & NavGraph::getEdge(const size_t nodeA, const size_t nodeB) const
{
    if (nodeA >= mEdges.size() || nodeB >= mEdges.size()) {
		throw("Index out of Bounds.");
	}
	for (uint32_t i = mNodes[nodeA].firstEdge; i < mNodes[nodeA].lastEdge; i++) {
		if (mEdges[i].target == nodeB) {
			return mEdges[i];
		}
		if (mEdges[i].target > nodeB) {
			//Break if current edge's target id is larger than target id
			throw("Edge not found!");
		}
	}
	throw("Edge not found!");
}

size_t NavGraph::numEdges()
{
	return mRouteEdges.size();
}

const NavEdge & NavGraph::getEdgeOfNode(uint32_t index, uint32_t offset) const
{
	if (mNodes[index].firstEdge+offset>mEdges.size()) {
        throw("Index out of Bounds.");
	}
	return mEdges[mNodes[index].firstEdge + offset];
}

const NavEdge & NavGraph::getEdgeFromTo(uint32_t startNode, uint32_t targetNode) const {
    if (startNode > mNodes.size() || targetNode>mNodes.size()) {
        throw("Index out of Bounds.");
    }
    uint16_t edgeCount = mNodes[startNode].lastEdge - mNodes[startNode].firstEdge;
    for (uint16_t i=0; i<edgeCount;i++) {
        if(mEdges[mNodes[startNode].firstEdge+i].target == targetNode) {
            return mEdges[mNodes[startNode].firstEdge+i];
        }
    }
    throw("Target not found.");
}

double NavGraph::parseOsmSpeedTag(const std::string & speedTag)
{
	//Check for walk speed tag
	if (speedTag.find("walk") != std::string::npos) {
		return 10;	//10 kmh
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

double NavGraph::pointDistance(uint32_t offsetNode_A, uint32_t offsetNode_B) const
{
	//NavNode wrapper
    return haversineDistance(mOsmNodes.OsmNodes[offsetNode_A].lat, mOsmNodes.OsmNodes[offsetNode_A].lon, mOsmNodes.OsmNodes[offsetNode_B].lat, mOsmNodes.OsmNodes[offsetNode_B].lon);
}

double NavGraph::haversineDistance(double lat_A, double lon_A, double lat_B, double lon_B) const
{
	//Step 1: convert degree to radian
	lat_A *= (PI / 180);
	lon_A *= (PI / 180);
	lat_B *= (PI / 180);
	lon_B *= (PI / 180);
	//Step 2: Calculate sinus of the delta of the points
	double u, v;
	u = sin((lat_B - lat_A) / 2);
	v = sin((lon_B - lon_A) / 2);
	//Do the actual Havensine calculation
	double ret = asin(sqrt(pow(u, 2) + cos(lat_A)*cos(lat_B)*pow(v, 2)));
	return (2.0*EARTH_RADIUS*ret);
}

double OsmNode::getLongitude()
{
	return this->lon;
}

double OsmNode::getLatitude()
{
	return this->lat;
}

uint32_t NavGraph::searchClosestNode(double lat_A, double lon_A) const {
    double origin [2] = {lon_A, lat_A};
    const size_t num_results = 1;           //We only search for the closest node
    size_t ret_index;                       //The index of the found point
    double out_dist_sqr;                    //Square distance to the closest node
    nanoflann::KNNResultSet<double> resultSet(num_results);
    resultSet.init(&ret_index, &out_dist_sqr );
    closenessTree.findNeighbors(resultSet, &origin[0], nanoflann::SearchParams(10));
    return ret_index;
}

bool NavGraph::selfCheck() {
    LogWriter log(false);
    log.logFile << "[NavGraph::selfCheck]\n";
    //check existence of all nodes/edges
    uint32_t nc = mNodes.size();
    uint32_t ec = mEdges.size();
    log.logFile << "Starting self check of parsed graph ...\n";
    log.logFile.flush();
    for(uint32_t i=0; i < nc; i++) {
        const NavNode n = mNodes[i];
        if (n.firstEdge == 0xFFFFFFFF || n.lastEdge == 0xFFFFFFFF) {
            if (n.firstEdge != n.lastEdge) {
                log.logFile << "Node[" << i << "] BROKEN: Half initialized: \n";
                log.logFile << n.firstEdge << ", " << n.lastEdge << "\n";
                return false;
            }
        }
        else if (n.firstEdge >= ec || n.lastEdge < n.firstEdge) {
            log.logFile << "Node[" << i << "] BROKEN: Offset out of bounds: \n";
            log.logFile << n.firstEdge << ", " << n.lastEdge << ", Edge-Size " << ec << ", Node-Size " << nc << "\n";
            return false;
        }
        for (uint32_t j=0; j<mNodes[i].lastEdge - mNodes[i].firstEdge;j++) {
            //Check if the target of the edge has the current node as source
            const NavEdge e = mEdges[mNodes[i].firstEdge+j];
            if (e.source != i) {
                log.logFile << "Node[" << i << "], Edge" << j <<" BROKEN: Edge has not set current Node as Source!\n";
                log.logFile << "Edge Source: " << e.source << ", current Node: " << i << "\n";
                return false;
            }
            if (e.target >= nc) {
                log.logFile << "Node[" << i << "], Edge " << j <<" BROKEN: Edge Target out of bounds!\n";
                log.logFile << "Edge Target: " << e.target << ", current Index bound: " << nc << "\n";
                return false;
            }
        }
    }
    log.logFile<< "Self-Check successful!\n";
    return true;
}
