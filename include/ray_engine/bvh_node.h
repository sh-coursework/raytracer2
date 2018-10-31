//
// Created by Steve Hwan on 10/9/18.
//

#ifndef RAYTRACER2_BVH_NODE_H
#define RAYTRACER2_BVH_NODE_H

#include <vector>

#include "ray_engine/aabb.h"
#include "ray_engine/ray.h"
#include "ray_engine/hitable.h"

class BVHNode : public Hitable {
public:
    BVHNode() = default;
    BVHNode(std::vector<std::shared_ptr<Hitable>>::iterator hitable_begin,
            std::vector<std::shared_ptr<Hitable>>::iterator hitable_end,
            float time0, float time1);
    bool Hit(const Ray &r, float t_min, float t_max,
             HitRecord &rec) const override;
    bool BoundingBox(float t_min, float t_max, AABB &box) const override;

    std::shared_ptr<Hitable> left_;
    std::shared_ptr<Hitable> right_;
    AABB box_;
};

bool box_x_compare(const std::shared_ptr<Hitable> &hitable_a,
                   const std::shared_ptr<Hitable> &hitable_b);
bool box_y_compare(const std::shared_ptr<Hitable> &hitable_a,
                   const std::shared_ptr<Hitable> &hitable_b);
bool box_z_compare(const std::shared_ptr<Hitable> &hitable_a,
                   const std::shared_ptr<Hitable> &hitable_b);

#endif //RAYTRACER2_BVH_NODE_H
