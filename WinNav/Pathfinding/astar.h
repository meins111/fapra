#ifndef ASTAR_H
#define ASTAR_H
#include <boost/heap/fibonacci_heap.hpp>
#include <vector>
#include "navgraph.h"
#include "TestGraph/lineargraph.h"
#include <unordered_set>
#include <unordered_map>
#include "Utils/condwait_t.h"
#include "Utils/easylogging++.h"
#include "Utils/ErrorCodes.h"
#include "allowancemasks.h"
#include <cmath>

//Max speed on any edge by travel medium - used in the a*-time heuristic
#define MAXSPEED_CAR_MPS 130*0.27778
#define MAXSPEED_BIKE_MPS 15*0.27778
#define MAXSPEED_FOOT_MPS 6*0.27778


struct OpenNode_t {
    //Node ID in the graph -> used to access node informations
    size_t id;
    //The partially-heuristic cost to target information
    double hScore;
    //The absolute cost to reach this node
    double gScore;
    OpenNode_t (const size_t &id, const double &hScore, const double &gScore) : id(id), hScore(hScore), gScore(gScore){}
    OpenNode_t(const OpenNode_t &copy): id(copy.id), hScore(copy.hScore), gScore(copy.gScore){}
    OpenNode_t & operator= ( const OpenNode_t & copy) {
                                                        id=copy.id;
                                                        hScore=copy.hScore;
                                                        gScore=copy.gScore;
                                                      }
};

struct CompareOpenNode_t {
    //Will result in a min heap rather than the default boost max heap implementation
    bool operator() (const OpenNode_t &a, const OpenNode_t &b) const {
        return a.hScore > b.hScore;
    }
};

//Fibonacci heap structure with custom comperator method to construct a min priority queue
typedef boost::heap::fibonacci_heap<OpenNode_t, boost::heap::compare<CompareOpenNode_t>> FibHeap_t;
typedef typename boost::heap::fibonacci_heap<OpenNode_t, boost::heap::compare<CompareOpenNode_t>>::handle_type Handle_Type_t;

class AStar
{
public:
    AStar(NavGraph &navGraph);

    //NOTE: currently emitted progress/errorCodes will directly go to the gui, which may not be wanted if
    //the current routing task has to run multiple a* runs ... will confuse programm & user!
    void findRoute (const size_t&start, const size_t &target, CondWait_t *condStruct, bool emitDoneSignal);

    void getRoute(LinearGraph &retRoute);

    int getErrorCode() { return errorCode; }

    void setMedium (TravelMedium medium);

    void setMaxDistance (double maxRange) { isMaxRangeSet = true;
                                            maxRange = maxRange;
                                          }
    void setRoutingPrio (bool timeIsPrio) { this->timeIsPrio=timeIsPrio; }

    void setNavNodeKeepFlag (bool keep) { keepNavNodes=keep; }

    double getRouteCost () { return totalCost; }

    double getRouteDistance() { return totalDistance; }

    double getRouteTravelTime () { return totalTime; }

    void getNavNodesOnRoute(std::vector<size_t> &nodes);

//DEBUG ONLY
//    void getVisitedNodes (std::vector<PODNode> &visited);

private:
    //The error code of the last routing run
    int errorCode;
    //The graph to route on
    NavGraph &graph;
    //Is there a max range set?
    bool isMaxRangeSet;
    //Maximum range of the medium
    double maxRange;
    //The resulting best path (with respective to the cost function)
    LinearGraph route;
    bool routeIsBuild;
    //The travel medium to use for the routing
    TravelMedium curMedium;
    //The maximum speed allowed for the respective travel medium: used for shortest time routing
    double curMaxSpeed;

    //The total cost of the last calculated route
    double totalCost;
    double totalDistance;
    double totalTime;

    //Flag if to store nov node ids during econstruction
    bool keepNavNodes;
    std::vector<size_t> routeNavNodes;
    std::vector<size_t> routeNavEdges;

    //debug array storing all expanded nodes
    //std::vector<NodeInfo> expandedNodes;

    //Flag indicates which criteria has to be tracked: travel time (true) or travel distance (false)
    bool timeIsPrio;
    //This priority queue contains all nodes which were found, but not yet finally expanded
    //The top element is the element with the smallest heuristic-to-target value (hScore)
    FibHeap_t prioQueue;
    //Map containing the mapping of all nodes in the prioQueue to their respective handlers allowing
    //r+w access to arbitrary nodes in the prioQueue by their node ID
    std::unordered_map <size_t, Handle_Type_t> openSet;
    //This set contains all node-IDs which were expanded and their shortest paths from start known
    //thus these nodes must not be expanded again
    std::unordered_set <size_t> closedSet;

    //This map stores the node ID from which this node can be reached with minimal cost
    //use this to create result path after successful run with repititive calls starting at the end path
    std::unordered_map <size_t, size_t> predecessorMap;

    //Simple graph integrity check
    bool checkGraph();

    //Utility method to fetch the heuristic distance from one node to another with respect to routing criteria
    double h(const size_t &n1, const size_t &n2);

    //Utility method to update the progress
    void calculateProgress(const OpenNode_t &cur, size_t start, size_t target, CondWait_t *updateStruct);

    //Utility method to construct the final path after the routing was successful
    void constructPath (size_t start, size_t target, CondWait_t *updateStruct);
};

#endif // ASTAR_H
