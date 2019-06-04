//
// Created by Steve Hwan on 10/30/18.
//

#ifndef RAYTRACER2_ROTATE_BASE_H
#define RAYTRACER2_ROTATE_BASE_H

// RotateBase IS A TEMPLATE

#include <memory>

#include "ray_engine/ray.h"
#include "ray_engine/aabb.h"
#include "ray_engine/hitable.h"


template <int N0, int N1, int NRotationAxis>
class RotateBase : public Hitable {
public:
    RotateBase(float angle, std::shared_ptr<Hitable> child_hitable);
    bool Hit(const Ray &r, float t_min, float t_max,
            HitRecord &rec) const override;

    bool BoundingBox(float t_min, float t_max, AABB &box) const override;

    std::shared_ptr<Hitable> child_hitable_;
    float sin_theta_;
    float cos_theta_;
    bool has_box_;
    AABB bounding_box_;
};

#include "scene_geometry/transforms/rotate_base.cpp"

#endif //RAYTRACER2_ROTATE_BASE_H
