//
// Created by Steve Hwan on 10/1/18.
//

#include <cmath>

#include "scene_geometry/sphere.h"


// Arguably, this could go in a math library (as could vec3)
void GetSphereUV(const vec3 &p, float& u, float &v)
{
    auto phi = float(atan2(p.z(), p.x()));
    auto theta = float(asin(p.y()));
    u = 1.0 - (phi + M_PI) / (2 * M_PI);
    v = (theta + M_PI/2.0) / M_PI;
}


bool sphere::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
    auto oc = r.origin() - center;
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
            GetSphereUV((rec.p - center) / radius, rec.u, rec.v);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
        // root 2
        temp = (-b + sqrtf(b*b - a*c)) / a;
        if (temp < tmax && temp > tmin) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            GetSphereUV((rec.p - center) / radius, rec.u, rec.v);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

bool sphere::bounding_box(float t0, float t1, aabb& box) const {
    box = aabb(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
    return true;
}


