#pragma once
#include <stdint.h>
#include <vector>
#include <string.h>
#include <queue>
#include <float.h>
#include <unordered_set>
#include <unordered_map>
#include "NavGraph.h"
#include "../Graph/Node.h"
#include "../Graph/Route.h"
#include "../logwriter.h"
#include <boost/heap/fibonacci_heap.hpp>

#define TRAVEL_MEDIUM_NUMBER 3

enum travelMedium {
	CAR=0x4,
	BIKE=0x2,
	FOOT=0x1
};


struct RoutingNode {
    uint32_t mID;                   //the index of the node <=> node index in NavGraph node list
    uint32_t mPredecessorID;        //The index of the predecessor node in the closedList
    double mNodeWeight;             //distance walked until this node = distance(predecessor)+ edgeWeight (predecessor, routeNode)
    double mDistToTarget;           //heuristic distance approximation to the target of the search

    RoutingNode() : mID(0xFFFFFFFF), mPredecessorID(0xFFFFFFFF), mNodeWeight(DBL_MAX), mDistToTarget(DBL_MAX) {}
    RoutingNode(uint32_t ID, uint32_t preID, double weight, double dist) : mID(ID), mPredecessorID(preID), mNodeWeight(weight), mDistToTarget(dist) {}
	RoutingNode & operator= (const RoutingNode &copy);
    RoutingNode(const RoutingNode &copy);
};

struct lesser {
	bool operator()(const RoutingNode &compA, const RoutingNode &compB) const;
};

class Navigator
{
public:
    Navigator();
    Navigator(travelMedium meanOfTravel, bool timePriorityFlag) : medium(meanOfTravel), timeIsPriority(timePriorityFlag) {}
	~Navigator();
	Route& route(const uint32_t sourceID, const uint32_t targetID);
    Route& route(const double lat_A, const double lon_A, const double lat_B, const double lon_B);
	//TODO: Setter für meanOfTravel, timePriorityFlag
	void setTravelMedium(enum travelMedium medium);
    void setPriority(bool timePriorityFlag);
    void buildNavGraph(std::string &path, uint8_t *progress);
//Node& getRoutingNodePosition (const double lat, const double lon);

private:
	bool timeIsPriority;
	travelMedium medium;
    NavGraph mGraph;
	//std::priority_queue <RouteNode, std::vector<RouteNode>, lesser> openList;
	typedef boost::heap::fibonacci_heap<RoutingNode, boost::heap::compare<lesser>> OpenList;
	OpenList openList;	//boost fibonacci heap, instead of std::prio_queue because boost supports find&decrease_key operations
	std::unordered_map <uint32_t, OpenList::handle_type> handleMap;		//matching of graph node id's and their handles in the open list - needed for find/decrease_key operations
    std::unordered_set <uint32_t> closedList;
    std::vector<RoutingNode> closedNodeObjects;
    void expandNode(const RoutingNode &current, const uint32_t targetID);
    void buildPath(const RoutingNode &startNode, const RoutingNode &finalNode);
	Route path;			//The last calculated path
};

