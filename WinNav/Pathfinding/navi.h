#ifndef Navi_H
#define Navi_H
#include "basicedge.h"
#include "basicnode.h"
#include "basicgraph.h"
#include "nodeinfo.h"
#include "edgeinfo.h"
#include "allowancemasks.h"
#include "roadtypes.h"
#include "navgraph.h"
#include "closenesstree.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "Utils/easylogging++.h"
#include "../Utils/inplacevectorreorder.h"
#include <Utils/condwait_t.h>
#include <TestGraph/lineargraph.h>
#include "astar.h"
//osmpbf includes
#include <osmpbf/parsehelpers.h>
#include <osmpbf/filter.h>
#include <osmpbf/iway.h>
#include <osmpbf/inode.h>

//Constant to convert from km/h to meter per second
#define KMH_TO_MPS 0.27778
//Constant to convert from miles/h to meter per second
#define MPH_TO_MPS 0.44704
//The number of OSM-Highway types we want to use
#define OSM_HIGHWAY_TYPES 27




class Navi
{
public:
    Navi() : pathfinder(fullGraph), fullGraph(), metaGraph(), metaGraphPrepared(false),
        medium(CAR), timeIsPrio(true), maxRange(0), startRange(0){}

    //void parsePbfFile(const std::string &path);
    void parsePbfFile(const std::string &path, CondWait_t *updateStruct);

    //Actual Routing from a start node to a target node
    void shortestPath(PODNode start, PODNode target, CondWait_t *updateStruct);

    //Method to complete the metaGraph structure in the background
    void buildMetaGraph(CondWait_t *updateStruct);

    void setTravelMedium (const TravelMedium medium);
    void setMaxRange(const size_t range);
    void setStartRange(const uint8_t range);
    void setRoutingPriority(bool travelTimePriority);

    void getFullGraph (LinearGraph &graph);
    void getShortestRouteGraph(LinearGraph &graph);

    void reset();

protected:
    //The actual pathfinding implementation
    AStar pathfinder;

    //The full street network: not meta-edges allowed
    NavGraph fullGraph;

    //The meta graph containing only shortest-path-connected charge station nodes (most likely via meta-edges)
    NavGraph metaGraph;
    bool metaGraphPrepared;


    //Lookup to match the EdgeType and its String representation
    const static std::unordered_map<std::string, EdgeType> streetTypeLookup;
    //Lookup array using the edgetype as an index to match its default allowance
    const static uint8_t allowanceLookup[OSM_HIGHWAY_TYPES];
    //Lookup array using the edgetype to match its default speed limit by type
    const static uint16_t speedLookup[OSM_HIGHWAY_TYPES];
    //km/h string to meters per second method
    double parseOsmSpeedTag(const std::string & speedTag);
    //Utility method to check the integrity of the stored graph
    bool selfCheck();


    //Build the edges of the meta graph: This includes MANY a* runs - depending on the amount of known recharge stations
    void buildMetaEdges();

    //Routing options
    TravelMedium medium;
    bool timeIsPrio;
    size_t maxRange;
    uint8_t startRange;

};

#endif // Navi_H
