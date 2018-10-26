//
// Created by Steve Hwan on 10/9/18.
//

#ifndef RAYTRACER2_BVH_NODE_H
#define RAYTRACER2_BVH_NODE_H

#include <vector>
#include "ray_engine/aabb.h"
#include "ray_engine/hitable.h"

class bvh_node : public hitable {
public:
    std::shared_ptr<hitable> left;
    std::shared_ptr<hitable> right;
    aabb box;

    bvh_node() = default;
    bvh_node(
            std::vector<std::shared_ptr<hitable>>::iterator hitable_begin,
            std::vector<std::shared_ptr<hitable>>::iterator hitable_end,
            float time0, float time1);
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;
    virtual bool bounding_box(float t0, float t1, aabb& box) const override;
};

bool box_x_compare(const std::shared_ptr<hitable> &hitable_a,
                   const std::shared_ptr<hitable> &hitable_b);
bool box_y_compare(const std::shared_ptr<hitable> &hitable_a,
                   const std::shared_ptr<hitable> &hitable_b);
bool box_z_compare(const std::shared_ptr<hitable> &hitable_a,
                   const std::shared_ptr<hitable> &hitable_b);

#endif //RAYTRACER2_BVH_NODE_H
