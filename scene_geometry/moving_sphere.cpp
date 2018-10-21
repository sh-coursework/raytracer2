//
// Created by Steve Hwan on 10/8/18.
//

#include "moving_sphere.h"

vec3 moving_sphere::center(float time) const {
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool moving_sphere::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
    auto oc = r.origin() - center(r.time());
    auto a = dot(r.direction(), r.direction());
    auto b = dot(oc, r.direction());
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = b * b - a * c;
    if (discriminant > 0) {
        // root 1
        auto temp = (-b - sqrtf(b*b - a*c)) / a;
        if (temp < tmax && temp > tmin) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center(r.time())) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
        // root 2
        temp = (-b + sqrtf(b*b - a*c)) / a;
        if (temp < tmax && temp > tmin) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center(r.time())) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

bool moving_sphere::bounding_box(float t0, float t1, aabb& box) const {
    aabb temp_box0 = aabb(center0 - vec3(radius, radius, radius), center0 + vec3(radius, radius, radius));
    aabb temp_box1 = aabb(center1 - vec3(radius, radius, radius), center1 + vec3(radius, radius, radius));
    box = surrounding_box(temp_box0, temp_box1);
    return true;
}


