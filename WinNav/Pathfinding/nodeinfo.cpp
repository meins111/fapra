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
   localID=copy.localID;
   return *this;
}
