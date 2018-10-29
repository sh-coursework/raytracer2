//
// Created by Steve Hwan on 10/1/18.
//

#include "scene_geometry/sphere.h"

#include <cmath>


// Arguably, this could go in a math library (as could Vec3)
void GetSphereUV(const Vec3 &p, float& u, float &v)
{
    auto phi = float(atan2(p.z(), p.x()));
    auto theta = float(asin(p.y()));
    u = 1.0 - (phi + M_PI) / (2 * M_PI);
    v = (theta + M_PI/2.0) / M_PI;
}


bool Sphere::hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const {
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

bool Sphere::bounding_box(float t0, float t1, AABB& box) const {
    box = AABB(center - Vec3(radius, radius, radius),
               center + Vec3(radius, radius, radius));
    return true;
}


