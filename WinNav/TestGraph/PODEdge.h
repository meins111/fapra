#ifndef _PODEDGE_H_
#define _PODEDGE_H_

#include "../GraphInterface/Edge.h"

/**
 * @brief 
 */
class PODEdge : public Edge {
	std::size_t start;
    std::size_t end;
    edgeType_t type;
public:
	virtual ~PODEdge();
    PODEdge () : start(0xFFFFFFFF), end (0xFFFFFFFF), type(UNDEFINED){}
    PODEdge (const PODEdge &copy) : start(copy.start), end(copy.end), type(copy.type){}
    PODEdge(size_t start, size_t end) : start(start), end(end), type(UNDEFINED){}
    PODEdge(size_t start, size_t end, edgeType_t eType) : start(start), end(end), type(eType){}
    PODEdge & operator= (const PODEdge &copy);
	void setStart(std::size_t start);
	void setEnd(std::size_t end);
    void setType (edgeType_t eType) {type=eType;}
	virtual std::size_t getStart();
	virtual std::size_t getEnd();

    edgeType_t getEdgeType() {return type;}
};

#endif
