#ifndef OSMFILTERCONTAINER
#define OSMFILTERCONTAINER
//osmpbf includes
#include <osmpbf/parsehelpers.h>
#include <osmpbf/filter.h>
#include <osmpbf/iway.h>
#include <osmpbf/inode.h>


//Container for osm routing filters for maxspeed, allowance (Car, Bike, Foot)
typedef struct osmRoutingFilter{
    //Conctructor
    osmRoutingFilter () :
        highwayFilter("highway"),
        speedFilter ("maxspeed"),
        onewayFilter ({ new osmpbf::KeyValueTagFilter("oneway", "yes"),
                        new osmpbf::KeyMultiValueTagFilter("highway",{ "motorway", "motorway_link", "invalid" }),
                        new osmpbf::KeyValueTagFilter("junction", "roundabout")}),
        footDenied ({   new osmpbf::KeyValueTagFilter("foot", "no"),
                        new osmpbf::KeyMultiValueTagFilter("access", {"no", "private"}),
                        new osmpbf::KeyMultiValueTagFilter("highway", {"motorroad", "trunk", "primary", "motorroad_link", "trunk_link", "primary_link", "invalid"})}),
        bikeDenied ({   new osmpbf::KeyValueTagFilter("bicycle", "no"),
                        new osmpbf::KeyMultiValueTagFilter("access", {"no", "private"}),
                        new osmpbf::KeyMultiValueTagFilter("highway", {"motorroad", "trunk", "primary", "motorroad_link", "trunk_link", "primary_link", "invalid"})}),
        carDenied ({    new osmpbf::KeyMultiValueTagFilter ("motorcar", {"no", "private", "agricultural", "forestry", "delivery"}),
                        new osmpbf::KeyMultiValueTagFilter("access", {"no", "private", "agricultural", "forestry", "delivery"}),
                        new osmpbf::KeyMultiValueTagFilter("highway", {"footway", "steps", "path", "cycleway", "platform", "track", "bridleway", "pedestrian", "invalid"})})

        {}
    //Assign the given primitive input block adapter to all mmanaged filters
    void assignInputAdapter (const osmpbf::PrimitiveBlockInputAdaptor *pbi) {
        highwayFilter.assignInputAdaptor(pbi);
        speedFilter.assignInputAdaptor(pbi);
        onewayFilter.assignInputAdaptor(pbi);
        footDenied.assignInputAdaptor(pbi);
        bikeDenied.assignInputAdaptor(pbi);
        carDenied.assignInputAdaptor(pbi);
    }
    //Refreshes all filters - Make sure to call this method after the pbi changed (e.g. after a pbi.next() call)
    void refreshAllFilter() {
        highwayFilter.rebuildCache();
        speedFilter.rebuildCache();
        onewayFilter.rebuildCache();
        footDenied.rebuildCache();
        bikeDenied.rebuildCache();
        carDenied.rebuildCache();
    }

    //Key Only Tag filter are able to dismiss any entry that does not match the given Tag, e.g. "highway" for streets
    osmpbf::KeyOnlyTagFilter highwayFilter;
    //Speed filter allows the extraction of the allowed max speed on that edge
    osmpbf::KeyOnlyTagFilter speedFilter;
    //Check if the route is a oneway: this is it is marked as one, or it's type is implicitly oneway such as motorways
    osmpbf::OrTagFilter onewayFilter;
    //Check if foot-access is forbidden or it's type implicitly denies foot-access such as motorways
    osmpbf::OrTagFilter footDenied;
    //Check if bike-access is forbidden or it's street type implicitly denies bike-access such as motorways
    osmpbf::OrTagFilter bikeDenied;
    //Check if car-access is forbidden or it's type implicitly denies car access such as footways
    osmpbf::OrTagFilter carDenied;

} osmRoutingFilter_t;

//Container for osm parking lane filter
typedef struct osmParkingLaneFilter{
    osmParkingLaneFilter ():
        parkingLaneFilter   ({  new osmpbf::KeyMultiValueTagFilter ("parking:lane:left", {"parallel", "diagonal", "perpendicular", "marked"}),
                                new osmpbf::KeyMultiValueTagFilter ("parking:lane:right", {"parallel", "diagonal", "perpendicular", "marked"}),
                                new osmpbf::KeyMultiValueTagFilter ("parking:lane:both", {"parallel", "diagonal", "perpendicular", "marked"})}),
        publicParkingDeniedFilter ({
                                new osmpbf::KeyMultiValueTagFilter ("parking:condition:left", {"residents", "private", "customers"}),
                                new osmpbf::KeyMultiValueTagFilter ("parking:condition:right", {"residents", "private", "customers"}),
                                new osmpbf::KeyMultiValueTagFilter ("parking:condition:both", {"residents", "private", "customers"}),
                                new osmpbf::KeyMultiValueTagFilter ("access", {"no", "private"})}),
        customerParkingFilter ({
                                new osmpbf::KeyValueTagFilter("parking:condition:left", "customer"),
                                new osmpbf::KeyValueTagFilter("parking:condition:right", "customer"),
                                new osmpbf::KeyValueTagFilter("parking:condition:both", "customer")}),
        privateParkingFilter ({
                                new osmpbf::KeyValueTagFilter("parking:condition:left", "private"),
                                new osmpbf::KeyValueTagFilter("parking:condition:left", "private"),
                                new osmpbf::KeyValueTagFilter("parking:condition:left", "private")}),
        freeParkingFiler ({
                          new osmpbf::KeyValueTagFilter("parking:condition:left", "free"),
                          new osmpbf::KeyValueTagFilter("parking:condition:right", "free"),
                          new osmpbf::KeyValueTagFilter("parking:condition:both", "free")}),
        timeLimitParkingFilter({
                               new osmpbf::KeyValueTagFilter("parking:condition:left", "disk"),
                               new osmpbf::KeyValueTagFilter("parking:condition:right", "disk"),
                               new osmpbf::KeyValueTagFilter("parking:condition:both", "disk"),
                               new osmpbf::KeyOnlyTagFilter("maxstay")})
        {}

    //Assign the given primitive input block adapter to all mmanaged filters
    void assignInputAdapter (const osmpbf::PrimitiveBlockInputAdaptor *pbi) {
        parkingLaneFilter.assignInputAdaptor(pbi);
        publicParkingDeniedFilter.assignInputAdaptor(pbi);
        customerParkingFilter.assignInputAdaptor(pbi);
        privateParkingFilter.assignInputAdaptor(pbi);
        freeParkingFiler.assignInputAdaptor(pbi);
        timeLimitParkingFilter.assignInputAdaptor(pbi);
    }
    //Refreshes all filters - Make sure to call this method after the pbi changed (e.g. after a pbi.next() call)
    void refreshAllFilter() {
        parkingLaneFilter.rebuildCache();
        publicParkingDeniedFilter.rebuildCache();
        customerParkingFilter.rebuildCache();
        privateParkingFilter.rebuildCache();
        freeParkingFiler.rebuildCache();
        timeLimitParkingFilter.rebuildCache();
    }

    //Fetch parking areas that are associated with a highway (so parking at the side of a road)
    osmpbf::OrTagFilter parkingLaneFilter;
    //Filter to check if public parking is allowed on the park lane or not
    osmpbf::OrTagFilter publicParkingDeniedFilter;
    //Filter to check if customer parking is allowed on this park lane
    osmpbf::OrTagFilter customerParkingFilter;
    //Filter to check if private parking is allowed on this lane
    osmpbf::OrTagFilter privateParkingFilter;
    //Filter to check whether the parking is free or if one has to buy a parking ticket
    osmpbf::OrTagFilter freeParkingFiler;
    //Filter to check if it is a time-limited parking spot
    osmpbf::OrTagFilter timeLimitParkingFilter;

} osmParkingLaneFilter_t;

//Container parking filters for area, entrance, public access, cost free, parking lanes etc
typedef struct osmDedicatedParkingFilter{
    osmDedicatedParkingFilter () :
                parkingAreaFilter ("amenity", {"parking", "parking_entrance", "parking_space"}),
                publicParkingDeniedFilter ("access", {"residents", "private", "customers"}),
                customerParkingFilter ("access", "customer "),
                privateParkingFilter ("access", "private"),
                parkingFeeFiler ("fee", {"yes", "interval"}),
                timeLimitParkingFilter ("maxstay")
        {}
    //Assign the given primitive input block adapter to all mmanaged filters
    void assignInputAdapter (const osmpbf::PrimitiveBlockInputAdaptor *pbi){
        parkingAreaFilter.assignInputAdaptor(pbi);
        publicParkingDeniedFilter.assignInputAdaptor(pbi);
        customerParkingFilter.assignInputAdaptor(pbi);
        privateParkingFilter.assignInputAdaptor(pbi);
        parkingFeeFiler.assignInputAdaptor(pbi);
        timeLimitParkingFilter.assignInputAdaptor(pbi);
    }

    //Refreshes all filters - Make sure to call this method after the pbi changed (e.g. after a pbi.next() call)
    void refreshAllFilter(){
        parkingAreaFilter.rebuildCache();
        publicParkingDeniedFilter.rebuildCache();
        customerParkingFilter.rebuildCache();
        privateParkingFilter.rebuildCache();
        parkingAreaFilter.rebuildCache();
        timeLimitParkingFilter.rebuildCache();
    }

    //Fetch parking areas that are not associated with a highway (e.g. a underground parking place, or a dedicated parking area)
    osmpbf::KeyMultiValueTagFilter parkingAreaFilter;
    //Filter to check if public parking is allowed on the park lane or not
    osmpbf::KeyMultiValueTagFilter publicParkingDeniedFilter;
    //Filter to check if customer parking is allowed on this park lane
    osmpbf::KeyValueTagFilter customerParkingFilter;
    //Filter to check if private parking is allowed on this lane
    osmpbf::KeyValueTagFilter privateParkingFilter;
    //Filter to check whether one has to pay a fee to park at this parking space or not
    osmpbf::KeyMultiValueTagFilter parkingFeeFiler;
    //Filter to check if it is a time-limited parking spot
    osmpbf::KeyOnlyTagFilter timeLimitParkingFilter;
} osmDedicatedParkingFilter_t;



#endif // OSMFILTERCONTAINER

