#ifndef CLOSENESSTREE
#define CLOSENESSTREE
#include <nanoflann.hpp>
#include <vector>
#include "nodeinfo.h"

struct NodeCloud {
    std::vector<NodeInfo> nodeData;

    ///Nanoflann custom class interface methods:
    // Must return the number of data points
    inline size_t kdtree_get_point_count() const { return nodeData.size(); }

    // Returns the distance between the vector "p1[0:size-1]" and the data point with index "idx_p2" stored in the class
    inline double kdtree_distance(const double *p1, const size_t idx_p2,size_t /*size*/) const
    {
        //Use the haversine distance as distance metrik
        NodeInfo start;
        start.longitude=p1[0];
        start.latitude=p1[1];
        const NodeInfo &target = nodeData[idx_p2];
        return start.getHaversineDistanceTo(target);
        /*
            const double d0=p1[0]-nodeData[idx_p2].longitude;
            const double d1=p1[1]-nodeData[idx_p2].latitude;
            ///TODO: This maybe should be the haversine distance instead for improves accurancy
            return d0*d0+d1*d1;
        */
    }

    // Returns the dim'th component of the idx'th point in the class:
    // Since this is inlined and the "dim" argument is typically an immediate value, the
    //  "if/else's" are actually solved at compile time.
    inline double kdtree_get_pt(const size_t idx, int dim) const
    {
            if (dim==0) return nodeData[idx].longitude;
            else return nodeData[idx].latitude;
    }

    // Optional bounding-box computation: return false to default to a standard bbox computation loop.
    //   Return true if the BBOX was already computed by the class and returned in "bb" so it can be avoided to redo it again.
    //   Look at bb.size() to find out the expected dimensionality (e.g. 2 or 3 for point clouds)
    template <class BBOX>
    bool kdtree_get_bbox(BBOX& /* bb */) const { return false; }

};

//nanoflann tree template
typedef nanoflann::KDTreeSingleIndexAdaptor<nanoflann::L2_Simple_Adaptor<double, NodeCloud>, NodeCloud, 2> ClosenessTree_t;


#endif // CLOSENESSTREE

