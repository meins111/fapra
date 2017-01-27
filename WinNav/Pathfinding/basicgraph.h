#ifndef BASICGRAPH_H
#define BASICGRAPH_H
#include <stdint.h>
#include <vector>
#include <stdexcept>
#include "basicedge.h"
#include "basicnode.h"

class BasicGraph
{
public:
    BasicGraph();
    std::vector<BasicNode> nodes;
    std::vector<BasicEdge> edges;

    const BasicNode & getTargetOfEdge(size_t index, size_t edgeNum);
};

#endif // BASICGRAPH_H
