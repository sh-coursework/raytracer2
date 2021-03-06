//
// Created by Steve Hwan on 10/30/18.
//

#include "scene_geometry/transforms/flip_normals.h"

bool FlipNormals::Hit(const Ray &r, float t_min, float t_max, HitRecord &rec) const {
    if (child_hitable_->Hit(r, t_min, t_max, rec)) {
        rec.normal = -rec.normal;
        return true;
    } else
        return false;
}

bool FlipNormals::BoundingBox(float t_min, float t_max, AABB &box) const {
    return child_hitable_->BoundingBox(t_min, t_max, box);
}

float FlipNormals::PDFValue(const Vec3 &origin, const Vec3 &direction, float time) const {
  return child_hitable_->PDFValue(origin, direction, time);
}

Vec3 FlipNormals::Random(const Vec3 &origin) const {
  return child_hitable_->Random(origin);
}
