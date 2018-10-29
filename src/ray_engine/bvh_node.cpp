//
// Created by Steve Hwan on 10/9/18.
//

#include "ray_engine/bvh_node.h"

#include <cmath>


bool box_x_compare(const std::shared_ptr<Hitable> &hitable_a,
                   const std::shared_ptr<Hitable> &hitable_b)
{
    AABB box_left, box_right;
    if (!hitable_a->bounding_box(0, 0, box_left) || !hitable_b->bounding_box(0, 0, box_right))
        std::cerr << "no bounding box in BVHNode constructor\n";
    return bool(box_left.min().x() < box_right.min().x());
}

bool box_y_compare(const std::shared_ptr<Hitable> &hitable_a,
                   const std::shared_ptr<Hitable> &hitable_b)
{
    AABB box_left, box_right;
    if (!hitable_a->bounding_box(0, 0, box_left) || !hitable_b->bounding_box(0, 0, box_right))
        std::cerr << "no bounding box in BVHNode constructor\n";
    return bool(box_left.min().y() < box_right.min().y());
}

bool box_z_compare(const std::shared_ptr<Hitable> &hitable_a,
                   const std::shared_ptr<Hitable> &hitable_b)
{
    AABB box_left, box_right;
    if (!hitable_a->bounding_box(0, 0, box_left) || !hitable_b->bounding_box(0, 0, box_right))
        std::cerr << "no bounding box in BVHNode constructor\n";
    return bool(box_left.min().z() < box_right.min().z());
}


BVHNode::BVHNode(
        std::vector<std::shared_ptr<Hitable>>::iterator hitable_begin,
        std::vector<std::shared_ptr<Hitable>>::iterator hitable_end,
        float time0, float time1)
{
    auto n = hitable_end - hitable_begin;

    auto axis = int(3 * drand48());
    if (axis == 0)
        sort(hitable_begin, hitable_end, box_x_compare);
    else if (axis == 1)
        sort(hitable_begin, hitable_end, box_y_compare);
    else
        sort(hitable_begin, hitable_end, box_z_compare);

    if (n == 1) {
        left = hitable_begin[0];
        right = hitable_begin[0];
    }
    else if (n ==2) {
        left = hitable_begin[0];
        right = hitable_begin[1];
    }
    else {
        left = std::shared_ptr<Hitable>(
                new BVHNode(hitable_begin, hitable_begin + (n / 2), time0, time1)
        );
        right = std::shared_ptr<Hitable>(
                new BVHNode(hitable_begin + (n / 2), hitable_end, time0, time1)
        );
    }

    AABB box_left, box_right;
    if (!left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0, time1, box_right))
        std::cerr << "no bounding box in bvh+node constructor\n";
    box = surrounding_box(box_left, box_right);
}

bool BVHNode::bounding_box(float t0, float t1, AABB &b) const
{
    b = box;
    return true;
}

bool BVHNode::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
    if (box.hit(r, t_min, t_max)) {
        HitRecord left_rec, right_rec;
        auto hit_left = left->hit(r, t_min, t_max, left_rec);
        auto hit_right = right->hit(r, t_min, t_max, right_rec);
        if (hit_left && hit_right) {
            if (left_rec.t < right_rec.t)
                rec = left_rec;
            else
                rec = right_rec;
            return true;
        }
        else if (hit_left) {
            rec = left_rec;
            return true;
        }
        else if (hit_right) {
            rec = right_rec;
            return true;
        }
        else
            return false;
    }
    else return false;
}




