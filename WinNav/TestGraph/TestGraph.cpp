#include "TestGraph.h"

TestGraph::TestGraph() {
	nodes[0].set(9.2046, 48.8406);
	nodes[1].set(9.47369, 48.751);
	nodes[2].set(10.1001, 48.8341);
	nodes[3].set(10.246, 48.9555);

	edges[0].setStart(0);
	edges[0].setEnd(1);

	edges[1].setStart(1);
	edges[1].setEnd(2);

	edges[2].setStart(2);
	edges[2].setEnd(3);
}

TestGraph::~TestGraph() {
}

Node &TestGraph::getNode(size_t index) {
	return nodes[index];
}

Edge & TestGraph::getEdge(size_t index) {
	return edges[index];
}

size_t TestGraph::numEdges() {
	return 3;
}

