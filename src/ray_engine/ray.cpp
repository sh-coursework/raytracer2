//
// Created by Steve Hwan on 10/1/18.
//

#include "ray_engine/ray.h"


Vec3 random_in_unit_sphere() {
    Vec3 p;
    do {
        p = 2.0f * Vec3(drand48(), drand48(), drand48()) - Vec3(1.0f, 1.0f, 1.0f);
    } while (p.squared_length() >= 1.0);
    return p;
}

