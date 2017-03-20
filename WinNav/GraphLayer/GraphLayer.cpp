/**
 * Created by cmd-keen
 * on netbook
 * at Di 27. Sep 21:31:01 CEST 2016
 */

#include "GraphLayer.h"
#include <marble/GeoDataLineString.h>
#include <iostream>
using namespace std;
using namespace Marble;

GraphLayer::GraphLayer() :
	graph(NULL)
{
}

GraphLayer::~GraphLayer() {
}

bool GraphLayer::render(GeoPainter * painter,
		ViewportParams * viewport, const QString & renderPos,
		GeoSceneLayer * layer) {

	Q_UNUSED( viewport )
	Q_UNUSED( renderPos )
	Q_UNUSED( layer )

	painter->save();

	paintGraph(painter);

	paintNotification(painter, start);
	paintNotification(painter, end);

	painter->restore();
	return true;
}

QStringList GraphLayer::renderPosition() const {
	return QStringList(QStringLiteral("HOVERS_ABOVE_SURFACE"));
}

void GraphLayer::paintNotification(GeoPainter *painter, Notification &note) {

	if( !note.wasSet )
		return;

	painter->setPen( Qt::black );
	painter->setBrush( Oxygen::raspberryPink1 );
	painter->drawEllipse(note.coordinates, 12, 12, false);
}

void GraphLayer::paintNode(GeoPainter * painter, Node &node) {

	GeoDataCoordinates coord(node.getLongitude(), node.getLatitude(), 0, GeoDataCoordinates::Degree);
	painter->setPen( Qt::black );
	painter->setBrush( Oxygen::hotOrange1 );
    painter->drawEllipse(coord, 6, 6, false);
}

void GraphLayer::paintParkingSpot(Marble::GeoPainter *painter, Node &node) {
    GeoDataCoordinates coord(node.getLongitude(), node.getLatitude(), 0, GeoDataCoordinates::Degree);
    painter->setPen( Qt::black );
    painter->setBrush( Oxygen::seaBlue1 );
    painter->drawEllipse(coord, 12, 12, false);
}

void GraphLayer::paintWalkingNode(Marble::GeoPainter *painter, Node &node) {
    GeoDataCoordinates coord(node.getLongitude(), node.getLatitude(), 0, GeoDataCoordinates::Degree);
    painter->setPen( Qt::black );
    painter->setBrush( Oxygen::emeraldGreen1 );
    painter->drawEllipse(coord, 8, 8, false);
}

void GraphLayer::paintEdge(GeoPainter * painter, Node & start, Node & end) {
	GeoDataCoordinates startCoord(start.getLongitude(), start.getLatitude(), 0, GeoDataCoordinates::Degree);
	GeoDataCoordinates endCoord(end.getLongitude(), end.getLatitude(), 0, GeoDataCoordinates::Degree);
	GeoDataLineString lineString;
	lineString << startCoord << endCoord;
	painter->drawPolyline(lineString);
}

void GraphLayer::setStart(qreal lon, qreal lat) {
	start.wasSet = true;
	start.coordinates.set(lon, lat, 0, GeoDataCoordinates::Degree);
}

void GraphLayer::setEnd(qreal lon, qreal lat) {
	end.wasSet = true;
	end.coordinates.set(lon, lat, 0, GeoDataCoordinates::Degree);
}

GraphLayer::Notification::Notification():
	wasSet(false)
{
}

void GraphLayer::paintGraph(Marble::GeoPainter * painter) {

	if(graph == NULL)
		return;

	size_t edges = graph->numEdges();
	for(size_t i=0 ; i < edges ; ++i) {
		Edge &edge = graph->getEdge(i);
		Node &start = graph->getNode( edge.getStart() );
		Node &end = graph->getNode( edge.getEnd() );

		paintNode( painter, start);
		paintNode( painter, end);
		paintEdge( painter, start, end);

	}
}

void GraphLayer::paintCarDriveGraph(Marble::GeoPainter *painter, bool parkingAtTarget) {
    paintGraph(painter);
    if (parkingAtTarget) {
        //We park at the target, so the last node of the graph shall be depicted as a parking node
        size_t edgeCnt = graph->numEdges();
        Edge &lastEdge = graph->getEdge( edgeCnt - 1);
        Node &end = graph->getNode( lastEdge.getEnd() );
        paintParkingSpot(painter, end);
    }
}

void GraphLayer::paintFootwalk(Marble::GeoPainter *painter) {
    if(graph == NULL)
        return;

    size_t edges = graph->numEdges();
    for(size_t i=0 ; i < edges ; ++i) {
        Edge &edge = graph->getEdge(i);
        Node &start = graph->getNode( edge.getStart() );
        Node &end = graph->getNode( edge.getEnd() );

        paintWalkingNode( painter, start);
        paintWalkingNode( painter, end);
        paintEdge( painter, start, end);

    }
}

void GraphLayer::setGraph(Graph &graph) {
	this->graph = &graph;
}

