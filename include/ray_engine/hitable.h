//
// Created by Steve Hwan on 10/1/18.
//

#ifndef RAYTRACER1_HITABLE_H
#define RAYTRACER1_HITABLE_H

#include "vec3.h"
#include "ray_engine/aabb.h"
#include "ray_engine/ray.h"

class Material;  // avoid circular Hitable / material

struct HitRecord {
    float t;
    Vec3 p;
    Vec3 normal;
    float u, v;
    Material *mat_ptr;
};

class Hitable {
public:
    virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const = 0;
    virtual bool bounding_box(float t0, float t1, AABB& box) const = 0;

};


#endif //RAYTRACER1_HITABLE_H
