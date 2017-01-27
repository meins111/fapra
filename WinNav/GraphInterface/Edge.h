#pragma once

#include <cstddef>

class Edge
{
public:
	Edge();
	virtual ~Edge();
	virtual std::size_t getStart() = 0;
	virtual std::size_t getEnd() = 0;
};

