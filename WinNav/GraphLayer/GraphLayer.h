#ifndef _GRAPHLAYER_H_
#define _GRAPHLAYER_H_

#include "../GraphInterface/Graph.h"
#include <marble/LayerInterface.h>
#include <marble/GeoPainter.h>
#include <marble/GeoDataCoordinates.h>

/**
 * @brief 
 */
class GraphLayer : public Marble::LayerInterface {
	Graph *graph;

	struct Notification {
		bool wasSet;
		Marble::GeoDataCoordinates coordinates;

		Notification();
	};

	Notification start, end;
protected:
	void paintGraph(Marble::GeoPainter *painter);
    void paintCarDriveGraph(Marble::GeoPainter *painter, bool parkingAtTarget);
    void paintFootwalk(Marble::GeoPainter *painter);
	void paintNotification(Marble::GeoPainter *painter, Notification &note);
	void paintNode(Marble::GeoPainter *painter, Node &node);
    void paintParkingSpot(Marble::GeoPainter *painter, Node &node);
	void paintEdge(Marble::GeoPainter *painter, Node &start, Node &end);
    void paintWalkingNode(Marble::GeoPainter *painter, Node &node);
public:
	GraphLayer();
	virtual ~GraphLayer();
	virtual bool 	render (Marble::GeoPainter *painter, Marble::ViewportParams *viewport, const QString &renderPos, Marble::GeoSceneLayer *layer);
	virtual QStringList renderPosition() const;
	void setStart(qreal lon, qreal lat);
	void setEnd(qreal lon, qreal lat);
	void setGraph(Graph &graph);
};

#endif
