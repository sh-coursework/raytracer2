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

// Basically a BVHNode, except it owns the hitables list.
// It's a little awkward because the constructor has a
// completely different signature than the parent.
// But I wanted to avoid the additional Hit call that was
// just deferring to the first BVHNode.
class BVHRoot : public BVHNode {
 public:
  BVHRoot() = default;
  explicit BVHRoot(std::vector<std::shared_ptr<Hitable>> &hitables,
                   float time0, float time1)
    : BVHNode(hitables.begin(),
              hitables.end(), time0, time1),
      vector_list_(std::move(hitables)) {}

  std::vector<std::shared_ptr<Hitable>> vector_list_;
};

#endif //RAYTRACER2_BVH_ROOT_H
