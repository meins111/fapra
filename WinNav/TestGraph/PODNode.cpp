#include "PODNode.h"

double PODNode::getLongitude() {
	return longitude;
}

double PODNode::getLatitude() {
	return latitude;
}

void PODNode::set(double lon, double lat) {
	setLatitude(lat);
	setLongitude(lon);
}

void PODNode::setLatitude(double lat) {
	this->latitude = lat;
}

void PODNode::setLongitude(double lon) {
	this->longitude = lon;
}

PODNode & PODNode::operator= (const PODNode &copy) {
    longitude=copy.longitude;
    latitude=copy.latitude;
    return *this;
}
