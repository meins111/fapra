#ifndef _PODNODE_H_
#define _PODNODE_H_

#include "../GraphInterface/Node.h"

/**
 * @brief 
 */
class PODNode : public Node {
	double longitude;
	double latitude;
public:
    PODNode () : longitude(0.0), latitude(0.0){}
    PODNode (const PODNode &copy) : latitude(copy.latitude), longitude(copy.longitude){}
    PODNode(const double &lon, const double &lat) : longitude(lon), latitude(lat){}
    PODNode & operator= (const PODNode &copy);

	void set(double lon, double lat);
	void setLatitude(double lat);
	void setLongitude(double lon);
	virtual double getLongitude();
	virtual double getLatitude();
};

#endif
