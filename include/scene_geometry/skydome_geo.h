//
// Created by svhwan on 6/16/2019.
//

#ifndef RAYTRACER2_SRC_SCENE_GEOMETRY_SKYDOME_GEO_H_
#define RAYTRACER2_SRC_SCENE_GEOMETRY_SKYDOME_GEO_H_

#include "ray_engine/hitable.h"
#include "ray_engine/ray.h"
#include "materials/material.h"

// A skydome is all sorts of crazy in that it's infinitely far away,
// and might have a different alpha treatment, so don't treat it as
// just normal geometry.

class SkydomeGeo : public Hitable {
 public:
  explicit SkydomeGeo(Material *material_ptr) : material_ptr_(material_ptr) {}
  bool Hit(const Ray &r, float t_min, float t_max, HitRecord &rec) const override;
 private:
  bool BoundingBox(float t_min, float t_max, AABB &box) const override;;
 private:
  Material *material_ptr_;
};

#endif //RAYTRACER2_SRC_SCENE_GEOMETRY_SKYDOME_GEO_H_
