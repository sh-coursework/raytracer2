//
// Created by Steve Hwan on 10/1/18.
//

#ifndef RAYTRACER1_HITABLE_LIST_H
#define RAYTRACER1_HITABLE_LIST_H

#include <utility>
#include <vector>

#include "ray_engine/hitable.h"
#include "ray_engine/aabb.h"

class HitableList : public Hitable {
public:
    HitableList() = default;
    explicit HitableList(std::vector<std::shared_ptr<Hitable>> &l)
            {vector_list_ = std::move(l); }
    bool Hit(const Ray &r, float t_min, float t_max,
             HitRecord &rec) const override;
    bool BoundingBox(float t_min, float t_max, AABB &box) const override;

    std::vector<std::shared_ptr<Hitable>> vector_list_;
};


#endif //RAYTRACER1_HITABLE_LIST_H
