//
// Created by Steve Hwan on 10/30/18.
//

#ifndef RAYTRACER2_ROTATE_Y_H
#define RAYTRACER2_ROTATE_Y_H

#include "ray_engine/ray.h"
#include "ray_engine/aabb.h"
#include "ray_engine/hitable.h"


class RotateY : public Hitable {
public:
    RotateY(float angle, Hitable *child_hitable);
    bool Hit(const Ray &r, float t_min, float t_max,
            HitRecord &rec) const override;

    bool BoundingBox(float t_min, float t_max, AABB &box) const override;

    Hitable *child_hitable_;
    float sin_theta_;
    float cos_theta_;
    bool has_box_;
    AABB bounding_box_;
};


#endif //RAYTRACER2_ROTATE_Y_H
