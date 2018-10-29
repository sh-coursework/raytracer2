//
// Created by Steve Hwan on 10/9/18.
//

#ifndef RAYTRACER2_AABB_H
#define RAYTRACER2_AABB_H

#include "boost/range/irange.hpp"

#include "vec3.h"
#include "ray_engine/ray.h"

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

class AABB {
public:
    AABB() = default;
    AABB(const Vec3& a, const Vec3&b) { _min = a; _max = b; }

    Vec3 min() const {return _min; }
    Vec3 max() const {return _max; }

    inline bool hit(const Ray& r, float tmin, float tmax) const {
        for (auto a: boost::irange(0, 3)) {
            auto invD = 1.0f / r.direction()[a];
            auto t0 = (min()[a] - r.origin()[a]) * invD;
            auto t1 = (max()[a] - r.origin()[a]) * invD;
            if (invD < 0.0f)
                std::swap(t0, t1);
            tmin = t0 > tmin ? t0 : tmin;
            tmax = t1 < tmax ? t1 : tmax;
            if (tmax <= tmin)
                return false;
        }
        return true;
    }

    Vec3 _min;
    Vec3 _max;
};

AABB surrounding_box(AABB box0, AABB box1);

#endif //RAYTRACER2_AABB_H
