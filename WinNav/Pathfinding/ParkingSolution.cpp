#include "ParkingSolution.h"

ParkingSolution::ParkingSolution()
{

}

bool ParkingSolution::operator ==(const ParkingSolution &request) const {
    if (request.publicParking && !publicParking)
        return false;
    if (request.privateParking && !privateParking)
        return false;
    if (request.customerParking && !customerParking)
        return false;
    if (request.freeParking && !freeParking)
        return false;
    if (request.constraintTime && !constraintTime)
        return false;
    //Positive equalitiy match -> return true
    return true;
}

bool ParkingSolution::operator !=(const ParkingSolution &request) const {
    //Inverse positive match
    return !(*this==request);
}



void ParkingSolution::buildFromDedicatedParkingFilter(osmDedicatedParkingFilter_t &filter, osmpbf::INodeStream &nodes) {
    if (filter.publicParkingDeniedFilter.matches(nodes)) {
        publicParking=false;
    }
    else {
        publicParking=true;
    }
    if (filter.customerParkingFilter.matches(nodes)) {
        customerParking=true;
    }
    else {
        customerParking=false;
    }
    if (filter.privateParkingFilter.matches(nodes)) {
        privateParking=true;
    }
    else {
        privateParking=false;
    }
    if (filter.parkingFeeFiler.matches(nodes)) {
        freeParking=false;
    }
    else {
        freeParking=true;
    }
    if (filter.timeLimitParkingFilter.matches(nodes)) {
       constraintTime=true;
    }
    else{
        constraintTime=false;
    }
}

void ParkingSolution::buildFromDedicatedParkingFilter(osmDedicatedParkingFilter_t &filter, osmpbf::IWayStream &ways) {
    if (filter.publicParkingDeniedFilter.matches(ways)) {
        publicParking=false;
    }
    else {
        publicParking=true;
    }
    if (filter.customerParkingFilter.matches(ways)) {
        customerParking=true;
    }
    else {
        customerParking=false;
    }
    if (filter.privateParkingFilter.matches(ways)) {
        privateParking=true;
    }
    else {
        privateParking=false;
    }
    if (filter.parkingFeeFiler.matches(ways)) {
        freeParking=false;
    }
    else {
        freeParking=true;
    }
    if (filter.timeLimitParkingFilter.matches(ways)) {
       constraintTime=true;
    }
    else{
        constraintTime=false;
    }
}

void ParkingSolution::buildFromParkingLaneFilter(osmParkingLaneFilter_t &filter, osmpbf::IWayStream &ways) {
    //Obviously, as we parse from a lane parking filter!
    streetParking=true;
    if (filter.publicParkingDeniedFilter.matches(ways)) {
        publicParking=false;
    }
    else {
        publicParking=true;
    }
    if (filter.customerParkingFilter.matches(ways)) {
        customerParking=true;
    }
    else {
        customerParking=false;
    }
    if (filter.privateParkingFilter.matches(ways)) {
        privateParking=true;
    }
    else {
        privateParking=false;
    }
    if (filter.freeParkingFiler.matches(ways)) {
        freeParking=true;
    }
    else {
        freeParking=false;
    }
    if (filter.timeLimitParkingFilter.matches(ways)) {
       constraintTime=true;
    }
    else{
        constraintTime=false;
    }
}

