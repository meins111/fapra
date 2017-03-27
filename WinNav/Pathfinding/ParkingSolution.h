#ifndef PARKINGSOLUTION_H
#define PARKINGSOLUTION_H

#include "Utils/OsmFilterContainer.h"
#include <osmpbf/inode.h>
#include <osmpbf/iway.h>

class ParkingSolution
{
public:
    ParkingSolution();

    /** Utility method to check the current object against a request.
     * @brief matches Return true if the object matches the request according to the parameters.
     * @param request The request who stores the search parameters.
     * @return True if the object matches the request. False otherwise.
     * Checking private/customer parameters will also consult the public parking parameter.
     * If searching for private/customer parking, public spots will also return true if the other parameters also match.
     */
    bool matches(const ParkingSolution &request) const;

    //Utility method to fetch all informations using a dedicated parking filter on a osm node
    void buildFromDedicatedParkingFilter(osmDedicatedParkingFilter_t &filter, osmpbf::INodeStream &nodes);
    //Utility method to fetch all informations using a dedicated parking filter on a osm way
    void buildFromDedicatedParkingFilter(osmDedicatedParkingFilter_t &filter, osmpbf::IWayStream &ways);

    //Utility method to fetch all informations using a parking lane filter on a osm way
    void buildFromParkingLaneFilter(osmParkingLaneFilter_t &filter, osmpbf::IWayStream &ways);

    //Specifies if this parking solution is simply free space at the side of the road,
    //or rather a dedicated parking solution
    bool streetParking;

    //Specifies if everyone can park at this place
    bool publicParking;

    //Specifies if this a private parking place (used to indicate club or company parking areas)
    bool privateParking;

    //Specifies if customers (of a close by market/shop/club/...) may park at this spot
    bool customerParking;

    //Specifies if parking is for free at this place or if one has to pay to park at this place
    bool freeParking;

    //Specifies whether this place has a constraint parking time or not
    bool constraintTime;


};

#endif // PARKINGSOLUTION_H
