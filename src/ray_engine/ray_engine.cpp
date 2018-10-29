//
// Created by Steve Hwan on 10/21/18.
//

#include "ray_engine/ray_engine.h"

// Until now, material was through forward declaration.
// But since I actually do need to access a method in
// the material, I need to fully include it here.
#include "materials/material.h"


vec3 color(const ray& r, hitable *world, int depth) {
    // Added Special 0,0 ray - stop hit test and return full.
    // (But how expensive is this test on every color call?)
    if (r.origin().length() == 0.0f and r.direction().length() == 0.0f)
        return {1.0f, 1.0f, 1.0f};

    hit_record rec;
    if (world->hit(r, 0.001f, MAXFLOAT, rec)) {
        ray scattered;
        vec3 attenuation;

        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * color(scattered, world, depth+1);
        } else {
            return {0.0f, 0.0f, 0.0f};
        }
    } else {
        auto unit_direction = unit_vector(r.direction());
        auto t = 0.5f * (unit_direction.y() + 1.0f);
        return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
    }
}