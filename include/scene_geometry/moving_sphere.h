//
// Created by Steve Hwan on 10/8/18.
//

#ifndef RAYTRACER2_MOVING_SPHERE_H
#define RAYTRACER2_MOVING_SPHERE_H

#include "vec3.h"
#include "ray_engine/hitable.h"
#include "ray_engine/aabb.h"
#include "materials/material.h"


class MovingSphere : public Hitable {
public:
    MovingSphere() = default;
    MovingSphere(const Vec3 &cen0, const Vec3 &cen1, float t0, float t1, float r, Material *m)
            : center_0_(cen0), center_1_(cen1), time_0_(t0), time_1_(t1),
              radius_(r), material_ptr_(m) {};
    bool Hit(const Ray &r, float t_min, float t_max,
             HitRecord &rec) const override;
    bool BoundingBox(float t_min, float t_max, AABB &box) const override;
    Vec3 center(float time) const;

    Vec3 center_0_, center_1_;
    float time_0_, time_1_;
    float radius_;
    Material *material_ptr_;  // I think I actually own the material ptr,
                              // but I don't really want to.
};


#endif //RAYTRACER2_MOVING_SPHERE_H
