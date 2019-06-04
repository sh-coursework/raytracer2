//
// Created by Steve Hwan on 10/30/18.
//

// RotateBase IS A TEMPLATE

#include <algorithm>
#include <cfloat>
#include <cmath>

#include <boost/range/irange.hpp>

#include "scene_geometry/transforms/rotate_base.h"


template <int N0, int N1, int NRotationAxis>
RotateBase<N0, N1, NRotationAxis>::RotateBase(float angle, std::shared_ptr<Hitable> child_hitable)
        : child_hitable_(std::move(child_hitable)) {
    float radians = (M_PI / 180.) * angle;
    sin_theta_ = sin(radians);
    cos_theta_ = cos(radians);
    has_box_ = child_hitable_->BoundingBox(0, 1, bounding_box_);
    Vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
    Vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    for(auto i: {0, 1}) {
        for (auto j: {0, 1}) {
            for (auto k: {0, 1}) {
                float bbox[3] = {
                  i * bounding_box_.max().x()
                          + (1 - i) * bounding_box_.min().x(),
                  j * bounding_box_.max().y()
                          + (1 - j) * bounding_box_.min().y(),
                  k * bounding_box_.max().z()
                          + (1 - k) * bounding_box_.min().z()
                };
                // Rotate the bounding box
                float transformed_bbox[3];
                transformed_bbox[N0] = cos_theta_ * bbox[N0] + sin_theta_ * bbox[N1];
                transformed_bbox[N1] = -sin_theta_ * bbox[N0] + cos_theta_ * bbox[N1];
                transformed_bbox[NRotationAxis] = bbox[NRotationAxis];
                Vec3 tester(transformed_bbox[0], transformed_bbox[1],
                            transformed_bbox[2]);
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

template <int N0, int N1, int NRotationAxis>
bool RotateBase<N0, N1, NRotationAxis>::Hit(const Ray &r, float t_min, float t_max,
        HitRecord &rec) const {
    Vec3 origin = r.origin();
    Vec3 direction = r.direction();
    origin[N0] = cos_theta_ * r.origin()[N0] - sin_theta_ * r.origin()[N1];
    origin[N1] = sin_theta_ * r.origin()[N0] + cos_theta_ * r.origin()[N1];
    direction[N0] = cos_theta_ * r.direction()[N0] - sin_theta_ * r.direction()[N1];
    direction[N1] = sin_theta_ * r.direction()[N0] + cos_theta_ * r.direction()[N1];
    Ray rotated_r(origin, direction, r.time());
    if (child_hitable_->Hit(rotated_r, t_min, t_max, rec)) {
        Vec3 p = rec.p;
        Vec3 normal = rec.normal;
        p[N0] = cos_theta_ * rec.p[N0] + sin_theta_ * rec.p[N1];
        p[N1] = -sin_theta_ * rec.p[N0] + cos_theta_ * rec.p[N1];
        normal[N0] = cos_theta_ * rec.normal[N0] + sin_theta_ * rec.normal[N1];
        normal[N1] = -sin_theta_ * rec.normal[N0] + cos_theta_ * rec.normal[N1];
        rec.p = p;
        rec.normal = normal;
        return true;
    }
    else
        return false;
}

template <int N0, int N1, int NRotationAxis>
bool RotateBase<N0, N1, NRotationAxis>::BoundingBox(float t_min, float t_max, AABB &box) const {
    box = bounding_box_;
    return has_box_;
}
