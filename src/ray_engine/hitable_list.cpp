//
// Created by Steve Hwan on 10/1/18.
//

#include "ray_engine/hitable_list.h"

#include <boost/range/irange.hpp>


bool HitableList::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
    HitRecord temp_rec;
    auto hit_anything = false;
    double closest_so_far = t_max;
    for (auto &curr_hitable: vector_list_) {
        if (curr_hitable->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

bool HitableList::bounding_box(float t0, float t1, AABB& box) const {
    if (vector_list_.empty())
        return false;

    AABB temp_box;

    // Special treatment for first element
    auto hitables_it = vector_list_.begin();
    if ((*hitables_it)->bounding_box(t0, t1, temp_box)) {
        box = temp_box;
    } else
        return false;

    std::advance(hitables_it, 1);
    for (; hitables_it != vector_list_.end(); ++hitables_it) {
        if ((*hitables_it)->bounding_box(t0, t1, temp_box)) {
            box = surrounding_box(box, temp_box);
        } else
            return false;
    }
    return true;
}
