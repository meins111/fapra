#pragma once
#include <stdint.h>
#include <vector>
#include <string.h>
#include <unordered_map>
#include "../Graph/Node.h"
#include "../Graph/Edge.h"
#include "../Graph/Graph.h"
#include <nanoflann.hpp>
#include <stdio.h>
//File I/O for debug log
#include "../logwriter.h"

#define OSM_HIGHWAY_TYPES 27
//Combinable Bit-Flags for street allowance
#define ALLOW_NONE 0	
#define ALLOW_FOOT 0x1
#define ALLOW_BIKE 0x2
#define ALLOW_CAR 0x4
#define ALLOW_ALL 0x7
#define DISALLOW_FOOT 0xFE
#define DISALLOW_BIKE 0xFD
//Constant to convert from km/h to meter per second
#define KMH_TO_MPS 0.27778
//Constant to convert from miles/h to meter per second
#define MPH_TO_MPS 0.44704
//Constants used to calculate the haversine distance
#define PI 3.14159265358979323846	//Some Pi numbers
#define EARTH_RADIUS 6371.0	//recommended by the International Union of Geodesy and Geophysics

using namespace nanoflann;

typedef enum {
	//All the possible OSM Road Types
	INVALID=0,
	MOTORWAY, TRUNK, PRIMARY, SECONDARY, TERTIARY, 
	UNCLASSIFIED, RESIDENTIAL, SERVICE,	
	MOTORWAY_LINK, TRUNK_LINK, PRIMARY_LINK, SECONDARY_LINK, TERTIARY_LINK,
	LIVING_STREET, PEDESTRIAN, TRACK, 
	BUS_GUIDE_WAY, RACEWAY, ROAD,
	FOOTWAY, BRIDLEWAY, STEPS, PATH, 
	CYCLEWAY, BUS_STOP, PLATORM
} EdgeType;



/*@NavNode*/
struct NavNode {
	uint32_t firstEdge;			//Offset into the NavEdgeArray, were the adjacent edges of one node starts
	uint32_t lastEdge;			//Offset into the NavEdgeArray, were the adjacent edges of one node ends
	NavNode() : firstEdge(0xFFFFFFFF), lastEdge(firstEdge) {}
};
/*@NavEdge*/
struct NavEdge : public Edge {
	uint32_t source;						//Offset into the NavNodeArray = the NavNode were this edge starts
	uint32_t target;						//Offset into the NavNodeArray = the NavNode were this edge ends
	EdgeType type;							//edge type
	uint8_t allowance;						//Which type of movement (car|bike|foot) is allowed on this edge
	double speed;							//Max speed on this edge, UNIT=meter per second 
	bool isOneway;							//Flag if the edge is oneway only or not
	double travelDistance;					//Total travel distance on this edge; UNIT=meters
	double travelTime;						//Total travel time on this edge; UNIT=seconds	

	bool operator < (const NavEdge& operand);	//Comparison Operator used to sort Edges according to source/target offset -->std::sort
	NavEdge(): source(0xFFFFFFFF), target(0xFFFFFFFF), type(INVALID), allowance(ALLOW_ALL), speed(0.0), isOneway(false), travelDistance(0.0), travelTime(0.0) {}
	NavEdge(const NavEdge &copy) : source(copy.source), target(copy.target), type(copy.type), 
		allowance(copy.allowance), speed(copy.speed), isOneway(copy.isOneway), travelDistance(copy.travelDistance), travelTime(copy.travelTime) {};
    NavEdge(uint32_t src, uint32_t tar, EdgeType type, uint8_t allow, double spd, bool flag, double dist, double time) :
        source(src), target(tar), allowance(allow), speed(spd), isOneway(flag), travelDistance(dist), travelTime(time){}

    //Edge Interface
	virtual size_t getStart();
	virtual size_t getEnd();
};
/*@OSM-Node*/
struct OsmNode : public Node {
	int64_t ID;
	double lon;
	double lat;
	bool isChargeStation;
	OsmNode(int64_t ID, double lon, double lat, bool charge) : ID(ID), lon(lon), lat(lat), isChargeStation(charge){}	//Constructor with init. list

																				//Node Interface
	virtual double getLongitude();
	virtual double getLatitude();
};

struct OsmNodeCloud {
    std::vector<OsmNode> OsmNodes;

    ///Nanoflann custom class interface methods:
    // Must return the number of data points
    inline size_t kdtree_get_point_count() const { return OsmNodes.size(); }

    // Returns the distance between the vector "p1[0:size-1]" and the data point with index "idx_p2" stored in the class
    inline double kdtree_distance(const double *p1, const size_t idx_p2,size_t /*size*/) const
    {
            const double d0=p1[0]-OsmNodes[idx_p2].lon;
            const double d1=p1[1]-OsmNodes[idx_p2].lat;
            return d0*d0+d1*d1;
    }

    // Returns the dim'th component of the idx'th point in the class:
    // Since this is inlined and the "dim" argument is typically an immediate value, the
    //  "if/else's" are actually solved at compile time.
    inline double kdtree_get_pt(const size_t idx, int dim) const
    {
            if (dim==0) return OsmNodes[idx].lon;
            else return OsmNodes[idx].lat;
    }

    // Optional bounding-box computation: return false to default to a standard bbox computation loop.
    //   Return true if the BBOX was already computed by the class and returned in "bb" so it can be avoided to redo it again.
    //   Look at bb.size() to find out the expected dimensionality (e.g. 2 or 3 for point clouds)
    template <class BBOX>
    bool kdtree_get_bbox(BBOX& /* bb */) const { return false; }

};

//nanoflann tree template
typedef KDTreeSingleIndexAdaptor<L2_Simple_Adaptor<double, OsmNodeCloud>, OsmNodeCloud, 2> FlannTree;

class NavGraph : public Graph
{
public:
    NavGraph(): closenessTree(2, mOsmNodes, KDTreeSingleIndexAdaptorParams(20/* max leaf nodes */)){}
    ~NavGraph();
    void buildFromFile(const std::string &osmPath, uint8_t *progress);	//Construct NavGraph from pointed OSMPBF-File

	inline const NavNode& getNavNode(uint32_t index);
	inline const OsmNode& getOsmNode(uint32_t index);

	const static uint8_t allowanceLookup[OSM_HIGHWAY_TYPES];			//Street Allowance lookup array, matches edge type enum value to its allowance
    const static uint16_t speedLookup[OSM_HIGHWAY_TYPES];				//Max Speed Allowance lookup array, matches edge type enum to its max speed

    const static std::unordered_map<std::string, EdgeType> streetTypeMap;	//Map of OSM-NodeType-Strings to EdgeType-Enum Values

	//Graph Interface
	virtual Node &getNode(size_t index);
	virtual Edge &getEdge(size_t index);			//The index' edge of a route
	virtual const NavEdge &getEdge(const size_t nodeA,  const size_t nodeB) const;
	virtual size_t numEdges();


	/** Computes the Havensine distance of two points on a ideal spere
	 * @param lat_A The latitude of the first point. UNIT=degree
	 * @param lon_A The longitude of the first point. UNIT=degree
	 * @param lat_B The latitude of the first point. UNIT=degree
	 * @param lon_B The longitude of the first point. UNIT=degree
	 * return The distance between the two points. UNI=kilometers.
	 */
	double haversineDistance(double lat_A, double lon_A, double lat_B, double lon_B) const;
	double pointDistance(uint32_t offsetNode_A, uint32_t offsetNode_B) const;		//Haversine distance between two NavGraph points

    uint32_t searchClosestNode(double lat_A, double lon_A) const;

	//TODO: make these private and implement some access methods		(OPTIONAL)
	std::vector<NavNode> mNodes;			//NavNode Array
    //std::vector<OsmNode> mOsmNodes;			//OsmNode Array, sorted in a way such that NavNode[i]<=>OsmNode[i]
    OsmNodeCloud mOsmNodes;
    std::vector<NavEdge> mEdges;			//NavEdge Array
	std::vector<int64_t> chargeStations;	//Stores the OsmNodesIds of all known charge stations

	const NavEdge & getEdgeOfNode(uint32_t index, uint32_t offset) const;
    const NavEdge & getEdgeFromTo(uint32_t startNode, uint32_t targetNode) const;
private:
    std::vector<NavEdge*> mRouteEdges;                          //A route consists of several edge references
    double parseOsmSpeedTag(const std::string &speedTag);		//parses the OSM speed tag for max edgeSpeed and converts the given unit to meter per second
    FlannTree closenessTree;                                    //KD-Tree to lookup the nearest point(s) of a given coordinate

    bool selfCheck();
};
