#include "edgeinfo.h"

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
    isOneWay=copy.isOneWay;
    type=copy.type;
    parking=copy.parking;
    parkingInfoID=copy.parkingInfoID;
    return *this;
}




void EdgeInfo::print() {
    el::Logger* logger = el::Loggers::getLogger("default");
    logger->debug("EdgeInfo :: Allow=%v : Speed=%v : Type=%v", (uint8_t)allowance, speed, (uint8_t)type);
    //printf ("EdgeInfo :: Allow=%2.2X : Dist=%f : Speed=%f : Type=%2.2X", allowance, distance, speed, type);
}
