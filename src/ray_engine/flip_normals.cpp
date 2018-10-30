//
// Created by Steve Hwan on 10/30/18.
//

#include "ray_engine/flip_normals.h"

bool FlipNormals::Hit(const Ray &r, float t_min, float t_max, HitRecord &rec) const {
    if (orig_hitable_ptr_->Hit(r, t_min, t_max, rec)) {
        rec.normal = -rec.normal;
        return true;
    } else
        return false;
}

bool FlipNormals::BoundingBox(float t_min, float t_max, AABB &box) const {
    return orig_hitable_ptr_->BoundingBox(t_min, t_max, box);
}
