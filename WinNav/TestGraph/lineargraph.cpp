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


void LinearGraph::merge (LinearGraph &brother) {
    //Fetch the final size of the merged graph
    size_t totalNodes = nodes.size() + brother.nodes.size();
    size_t totalEdges = edges.size() + brother.edges.size();
    size_t startingNodeCnt = nodes.size();
    size_t startingBrotherNodes = brother.nodes.size();
    size_t startingBrotherEdges = brother.edges.size();
    nodes.reserve(totalNodes);
    edges.reserve(totalEdges);
    //Cut&Paste all nodes from the brother to the back of this graph (but keeping the internal order!)
    for (size_t i=0; i<startingBrotherNodes; i++) {
        nodes.emplace_back(brother.nodes.front());
        brother.nodes.erase(brother.nodes.begin());
    }
    //Cut&Paste all edges from the brother to the back of this graph
    for (size_t i=0; i<startingBrotherEdges; i++) {
        edges.emplace_back(brother.edges.front());
        //Modify that edge to accomodate to the offset
        PODEdge &curEdge = edges.back();
        //The IDs of start and end keep their relative positioning, but are now offset by the number of nodes this graph has stored before the merge
        curEdge.setStart(curEdge.getStart() + startingNodeCnt);
        curEdge.setEnd(curEdge.getEnd() + startingNodeCnt);
        brother.edges.erase(brother.edges.begin());
    }
}

void LinearGraph::insertNodes(const std::vector<PODNode> &nodeVect) {
    for (size_t i=0; i<nodeVect.size(); i++) {
        nodes.emplace_back(nodeVect[i]);
    }
}

void LinearGraph::insertEdges(const std::vector<PODEdge> &edgeVect) {
    for (size_t i=0; i<edgeVect.size(); i++) {
        edges.emplace_back(edgeVect[i]);
    }
}
 void LinearGraph::setEdgeType(edgeType_t type) {
     for (size_t i=0; i<edges.size(); i++) {
         edges[i].setType(type);
     }
 }
