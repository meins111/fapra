#pragma once

class Node
{
public:
	Node();
	virtual ~Node();
	virtual double getLongitude() = 0;
	virtual double getLatitude() = 0;
};

