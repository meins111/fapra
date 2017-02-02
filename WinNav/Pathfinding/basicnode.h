#ifndef BASICNODE_H
#define BASICNODE_H

#include <stdint.h>
#include <cstddef>

class BasicNode
{
public:
    BasicNode() : firstEdge(0xFFFFFFFF), lastEdge(0xFFFFFFFF){}
    BasicNode(const BasicNode &copy): firstEdge(copy.firstEdge), lastEdge(copy.lastEdge){}
    BasicNode(size_t first, size_t last): firstEdge(first), lastEdge(last){}
    BasicNode & operator= ( const BasicNode & copy);

    /* Basic Node Connectivity: These represents offsets into an edge array that is sorted by the source node */
    size_t firstEdge;
    size_t lastEdge;

    //Utility method to fetch the number of edges of this node
    inline size_t numberOfEdges() { return lastEdge-firstEdge; }


};

#endif // BASICNODE_H
