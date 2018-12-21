//
// Created by Steve Hwan on 12/20/18.
//

#ifndef RAYTRACER2_BVH_ROOT_H
#define RAYTRACER2_BVH_ROOT_H

#include <memory>

#include "ray_engine/hitable_list.h"
#include "ray_engine/bvh_node.h"

#include "ray_engine/hitable.h"
#include "ray_engine/ray.h"
#include "ray_engine/aabb.h"

// This is effectively a BVHNode, but I want to store the list, and I don't
// want to pollute every BVH node with a pointer that is going to be null for
// all but one.
class BVHRoot : public HitableList {
 public:
  BVHRoot() = default;
  BVHRoot(std::vector<std::shared_ptr<Hitable>> &hitables,
      float time0, float time1)
    : HitableList(hitables) {
    bvh_root_ptr_ = std::unique_ptr<BVHNode>(new BVHNode(vector_list_.begin(),
        vector_list_.end(), time0, time1));
  };
  inline bool Hit(const Ray &r, float t_min, float t_max, HitRecord &rec)
      const override {
    return bvh_root_ptr_->Hit(r, t_min, t_max, rec);
  }
  inline bool BoundingBox(float t_min, float t_max, AABB &box) const override {
    return bvh_root_ptr_->BoundingBox(t_min, t_max, box);
  };

  std::unique_ptr<BVHNode> bvh_root_ptr_;
};

#endif //RAYTRACER2_BVH_ROOT_H
