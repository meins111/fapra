/**
 * Created by cmd-keen
 * on netbook
 * at Di 27. Sep 21:31:01 CEST 2016
 */

#include "GraphLayer.h"
#include <marble/GeoDataLineString.h>
#include <iostream>
#include <vector>

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
    painter->drawEllipse(note.coordinates, 10, 10, false);
}

void GraphLayer::paintStartNode (Marble::GeoPainter *painter, Node &node) {
    GeoDataCoordinates coord(node.getLongitude(), node.getLatitude(), 0, GeoDataCoordinates::Degree);
    painter->setPen( Qt::black );
    painter->setBrush( Oxygen::brickRed1 );
    painter->drawEllipse(coord, 10, 10, false);
}

void GraphLayer::paintTargetNode(Marble::GeoPainter *painter, Node &node) {
    GeoDataCoordinates coord(node.getLongitude(), node.getLatitude(), 0, GeoDataCoordinates::Degree);
    painter->setPen( Qt::black );
    painter->setBrush( Oxygen::emeraldGreen1 );
    painter->drawEllipse(coord, 10, 10, false);

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
    painter->drawEllipse(coord, 10, 10, false);
}

void GraphLayer::paintWalkingNode(Marble::GeoPainter *painter, Node &node) {
    GeoDataCoordinates coord(node.getLongitude(), node.getLatitude(), 0, GeoDataCoordinates::Degree);
    painter->setPen( Qt::black );
    painter->setBrush( Oxygen::emeraldGreen1 );
    painter->drawEllipse(coord, 8, 8, false);
}

void GraphLayer::paintEdge(Marble::GeoPainter *painter, std::vector<Node *>&edgesNodes, edgeType_t type) {
    GeoDataLineString lineString;
    //Iterate over all stored edge
    for (auto it=edgesNodes.begin(); it!=edgesNodes.end(); ++it) {
        Node *curNode = *it;
        if (curNode!=NULL) {
            GeoDataCoordinates coord(curNode->getLatitude(), curNode->getLongitude(), 0, GeoDataCoordinates::Degree);
            lineString << coord;
        }
    }
    switch(type) {
        case CARDRIVE: {
            QPen carPen (Qt::blue, 4, Qt::SolidLine);
            painter->setPen(carPen);
            break;
        }
        case FOOTWALK: {
            QPen footPen (Qt::green, 4, Qt::DashDotLine);
            painter->setPen(footPen);
            break;
        }
        case BIKERIDE: {
            QPen bikePen (Qt::yellow, 4, Qt::SolidLine);
            painter->setPen(bikePen);
            break;
        }
        default:
        case UNDEFINED: {
            QPen undefPen (Qt::gray, 1, Qt::SolidLine);
            painter->setPen(undefPen);
            break;
        }
    }
    painter->drawPolyline(lineString);
}


void GraphLayer::paintEdge(GeoPainter * painter, Node & start, Node & end, edgeType_t type) {
	GeoDataCoordinates startCoord(start.getLongitude(), start.getLatitude(), 0, GeoDataCoordinates::Degree);
	GeoDataCoordinates endCoord(end.getLongitude(), end.getLatitude(), 0, GeoDataCoordinates::Degree);
	GeoDataLineString lineString;
	lineString << startCoord << endCoord;
    /*
        QPen pen(Qt::black, 2, Qt::SolidLine);
        qp->setPen(pen);
*/
    switch(type) {
        case CARDRIVE: {
            QPen carPen (Qt::blue, 3, Qt::SolidLine);
            carPen.setWidth(3);
            painter->setPen(carPen);
            break;
        }
        case FOOTWALK: {
            QPen footPen (Qt::green, 3, Qt::DashDotLine);
            footPen.setWidth(3);
            painter->setPen(footPen);
            break;
        }
        case BIKERIDE: {
            QPen bikePen (Qt::yellow, 3, Qt::SolidLine);
            bikePen.setWidth(3);
            painter->setPen(bikePen);
            break;
        }
        default:
        case UNDEFINED: {
            QPen undefPen (Qt::gray, 1, Qt::SolidLine);
            painter->setPen(undefPen);
            break;
        }
    }
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
    std::vector<Node*> edgeNodes;
	size_t edges = graph->numEdges();    
    size_t nodes = graph->numNodes();
    edgeNodes.reserve(nodes);

    if (edges==0) {
        //Only print the nodes
        for (size_t i=0; i<nodes; i++) {
            Node &cur = graph->getNode(i);
            paintNode (painter, cur);
        }
        return;
    }
    //Otherwise neatly draw the edges and nodes along them - take special care for switching edge-Types
    edgeType_t prev=graph->getEdge(0).getEdgeType();
    //as those are much likely point-of-interests
    //Also draw the first and last node bigger
	for(size_t i=0 ; i < edges ; ++i) {
        Edge &edge = graph->getEdge(i);
        Node &start = graph->getNode( edge.getStart() );
        Node &end = graph->getNode( edge.getEnd() );
        edgeNodes.push_back(&start);
        if (i==0) {
            //First edge -> draw the first node bigger!
            paintStartNode(painter, start);
            //paintNode(painter, end);
            //paintEdge( painter, start, end, edge.getEdgeType());
        }
        else if (i==edges-1) {
            //last edges -> draw the last edge bigger!
            //paintNode(painter, start);
            paintTargetNode(painter, end);
            edgeNodes.push_back(&end);
            //paintEdge( painter, start, end, edge.getEdgeType());
        }
        else if (edge.getEdgeType() != prev) {
            //A switch in the edge type most likely means a PoI! Draw it bigger!
            paintParkingSpot(painter, start);
            //paintNode(painter, end);
            paintEdge(painter, edgeNodes, prev);
            edgeNodes.clear();
            edgeNodes.push_back(&start);
            //paintEdge( painter, start, end, edge.getEdgeType());
        }
        else {
            //Intermediate edge - We'll not want to paint intermediate nodes
            //paintNode(painter, start);
            //paintNode(painter, end);
            //paintEdge( painter, start, end, edge.getEdgeType());
        }
        prev=edge.getEdgeType();
	}
    paintEdge(painter, edgeNodes, prev);
}
/*
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
*/
void GraphLayer::setGraph(Graph &graph) {
	this->graph = &graph;
}

