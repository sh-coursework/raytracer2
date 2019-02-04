//
// Created by Steve Hwan on 10/21/18.
//

#include <memory>

#include <materials/cosine_pdf.h>
#include <materials/hitable_pdf.h>
#include <materials/mixture_pdf.h>

#include "ray_engine/ray_engine.h"

// TODO: Super unhappy about bringing geometry
// TODO for a specific light in here.
#include "scene_geometry/rect.h"

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
            // Book 3, Chapter 8 hardcodes the Cornell Box light geometry
            // here in the oolor function. :(
            std::shared_ptr<Hitable> light_shape =
                                         std::shared_ptr<XZRect>( new XZRect(213, 343, 227, 332, 554, nullptr));
            HitablePDF hitable_pdf(light_shape, rec.p);
            CosinePDF cosine_pdf(rec.normal);
            MixturePDF mixture_pdf(&hitable_pdf, &cosine_pdf);

            scattered = Ray(rec.p, mixture_pdf.Generate(), r.time());
            pdf = mixture_pdf.Value(scattered.direction());

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