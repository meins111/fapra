#include "lineargraph.h"

LinearGraph::LinearGraph()
{

}

LinearGraph::LinearGraph (LinearGraph &copy) {
    //Copy over all nodes in order
    for (size_t i=0; i<copy.numNodes(); i++) {
        nodes.emplace_back(copy.getNode(i));
    }
    //Copy over all edges
    for (size_t i=0; i<copy.numEdges(); i++) {
        edges.emplace_back(copy.getEdge(i));
    }

}

LinearGraph & LinearGraph::operator= (LinearGraph &copy) {
    nodes.clear();
    edges.clear();
    //Copy over all nodes in order
    for (size_t i=0; i<copy.numNodes(); i++) {
        nodes.emplace_back(copy.getNode(i));
    }
    //Copy over all edges
    for (size_t i=0; i<copy.numEdges(); i++) {
        edges.emplace_back(copy.getEdge(i));
    }
    return *this;
}


PODNode &LinearGraph::getNode(size_t index) {
    if (index < nodes.size()) {
        return nodes[index];
    }
    else {
        throw (std::invalid_argument("Index out of bounds!"));
    }

}

PODEdge &LinearGraph::getEdge(size_t index) {
    if (index < edges.size()) {
        return edges[index];
    }
    else {
        throw (std::invalid_argument("Index out of bounds!"));
    }
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
