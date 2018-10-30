//
// Created by Steve Hwan on 10/1/18.
//

#ifndef RAYTRACER1_RAY_H
#define RAYTRACER1_RAY_H

#include "vec3.h"


class Ray {
    public:
        Ray() = default;
        Ray(const Vec3 &origin, const Vec3 &direction, float time)
                { origin_ = origin; direction_ = direction; time_ = time;}
        Vec3 origin() const { return origin_; }
        Vec3 direction() const { return direction_; }
        float time() const { return time_; }
        Vec3 PointAtParameter(float t) const
                { return  origin_ + t * direction_; }

        Vec3 origin_;
        Vec3 direction_;
        float time_;
};

#endif //RAYTRACER1_RAY_H
