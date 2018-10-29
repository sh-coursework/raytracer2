//
// Created by Steve Hwan on 10/21/18.
//

#include "ray_engine/ray_engine.h"

#include "materials/material.h"


Vec3 ColorForRay(const Ray &r, Hitable *world, int depth) {
    // Added Special 0,0 Ray - stop hit test and return full.
    // (But how expensive is this test on every ColorForRay call?)
    if (r.origin().length() == 0.0f and r.direction().length() == 0.0f)
        return {1.0f, 1.0f, 1.0f};

    HitRecord rec;
    if (world->hit(r, 0.001f, MAXFLOAT, rec)) {
        Ray scattered;
        Vec3 attenuation;

        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * ColorForRay(scattered, world, depth + 1);
        } else {
            return {0.0f, 0.0f, 0.0f};
        }
    } else {
        auto unit_direction = unit_vector(r.direction());
        auto t = 0.5f * (unit_direction.y() + 1.0f);
        return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
    }
}