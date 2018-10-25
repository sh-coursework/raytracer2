//
// Created by Steve Hwan on 10/1/18.
//

#ifndef RAYTRACER1_HITABLE_LIST_H
#define RAYTRACER1_HITABLE_LIST_H

#include <utility>
#include <vector>

#include "ray_engine/hitable.h"
#include "ray_engine/aabb.h"

class hitable_list : public hitable {
public:
    std::vector<std::shared_ptr<hitable>> vector_list_;

    hitable_list() = default;
    explicit hitable_list(std::vector<std::shared_ptr<hitable>> &l) {vector_list_ = std::move(l); }
    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;
    bool bounding_box(float t0, float t1, aabb& box) const override;
};


#endif //RAYTRACER1_HITABLE_LIST_H
