#include "lineargraph.h"

LinearGraph::LinearGraph()
{

}


PODNode &LinearGraph::getNode(size_t index) {
    if (index>0 && index < nodes.size()) {
        return nodes[index];
    }
    else {
        throw (std::invalid_argument("Index out of bounds!"));
    }

}

PODEdge &LinearGraph::getEdge(size_t index) {
    if (index>0 && index < edges.size()) {
        return edges[index];
    }
    else {
        throw (std::invalid_argument("Index out of bounds!"));
    }
}

size_t LinearGraph::numEdges() {
    return edges.size();
}

void LinearGraph::insertNode(const PODNode &node) {
    nodes.push_back(node);
}

void LinearGraph::insertEdge(const PODEdge &edge){
    edges.push_back(edge);
}

void LinearGraph::reset() {
    edges.clear();
    nodes.clear();
}
