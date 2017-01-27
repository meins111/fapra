/**
 * Created by cmd-keen
 * on netbook
 * at Di 27. Sep 21:50:28 CEST 2016
 */

#include "MapWidget.h"
using namespace Marble;

MapWidget::MapWidget(QWidget * widget) :
	MarbleWidget(widget)
{
	this->addLayer(&graphLayer);
}

GraphLayer & MapWidget::getGraphLayer() {
	return graphLayer;
}

