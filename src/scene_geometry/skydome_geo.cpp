//
// Created by svhwan on 6/16/2019.
//

#include <cfloat>

#include "scene_geometry/skydome_geo.h"

bool SkydomeGeo::Hit(const Ray &r, float t_min, float t_max, HitRecord &rec) const {
  // Hit will actually be at infinity, and shouldn't have an alpha.
  // That is, this is all sorts of a crazy weird hit record,
  // so the SkydomeGeo needs to be treated separately/carefully for now.
  Vec3 unit_direction = unit_vector(r.direction());
  rec.t = FLT_MAX;
  rec.u = 0.0;
  rec.v = 0.5 * (unit_direction.y() + 1.0);  // look at y mapped [-1,1] to [0,1]
  rec.material_ptr = material_ptr_;
  return true;
}

bool SkydomeGeo::BoundingBox(float t_min, float t_max, AABB &box) const {
  return false;
}
