#ifndef NODEINFO_H
#define NODEINFO_H

#define PI 3.14159265358979323846	//Some Pi numbers
#define degreeToRadian  PI/180      //Used to convert from degree to radian
#define EARTH_RADIUS 6371.0         //recommended radius by the International Union of Geodesy and Geophysics
#define LATITUDE_KM 110.574         //One Latitude is defined to be 110,574 km in length

#include <math.h>
#include <stdint.h>
#include <stddef.h>
#include "GraphInterface/Node.h"
#include "Pathfinding/ParkingSolution.h"

class NodeInfo : public  Node
{
public:
    NodeInfo() : allowsParking(false) , parkingPropertyID(0xFFFFFFFF), osmID(0xFFFFFFFFFFFFFFFF), localID(0xFFFFFFFF), longitude(0.0), latitude(0.0){}
    NodeInfo(const NodeInfo &copy): allowsParking(copy.allowsParking), parkingPropertyID(copy.parkingPropertyID), longitude(copy.longitude), latitude(copy.latitude),  osmID(copy.osmID), localID(copy.localID){}
    NodeInfo(bool parking, size_t parkingInfoID, int64_t osmID, size_t localID, double lon, double lat):
        allowsParking(parking), parkingPropertyID(parkingInfoID), osmID(osmID), localID(localID), longitude(lon), latitude(lat){}
    NodeInfo & operator= ( const NodeInfo & copy);

    // Flag indicating if this node is (part of) a parking solution or not
    bool allowsParking;
    size_t parkingPropertyID;



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

    //Utility method to forge a point with a set of coordinates that are about dist-meters from this point
    NodeInfo forgePointWithDistance (double dist) const;

};

#endif // NODEINFO_H
