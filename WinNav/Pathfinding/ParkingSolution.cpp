#include "ParkingSolution.h"

ParkingSolution::ParkingSolution()
{

}

bool ParkingSolution::matches(const ParkingSolution &request) const {
    //If the request searches for public parking, and it is not set return false
    if (request.publicParking && !publicParking) {
        return false;
    }
    //If the request searches for customer parking, and this parking is whether a customer nor a public parking
    if (request.customerParking && (!customerParking && !publicParking)) {
        return false;
    }
    //If the request searches for private parking, and this parking is whether a private nor a public parking
    if (request.privateParking && (!privateParking && !publicParking)) {
        return false;
    }
    //If the request searches for free parking and it is not set
    if (request.freeParking && !freeParking) {
        return false;
    }
    //If the request searches for unlimited parking and the parking spot is limited ...
    if (!request.constraintTime && constraintTime) {
        return false;
    }
    //If all the above does not apply ... we have a match!
    return true;
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

