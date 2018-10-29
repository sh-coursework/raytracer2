//
// Created by Steve Hwan on 10/8/18.
//

#include "scene_geometry/moving_sphere.h"

#include "scene_geometry/sphere.h"  // For the UV math

Vec3 MovingSphere::center(float time) const {
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool MovingSphere::hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const {
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
            GetSphereUV((rec.p - center(r.time())) / radius, rec.u, rec.v);
            rec.normal = (rec.p - center(r.time())) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
        // root 2
        temp = (-b + sqrtf(b*b - a*c)) / a;
        if (temp < tmax && temp > tmin) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            GetSphereUV((rec.p - center(r.time())) / radius, rec.u, rec.v);
            rec.normal = (rec.p - center(r.time())) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

bool MovingSphere::bounding_box(float t0, float t1, AABB& box) const {
    AABB temp_box0 = AABB(center0 - Vec3(radius, radius, radius), center0 + Vec3(radius, radius, radius));
    AABB temp_box1 = AABB(center1 - Vec3(radius, radius, radius), center1 + Vec3(radius, radius, radius));
    box = surrounding_box(temp_box0, temp_box1);
    return true;
}


