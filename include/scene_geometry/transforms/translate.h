//
// Created by Steve Hwan on 10/30/18.
//

#ifndef RAYTRACER2_TRANSLATE_H
#define RAYTRACER2_TRANSLATE_H

#include "vec3.h"
#include "ray_engine/ray.h"
#include "ray_engine/aabb.h"
#include "ray_engine/hitable.h"


class Translate : public Hitable {
public:
    Translate(const Vec3 &translate, Hitable *child_hitable)
            :  child_hitable_(child_hitable), translate_(translate) {}

    bool Hit(const Ray &r, float t_min, float t_max,
            HitRecord &rec) const override;
    bool BoundingBox(float t_min, float t_max, AABB &box) const override;

    Hitable *child_hitable_;
    Vec3 translate_;
};


#endif //RAYTRACER2_TRANSLATE_H
