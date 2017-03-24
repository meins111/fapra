#include "nodeinfo.h"

double NodeInfo::getHaversineDistanceTo (const NodeInfo &targetNode) {
    //Step 1: convert degree values to radian
    double lat = latitude * degreeToRadian;
    double lon = longitude * degreeToRadian;
    double tarLat = targetNode.latitude * degreeToRadian;
    double tarLon = targetNode.longitude * degreeToRadian;

    //Step 2: Calculate the sinus of the delta of the points
    double u, v;
    u = sin((tarLat - lat) / 2);
    v = sin((tarLon - lon) / 2);

    //Step 3: Calculate the Havensine distance and return it
    double inKm = 2*EARTH_RADIUS*asin(sqrt(pow(u, 2) + cos(lat)*cos(tarLat)*pow(v, 2)));
    //Return distnce in meters
    return inKm*1000;
}
NodeInfo & NodeInfo::operator= ( const NodeInfo & copy) {
   longitude=copy.longitude;
   latitude=copy.latitude;
   isChargeStation=copy.isChargeStation;
   osmID=copy.osmID;
   allowsParking=copy.allowsParking;    
   localID=copy.localID;
   parkingPropertyID=copy.parkingPropertyID;
   return *this;
}

NodeInfo NodeInfo::forgePointWithDistance (double dist) const {
    //Calculate the coordinates of the forged point, given the distance and the coordinatees of this node
    //To reduce the number of unknown to one, we can state that one of the two terms results to zero
    //by setting lonB/latB to the same value as the respective value of this node - we'll take longitude
    NodeInfo forged;
    //Simply use the longitude of the start node
    forged.longitude=this->longitude;
    //And add the distance on top of its latitude value, using the defined latitude length in km to compute directly on latitude-degree's
    forged.latitude =this->latitude + (dist/1000)/LATITUDE_KM;
    return forged;
}
