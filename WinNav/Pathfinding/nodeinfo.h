#ifndef NODEINFO_H
#define NODEINFO_H

#define PI 3.14159265358979323846	//Some Pi numbers
#define degreeToRadian  PI/180      //Used to convert from degree to radian
#define EARTH_RADIUS 6371.0         //recommended radius by the International Union of Geodesy and Geophysics

#include <math.h>
#include <stdint.h>
#include <stddef.h>
#include "GraphInterface/Node.h"

class NodeInfo : public  Node
{
public:
    NodeInfo() : longitude(0.0), latitude(0.0), isChargeStation(false), osmID(0xFFFFFFFFFFFFFFFF), localID(0xFFFFFFFF){}
    NodeInfo(const NodeInfo &copy): longitude(copy.longitude), latitude(copy.latitude), isChargeStation(copy.isChargeStation), osmID(copy.osmID), localID(copy.localID){}
    NodeInfo(double lat, double lon, bool isCharge, int64_t osmID, size_t localID): latitude(lat), longitude(lon), isChargeStation(isCharge), osmID(osmID), localID(localID){}
    NodeInfo & operator= ( const NodeInfo & copy);

    // Flag indicates if a E-Car can recharge at this node
    bool isChargeStation;
    // Original ID of this node in the open street map database
    int64_t osmID;
    // ID (=offset) of this node in the local offset array representation
    size_t localID;

    /* Node Interface */
    double getLongitude()   { return longitude; }
    double getLatitude()    { return latitude;  }

    /* Basic Node Information: Coordinates */
    double longitude;
    double latitude;

    //Calculate the haversine distance from this node to another node
    double getHaversineDistanceTo (const NodeInfo &targetNode);

};

#endif // NODEINFO_H
