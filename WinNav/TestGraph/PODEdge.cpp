#include "PODEdge.h"

PODEdge::~PODEdge() {
}

std::size_t PODEdge::getStart() {
	return start;
}

std::size_t PODEdge::getEnd() {
	return end;
}

void PODEdge::setStart(std::size_t start) {
	this->start = start;
}

void PODEdge::setEnd(std::size_t end) {
	this->end = end;
}

PODEdge & PODEdge::operator= (const PODEdge &copy) {
    start=copy.start;
    end=copy.end;
    type=copy.type;
    return *this;
}
