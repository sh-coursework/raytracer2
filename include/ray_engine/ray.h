//
// Created by Steve Hwan on 10/1/18.
//

#ifndef RAYTRACER1_RAY_H
#define RAYTRACER1_RAY_H
#include "vec3.h"


class Ray {
    public:
        Ray() = default;
        Ray(const Vec3 &a, const Vec3 &b, float ti) { A = a; B = b; _time = ti;}
        Vec3 origin() const { return A; }
        Vec3 direction() const { return B; }
        float time() const { return _time; }
        Vec3 point_at_parameter(float t) const { return  A + t * B; }

        Vec3 A;
        Vec3 B;
        float _time;
};

Vec3 random_in_unit_sphere();

#endif //RAYTRACER1_RAY_H
