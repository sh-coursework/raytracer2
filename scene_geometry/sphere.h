//
// Created by Steve Hwan on 10/1/18.
//

#ifndef RAYTRACER1_SPHERE_H
#define RAYTRACER1_SPHERE_H

#include "hitable.h"

class sphere : public hitable {
public:
    sphere() {}
    sphere(vec3 cen, float r, material *m) : center(cen), radius(r), mat_ptr(m) {};
    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
    vec3 center;
    float radius;
    material *mat_ptr;  // I think I actually own the material ptr, but I don't really want to.
};

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
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
        // root 2
        temp = (-b + sqrtf(b*b - a*c)) / a;
        if (temp < tmax && temp > tmin) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

#endif //RAYTRACER1_SPHERE_H
