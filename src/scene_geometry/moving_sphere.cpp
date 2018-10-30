//
// Created by Steve Hwan on 10/8/18.
//

#include "scene_geometry/moving_sphere.h"

#include "scene_geometry/sphere.h"  // For the UV math

Vec3 MovingSphere::center(float time) const {
    return center_0_ + ((time - time_0_) / (time_1_ - time_0_))
            * (center_1_ - center_0_);
}

bool MovingSphere::Hit(const Ray &r, float t_min, float t_max,
                       HitRecord &rec) const {
    auto oc = r.origin() - center(r.time());
    auto a = dot(r.direction(), r.direction());
    auto b = dot(oc, r.direction());
    auto c = dot(oc, oc) - radius_ * radius_;
    auto discriminant = b * b - a * c;
    if (discriminant > 0) {
        // root 1
        auto temp = (-b - sqrtf(b*b - a*c)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.PointAtParameter(rec.t);
            GetSphereUV((rec.p - center(r.time())) / radius_, rec.u, rec.v);
            rec.normal = (rec.p - center(r.time())) / radius_;
            rec.material_ptr = material_ptr_;
            return true;
        }
        // root 2
        temp = (-b + sqrtf(b*b - a*c)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.PointAtParameter(rec.t);
            GetSphereUV((rec.p - center(r.time())) / radius_, rec.u, rec.v);
            rec.normal = (rec.p - center(r.time())) / radius_;
            rec.material_ptr = material_ptr_;
            return true;
        }
    }
    return false;
}

bool MovingSphere::BoundingBox(float t_min, float t_max, AABB &box) const {
    AABB temp_box0 = AABB(center_0_ - Vec3(radius_, radius_, radius_),
                          center_0_ + Vec3(radius_, radius_, radius_));
    AABB temp_box1 = AABB(center_1_ - Vec3(radius_, radius_, radius_),
                          center_1_ + Vec3(radius_, radius_, radius_));
    box = surrounding_box(temp_box0, temp_box1);
    return true;
}


