#ifndef _PODEDGE_H_
#define _PODEDGE_H_

#include "../GraphInterface/Edge.h"

/**
 * @brief 
 */
class PODEdge : public Edge {
	std::size_t start;
	std::size_t end;
public:
	virtual ~PODEdge();
    PODEdge () : start(0xFFFFFFFF), end (0xFFFFFFFF){}
    PODEdge (const PODEdge &copy) : start(copy.start), end(copy.end){}
    PODEdge(size_t start, size_t end) : start(start), end(end){}
    PODEdge & operator= (const PODEdge &copy);
	void setStart(std::size_t start);
	void setEnd(std::size_t end);
	virtual std::size_t getStart();
	virtual std::size_t getEnd();
};

#endif
