//
// Created by Steve Hwan on 10/1/18.
//

#ifndef RAYTRACER1_SPHERE_H
#define RAYTRACER1_SPHERE_H

#include "vec3.h"
#include "ray_engine/hitable.h"
#include "ray_engine/aabb.h"
#include "materials/material.h"

class Sphere : public Hitable {
public:
    Sphere() = default;
    Sphere(Vec3 cen, float r, Material *m) : center(cen), radius(r),
                                             mat_ptr(m) {};
    bool hit(const Ray& r, float tmin, float tmax,
             HitRecord& rec) const override;
    bool bounding_box(float t0, float t1, AABB& box) const override;
    Vec3 center;
    float radius;
    Material *mat_ptr;  // I think I actually own the material ptr,
                        // but I don't really want to.
};


void GetSphereUV(const Vec3 &p, float& u, float &v);


#endif //RAYTRACER1_SPHERE_H
