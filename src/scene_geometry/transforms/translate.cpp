//
// Created by Steve Hwan on 10/30/18.
//

#include "scene_geometry/transforms/translate.h"

bool Translate::Hit(const Ray &r, float t_min, float t_max,
        HitRecord &rec) const {
    Ray moved_r(r.origin() - translate_, r.direction(), r.time());
    if (child_hitable_->Hit(moved_r, t_min, t_max, rec)) {
        rec.p += translate_;
        return true;
    }
    else
        return false;
}

bool Translate::BoundingBox(float t_min, float t_max, AABB &box) const {
    if (child_hitable_->BoundingBox(t_min, t_max, box)) {
        box = AABB(box.min() + translate_, box.max() + translate_);
        return true;
    }
    else
        return false;
}
