//
// Created by Steve Hwan on 10/21/18.
//

#include "ray_engine/ray_engine.h"

#include "materials/material.h"


Vec3 ColorForRay(const Ray &r, const Hitable *const world, int depth) {
    // Added Special 0,0 Ray - stop hit test and return full.
    // (But how expensive is this test on every ColorForRay call?)
    // TODO: get rid of this - it's already covered by emission.
    if (r.origin().length() == 0.0f and r.direction().length() == 0.0f)
        return {1.0f, 1.0f, 1.0f};

    HitRecord rec;
    if (world->Hit(r, 0.001f, MAXFLOAT, rec)) {
        Ray scattered;
        Vec3 attenuation;

        Vec3 emitted {0.0f, 0.0f, 0.0f};
        if (dot(rec.normal, r.direction()) < 0.0)
            emitted = rec.material_ptr->Emitted(rec.u, rec.v, rec.p);

        float pdf {1.0f};
        if (depth < 50 && rec.material_ptr->Scatter(r, rec, attenuation, scattered, pdf)) {
            // Book 3 Chapter 7 hack hardcoded sampling assuming
            // specific Cornell box light
            Vec3 on_light = Vec3(213 + drand48() * (343 - 213), 554, 227 + drand48() * (332 - 227));
            Vec3 to_light = on_light - rec.p;
            float distance_squared = to_light.squared_length();
            to_light.make_unit_vector();
            if (dot(to_light, rec.normal) < 0)
                return emitted;
            float light_area = (343 - 213) * (332 - 227);
            float light_cosine = fabs(to_light.y());
            if (light_cosine < 0.000001)
                return emitted;
            pdf = distance_squared / (light_cosine * light_area);
            scattered = Ray(rec.p, to_light, r.time());

            return emitted
                + attenuation
                    * rec.material_ptr->ScatteringPdf(r, rec, scattered)
                    * ColorForRay(scattered, world, depth + 1) / pdf;
        } else
            return emitted;
    } else {
        return {0.0f, 0.0f, 0.0f};
    }
}