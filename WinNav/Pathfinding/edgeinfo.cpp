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
