//
// Created by Steve Hwan on 10/9/18.
//


#include "ray_engine/aabb.h"

#include <algorithm>

// version 1 from book
//bool AABB::hit(const Ray& r, float tmin, float tmax) const {
//    for (auto a: boost::irange(0, 3)) {
//        float t0 = ffmin((_min[a] - r.origin()[a]) / r.direction()[a],
//                         (_max[a] - r.origin()[a]) / r.direction()[a]);
//        float t1 = ffmax((_min[a] - r.origin()[a]) / r.direction()[a],
//                         (_max[a] - r.origin()[a]) / r.direction()[a]);
//        tmin = ffmax(t0, tmin);
//        tmax = ffmin(t1, tmax);
//        if (tmax <= tmin)
//            return false;
//    }
//    return true;
//}


AABB surrounding_box(AABB box0, AABB box1) {
    Vec3 small( ffmin(box0.min().x(), box1.min().x()),
                ffmin(box0.min().y(), box1.min().y()),
                ffmin(box0.min().z(), box1.min().z()));
    Vec3 big  ( ffmax(box0.max().x(), box1.max().x()),
                ffmax(box0.max().y(), box1.max().y()),
                ffmax(box0.max().z(), box1.max().z()));
    return {small, big};
}
