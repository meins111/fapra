#ifndef BASICEDGE_H
#define BASICEDGE_H

#include "GraphInterface/Graph.h"

class BasicEdge : public Edge
{
public:
    BasicEdge() : startNode(0xFFFFFFFF), endNode(0xFFFFFFFF), index(0xFFFFFFFF), edgeInfoId(0xFFFFFFFF){}
    BasicEdge(const BasicEdge &copy): startNode(copy.startNode), endNode(copy.endNode), index(copy.index), edgeInfoId(copy.edgeInfoId){}
    BasicEdge(size_t start, size_t end, size_t index, size_t infoID): startNode(start), endNode(end), index(index), edgeInfoId(infoID){}
    BasicEdge & operator= ( const BasicEdge & copy);
    bool operator< (const BasicEdge &otherOne);

    /* Edge Interface */
    size_t getStart();
    size_t getEnd();

    /* Offset values of start and end node of this edge in the local offset array representation */
    size_t startNode;
    size_t endNode;

    //Own index informations - can be useful in a number of ways
    size_t index;

    //Edge Info identifier: used to lookup additional edge informations in an external ressource
    size_t edgeInfoId;
};

#endif // BASICEDGE_H
