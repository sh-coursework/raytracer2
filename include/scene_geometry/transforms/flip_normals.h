//
// Created by Steve Hwan on 10/30/18.
//

#ifndef RAYTRACER2_FLIP_NORMALS_H
#define RAYTRACER2_FLIP_NORMALS_H

#include <memory>

#include "ray_engine/hitable.h"


class FlipNormals : public Hitable {
public:
  explicit FlipNormals(std::shared_ptr<Hitable> child_hitable_)
      : child_hitable_(std::move(child_hitable_)) {}

  bool Hit(const Ray &r, float t_min, float t_max,
           HitRecord &rec) const override;
  bool BoundingBox(float t_min, float t_max, AABB &box) const override;
  float PDFValue(const Vec3 &origin, const Vec3 &direction, float time) const override;
  Vec3 Random(const Vec3 &origin) const override;

  std::shared_ptr<Hitable> child_hitable_;
};


#endif //RAYTRACER2_FLIP_NORMALS_H
