#include "basicgraph.h"

BasicGraph::BasicGraph()
{

}

/*** Graph Layer Interface, CURRENTLY DEPRECIATED ***/
/*BasicNode & BasicGraph::getNode(const size_t index) {
    if (index > nodes.size()) {
        throw(std::invalid_argument("Index out of bounds!"));
    }
    return nodes[index];
}
BasicEdge & BasicGraph::getEdge(const size_t index) {
    if (index > nodes.size()) {
        throw(std::invalid_argument("Index out of bounds!"));
    }
    return edges[index];
}
size_t BasicGraph::numEdges(){
    return edges.size();
} */


const BasicNode & BasicGraph::getTargetOfEdge(size_t index, size_t edgeNum) {
    //The Node we are searching is the endNode of the edgeNum-th adjacent edge of the given node
    size_t edgeOffset = nodes[index].firstEdge+edgeNum;
    if(edgeOffset> edges.size() || edgeOffset > nodes[index].lastEdge) {
        //Index out of bounds (ether array or adjacent edges)
        throw(std::invalid_argument("Index out of bounds!"));
    }
    else {
        //Return the node this edge is pointing at
        return nodes[edges[edgeOffset].endNode];
    }
}
