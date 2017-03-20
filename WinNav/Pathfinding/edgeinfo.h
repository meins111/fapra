#ifndef EDGEINFO_H
#define EDGEINFO_H

#include <limits.h>
#include <stdint.h>
#include <stdio.h>

#include "roadtypes.h"
#include <stddef.h>
#include <stdint.h>
#include <vector>
#include <Utils/easylogging++.h>
#include <unordered_map>

class EdgeInfo
{
public:
    EdgeInfo() : allowance(0), speed(0.0), isOneWay(false), type(INVALID), parking(false), parkingInfoID(0xFFFFFFFF), isMetaEdge(false) {}

    EdgeInfo(uint8_t allowance, double speed, bool oneWay, bool parkingFlag, size_t parkingOffset, bool meta, const std::vector<size_t> subEdges, const EdgeType type) :
        allowance(allowance), speed(speed), isOneWay(oneWay), parking(parkingFlag), parkingInfoID(parkingOffset), isMetaEdge(meta), subEdges(subEdges), type(type){}

    EdgeInfo (const EdgeInfo &copy) : allowance(copy.allowance), speed(copy.speed), isOneWay(copy.isOneWay),
        parking(copy.parking), parkingInfoID(copy.parkingInfoID), isMetaEdge(copy.isMetaEdge), subEdges(copy.subEdges), type(copy.type){}

    EdgeInfo & operator= ( const EdgeInfo & copy);

    /* Edge Informations */
    uint8_t allowance;
    double speed;
    bool isOneWay;
    EdgeType type;
    //Indicate if public parking is possible along this route
    bool parking;
    size_t parkingInfoID;

    /* Meta - Edge informations */
    bool isMetaEdge;
    std::vector<size_t> subEdges;

    bool checkAllowance (uint8_t travelMedium);

    void print();
};

#endif // EDGEINFO_H
