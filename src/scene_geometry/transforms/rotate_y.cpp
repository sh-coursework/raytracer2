//
// Created by Steve Hwan on 10/30/18.
//

#include "scene_geometry/transforms/rotate_y.h"

#include <algorithm>
#include <cfloat>
#include <cmath>

#include <boost/range/irange.hpp>


RotateY::RotateY(float angle, Hitable *child_hitable)
        : child_hitable_(child_hitable) {
    float radians = (M_PI / 180.) * angle;
    sin_theta_ = sin(radians);
    cos_theta_ = cos(radians);
    has_box_ = child_hitable_->BoundingBox(0, 1, bounding_box_);
    Vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
    Vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    for(auto i: {0, 1}) {
        for (auto j: {0, 1}) {
            for (auto k: {0, 1}) {
                float x = i * bounding_box_.max().x()
                        + (1 - i) * bounding_box_.min().x();
                float y = j * bounding_box_.max().y()
                        + (1 - j) * bounding_box_.min().y();
                float z = k * bounding_box_.max().z()
                        + (1 - k) * bounding_box_.min().z();
                float new_x = cos_theta_ * x + sin_theta_ * z;
                float new_z = -sin_theta_ * x + cos_theta_ * z;
                Vec3 tester(new_x, y, new_z);
                for (auto c: {0, 1, 2}) {
                    if (tester[c] > max[c])
                        max[c] = tester[c];
                    if (tester[c] < min[c])
                        min[c] = tester[c];
                }

            }
        }
    }
    bounding_box_ = AABB(min, max);
}

bool RotateY::Hit(const Ray &r, float t_min, float t_max,
        HitRecord &rec) const {
    Vec3 origin = r.origin();
    Vec3 direction = r.direction();
    origin[0] = cos_theta_ * r.origin()[0] - sin_theta_ * r.origin()[2];
    origin[2] = sin_theta_ * r.origin()[0] + cos_theta_ * r.origin()[2];
    direction[0] = cos_theta_ * r.direction()[0] - sin_theta_ * r.direction()[2];
    direction[2] = sin_theta_ * r.direction()[0] + cos_theta_ * r.direction()[2];
    Ray rotated_r(origin, direction, r.time());
    if (child_hitable_->Hit(rotated_r, t_min, t_max, rec)) {
        Vec3 p = rec.p;
        Vec3 normal = rec.normal;
        p[0] = cos_theta_ * rec.p[0] + sin_theta_ * rec.p[2];
        p[2] = -sin_theta_ * rec.p[0] + cos_theta_ * rec.p[2];
        normal[0] = cos_theta_ * rec.normal[0] + sin_theta_ * rec.normal[2];
        normal[2] = -sin_theta_ * rec.normal[0] + cos_theta_ * rec.normal[2];
        rec.p = p;
        rec.normal = normal;
        return true;
    }
    else
        return false;
}

bool RotateY::BoundingBox(float t_min, float t_max, AABB &box) const {
    box = bounding_box_;
    return has_box_;
}
