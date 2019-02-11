//
// Created by Steve Hwan on 10/9/18.
//

#ifndef RAYTRACER2_AABB_H
#define RAYTRACER2_AABB_H

#include <utility>
#include "boost/range/irange.hpp"

#include "vec3.h"
#include "ray_engine/ray.h"


// Code Inspection note:
// Below, CLion flags the std::swap as
//   Parameter type mismatch: Incompatible types 'std::tuple<_Tp...>&' and 'float'
// This is a bug in CLion.  In the youtrack.jetbrains.com database, this is
// tickets CPP-8423 - std::swap is red for builtin type if <tuple> is included.
// and CPP-9222 - False "No matching function" for std::swap.
// The code compiles and executes fine, at code inspection time, CLion is
// unable to see the utility std::swap. If <tuple> is included, it finds the
// swap in that module, also in the std:: namespace. Otherwise, it just can't
// find swap.
// One of the other users on that thread said:
// This occurred when using libstdc++ as standard C++ library,
// possibly the definiton of std::swap in libstdc++ trigger some bug of CLion.
// On Linux, currently I install clang and libc++ and make CLion use them as
// compiler and standard library to get it work.

class AABB {
public:
    AABB() = default;
    AABB(const Vec3& a, const Vec3&b) { min_ = a; max_ = b; }

    Vec3 min() const {return min_; }
    Vec3 max() const {return max_; }

    inline bool hit(const Ray& r, float t_min, float t_max) const {
        for (auto a: boost::irange(0, 3)) {
            auto invD = 1.0f / r.direction()[a];
            auto t0 = (min()[a] - r.origin()[a]) * invD;
            auto t1 = (max()[a] - r.origin()[a]) * invD;
            if (invD < 0.0f)
                std::swap(t0, t1);  // Ignore code inspection. See above.
            t_min = t0 > t_min ? t0 : t_min;
            t_max = t1 < t_max ? t1 : t_max;
            if (t_max <= t_min)
                return false;
        }
        return true;
    }

    Vec3 min_;
    Vec3 max_;
};

AABB surrounding_box(AABB box0, AABB box1);

#endif //RAYTRACER2_AABB_H
