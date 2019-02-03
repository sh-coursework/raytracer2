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

        Vec3 emitted = rec.material_ptr->Emitted(rec.u, rec.v, rec.p);
        float pdf = 1.0f;
        if (depth < 50 && rec.material_ptr->Scatter(r, rec, attenuation, scattered, pdf))
//            return emitted
//                + attenuation * ColorForRay(scattered, world, depth + 1);
            return emitted
                    + attenuation
                      * rec.material_ptr->ScatteringPdf(r, rec, scattered)
                      * ColorForRay(scattered, world, depth + 1) / pdf;
        else
            return emitted;
    } else {
        return {0.0f, 0.0f, 0.0f};
    }
}