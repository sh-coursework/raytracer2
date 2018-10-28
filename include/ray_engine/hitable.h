//
// Created by Steve Hwan on 10/1/18.
//

#ifndef RAYTRACER1_HITABLE_H
#define RAYTRACER1_HITABLE_H

#include "vec3.h"
#include "ray_engine/aabb.h"
#include "ray_engine/ray.h"

class material;  // avoid circular hitable / material

struct hit_record {
    float t;
    vec3 p;
    vec3 normal;
    float u, v;
    material *mat_ptr;
};

class hitable {
public:
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
    virtual bool bounding_box(float t0, float t1, aabb& box) const = 0;

};


#endif //RAYTRACER1_HITABLE_H
