//
// Created by Steve Hwan on 10/1/18.
//

#ifndef RAYTRACER1_RAY_H
#define RAYTRACER1_RAY_H
#include "vec3.h"


class ray {
    public:
        ray() {}
        ray(const vec3& a, const vec3& b) { A = a; B = b; }
        vec3 origin() const { return A; }
        vec3 direction() const { return B; }
        vec3 point_at_parameter(float t) const { return  A + t*B; }

        vec3 A;
        vec3 B;
};

vec3 random_in_unit_sphere();

#endif //RAYTRACER1_RAY_H
