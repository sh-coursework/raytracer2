//
// Created by Steve Hwan on 10/9/18.
//


#include "ray_engine/aabb.h"

#include <algorithm>

// version 1 from book
//bool AABB::Hit(const Ray& r, float tmin, float tmax) const {
//    for (auto a: boost::irange(0, 3)) {
//        float t0 = ffmin((min_[a] - r.origin()[a]) / r.direction()[a],
//                         (max_[a] - r.origin()[a]) / r.direction()[a]);
//        float t1 = ffmax((min_[a] - r.origin()[a]) / r.direction()[a],
//                         (max_[a] - r.origin()[a]) / r.direction()[a]);
//        tmin = ffmax(t0, tmin);
//        tmax = ffmin(t1, tmax);
//        if (tmax <= tmin)
//            return false;
//    }
//    return true;
//}


AABB surrounding_box(AABB box0, AABB box1) {
    Vec3 small( std::min(box0.min().x(), box1.min().x()),
                std::min(box0.min().y(), box1.min().y()),
                std::min(box0.min().z(), box1.min().z()));
    Vec3 big  ( std::max(box0.max().x(), box1.max().x()),
                std::max(box0.max().y(), box1.max().y()),
                std::max(box0.max().z(), box1.max().z()));
    return {small, big};
}
