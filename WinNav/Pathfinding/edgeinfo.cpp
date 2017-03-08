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


double EdgeInfo::getEdgeCost (bool timeIsPrio, const double &mediumMaxSpeed) {
    if (distance == 0.0) {
        //invalid edge info
        return 0xFFFFFFFF;
    }
    double curSpeed=speed;
    //If a medium max speed is set, we'll take the lower of the medium max speed
    // and the allowed speed of this edge for the actual speed for the calculation
    //NOTE: This is important to make sure the cost for an edge are not cheaper than possible
    if(mediumMaxSpeed>0.0) {
        curSpeed = std::min<double> (mediumMaxSpeed, this->speed);
    }
    //Calculate the cost according to the priority selection
    if (timeIsPrio && curSpeed > 0.0) {
        return distance/curSpeed;
    }
    else if (timeIsPrio && curSpeed <= 0.0) {
        //Invalid speed tag
        return 0xFFFFFFFF;
    }
    else {
        //Return the travel distance
        return distance;
    }
}

void EdgeInfo::print() {
    el::Logger* logger = el::Loggers::getLogger("default");
    logger->debug("EdgeInfo :: Allow=%v : Dist=%v : Speed=%v : Type=%v", allowance, distance, speed, type);
    //printf ("EdgeInfo :: Allow=%2.2X : Dist=%f : Speed=%f : Type=%2.2X", allowance, distance, speed, type);
}
