//
// Created by Steve Hwan on 10/1/18.
//

#include "ray_engine/hitable_list.h"

#include <boost/range/irange.hpp>

bool HitableList::Hit(const Ray &r, float t_min, float t_max,
                      HitRecord &rec) const {
    HitRecord temp_rec;
    auto hit_anything = false;
    double closest_so_far = t_max;
    for (auto &curr_hitable: vector_list_) {
        if (curr_hitable->Hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;  // TODO: Can I move?  Would it matter?
        }
    }
    return hit_anything;
}

bool HitableList::BoundingBox(float t_min, float t_max, AABB &box) const {
    if (vector_list_.empty())
        return false;

    AABB temp_box;

    // Special treatment for first element
    auto hitables_it = vector_list_.begin();
    if ((*hitables_it)->BoundingBox(t_min, t_max, temp_box)) {
        box = temp_box;
    } else
        return false;

    std::advance(hitables_it, 1);
    for (; hitables_it != vector_list_.end(); ++hitables_it) {
        if ((*hitables_it)->BoundingBox(t_min, t_max, temp_box)) {
            box = surrounding_box(box, temp_box);
        } else
            return false;
    }
    return true;
}

float HitableList::PDFValue(const Vec3 &origin,
                            const Vec3 &direction,
                            float time) const {
    auto weight = 1.0f / float(vector_list_.size());
    auto sum = 0.0f;
    for (auto &curr_hitable: vector_list_)
        sum += weight * curr_hitable->PDFValue(origin, direction, time);

    return sum;
}

Vec3 HitableList::Random(const Vec3 &origin) const {
    auto index =int(drand48() * vector_list_.size());
    return vector_list_[index]->Random(origin);
}
