#include "edgeinfo.h"

double EdgeInfo::getTravelTime()
{
    if (speed==0.0) {
        return 0.0;
    }
    else {
        return distance/speed;
    }
}
bool EdgeInfo::checkAllowance(uint8_t travelMedium) {
    if ((travelMedium & allowance) == 0x00) {
        return false;
    }
    else {
        return true;
    }
}

EdgeInfo & EdgeInfo::operator= ( const EdgeInfo & copy) {
    allowance=copy.allowance;
    speed=copy.speed;
    subEdges=copy.subEdges;
    isMetaEdge=copy.isMetaEdge;
    distance=copy.distance;
    isOneWay=copy.isOneWay;
    type=copy.type;
    return *this;
}


double EdgeInfo::getEdgeCost (bool timeIsPrio) {
    if (distance == 0.0) {
        //invalid edge info
        return 0xFFFFFFFF;
    }
    if (timeIsPrio && speed > 0.0) {
        //Return travel time on this edge
        return distance/speed;
    }
    else if (timeIsPrio && speed <= 0.0) {
        //Invalid speed tag
        return 0xFFFFFFFF;
    }
    else {
        //Return the travel distance
        return distance;
    }
}
