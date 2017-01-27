#include "Route.h"


Route::Route()
{
}


Route::~Route()
{
}

Node & Route::getNode(size_t index)
{
	if (index >= routeNodes.size()) {
		throw("Index out of Bounds!\n");
	}
	return routeNodes[index];
}

Edge & Route::getEdge(size_t index)
{
	if (index >= routeEdges.size()) {
		throw("Index out of Bounds!\n");
	}
	return routeEdges[index];
}

size_t Route::numEdges()
{
	return routeEdges.size();
}
