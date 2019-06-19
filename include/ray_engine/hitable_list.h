//
// Created by Steve Hwan on 10/1/18.
//

#ifndef RAYTRACER1_HITABLE_LIST_H
#define RAYTRACER1_HITABLE_LIST_H

#include <memory>
#include <utility>
#include <vector>

#include "ray_engine/aabb.h"
#include "ray_engine/hitable.h"
#include "ray_engine/ray.h"
#include "vec3.h"

class HitableList : public Hitable {
public:
  HitableList() = default;
  explicit HitableList(std::vector<std::shared_ptr<Hitable>> &hitables)
          {vector_list_ = std::move(hitables); }
  void push_back(const std::shared_ptr<Hitable> &new_hitable_ptr)  // includes ownership, not sure if it should
    {vector_list_.push_back(new_hitable_ptr); }
  int size() const
    {return vector_list_.size(); }
  bool Hit(const Ray &r, float t_min, float t_max,
           HitRecord &rec) const override;
  bool BoundingBox(float t_min, float t_max, AABB &box) const override;
  float PDFValue(const Vec3 &origin,
                 const Vec3 &direction,
                 float time) const override;
  Vec3 Random(const Vec3 &origin) const override;

  std::vector<std::shared_ptr<Hitable>> vector_list_;
};


#endif //RAYTRACER1_HITABLE_LIST_H
