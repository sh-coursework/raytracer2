//
// Created by Steve Hwan on 10/1/18.
//

#ifndef RAYTRACER1_SPHERE_H
#define RAYTRACER1_SPHERE_H

#include "hitable.h"

class sphere : public hitable {
public:
    sphere() {}
    sphere(vec3 cen, float r, material *m) : center(cen), radius(r), mat_ptr(m) {};
    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
    virtual bool bounding_box(float t0, float t1, aabb& box) const;
    vec3 center;
    float radius;
    material *mat_ptr;  // I think I actually own the material ptr, but I don't really want to.
};



#endif //RAYTRACER1_SPHERE_H
