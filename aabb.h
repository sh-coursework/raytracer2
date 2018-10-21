//
// Created by Steve Hwan on 10/9/18.
//

#ifndef RAYTRACER2_AABB_H
#define RAYTRACER2_AABB_H

#include "boost/range/irange.hpp"

#include "vec3.h"
#include "ray.h"

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

class aabb {
public:
    aabb() {}
    aabb(const vec3& a, const vec3&b) { _min = a; _max = b; }

    vec3 min() const {return _min; }
    vec3 max() const {return _max; }

    inline bool hit(const ray& r, float tmin, float tmax) const {
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

    vec3 _min;
    vec3 _max;
};

aabb surrounding_box(aabb box0, aabb box1);

#endif //RAYTRACER2_AABB_H
