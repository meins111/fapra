#include "basicnode.h"

BasicNode & BasicNode::operator= ( const BasicNode & copy) {
    lastEdge=copy.lastEdge;
    firstEdge=copy.firstEdge;
    return *this;
}
