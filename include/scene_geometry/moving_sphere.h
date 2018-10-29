//
// Created by Steve Hwan on 10/8/18.
//

#ifndef RAYTRACER2_MOVING_SPHERE_H
#define RAYTRACER2_MOVING_SPHERE_H

#include "ray_engine/hitable.h"
#include "materials/material.h"

class MovingSphere : public Hitable {
public:
    MovingSphere() = default;
    MovingSphere(Vec3 cen0, Vec3 cen1, float t0, float t1, float r, Material *m)
            : center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r),
              mat_ptr(m) {};
    bool hit(const Ray& r, float tmin, float tmax,
             HitRecord& rec) const override;
    bool bounding_box(float t0, float t1, AABB& box) const override;
    Vec3 center(float time) const;
    Vec3 center0, center1;
    float time0, time1;
    float radius;
    Material *mat_ptr;  // I think I actually own the material ptr,
                        // but I don't really want to.
};


#endif //RAYTRACER2_MOVING_SPHERE_H
