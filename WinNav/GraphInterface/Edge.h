#pragma once

#include <cstddef>

typedef enum edgeType {UNDEFINED=0, CARDRIVE=1, FOOTWALK=2, BIKERIDE=3} edgeType_t;

class Edge
{
public:
	Edge();
	virtual ~Edge();
	virtual std::size_t getStart() = 0;
	virtual std::size_t getEnd() = 0;
    virtual edgeType_t getEdgeType() = 0;
};

