#ifndef EDGEINFO_H
#define EDGEINFO_H

#include <limits.h>
#include <stdint.h>

#include "roadtypes.h"
#include <stddef.h>
#include <stdint.h>
#include <vector>

class EdgeInfo
{
public:
    EdgeInfo() : allowance(0), distance(0.0), speed(0.0), isOneWay(false), isMetaEdge(false), type(INVALID) {}
    EdgeInfo(uint8_t allowance, double distance, double speed, bool oneWay, bool meta, const std::vector<size_t> subEdges, const EdgeType type) :
        allowance(allowance), distance(distance), speed(speed), isOneWay(oneWay), isMetaEdge(meta), subEdges(subEdges), type(type){}
    EdgeInfo (const EdgeInfo &copy) : allowance(copy.allowance), speed(copy.speed), distance(copy.distance), isOneWay(copy.isOneWay),
        isMetaEdge(copy.isMetaEdge), subEdges(copy.subEdges), type(copy.type){}
    EdgeInfo & operator= ( const EdgeInfo & copy);

    /* Edge Informations */
    uint8_t allowance;
    double distance;
    double speed;
    bool isOneWay;
    EdgeType type;

    /* Meta - Edge informations */
    bool isMetaEdge;
    std::vector<size_t> subEdges;

    /* Utility methods */
    //Calculates the travel time on this edge by performing the following calculation: distance/speed
    double getTravelTime();
    //Checks a given travelMedium bit mask if this edge allows this mean of travel
    bool checkAllowance (uint8_t travelMedium);

    double getEdgeCost (bool timeIsPrio);
};

#endif // EDGEINFO_H
