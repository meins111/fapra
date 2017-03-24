#include "basicedge.h"

size_t BasicEdge::getStart() {
    return this->startNode;
}

size_t BasicEdge::getEnd() {
    return this->endNode;
}
BasicEdge & BasicEdge::operator= ( const BasicEdge & copy) {
    startNode=copy.startNode;
    endNode=copy.endNode;
    index=copy.index;
    edgeInfoId=copy.edgeInfoId;
    type=copy.type;
    return *this;
}

bool BasicEdge::operator< (const BasicEdge &otherOne) {
    if (otherOne.startNode == startNode) {
        //If start nodes are equal, sort by last node
        if (otherOne.endNode < startNode) {
            return false;
        }
        else {
            return true;
        }
    }
    else if (otherOne.startNode < startNode) {
        //This start node is larger
        return false;
    }
    else {
        //This start node is smaller or equal
        return true;
    }
}
