//
// Created by Steve Hwan on 10/21/18.
//

#include "ray_engine/ray_engine.h"

#include <memory>

#include <materials/cosine_pdf.h>
#include <materials/hitable_pdf.h>
#include "materials/material.h"
#include <materials/mixture_pdf.h>

Vec3 DeNaN(const Vec3 &new_color) {
    Vec3 temp = new_color;
    if (! (temp[0] == temp[0])) temp[0] = 0;
    if (! (temp[1] == temp[1])) temp[1] = 0;
    if (! (temp[2] == temp[2])) temp[2] = 0;
    return temp;
}

Vec3 ColorForRay(const Ray &r, const Hitable *const world,
                 const Hitable *const light_shape_raw_ptr, int depth) {
    // Added Special 0,0 Ray - stop hit test and return full.
    // (But how expensive is this test on every ColorForRay call?)

    HitRecord hit_record;
    if (world->Hit(r, 0.001f, MAXFLOAT, hit_record)) {
        Vec3 emitted {0.0f, 0.0f, 0.0f};
        if (dot(hit_record.normal, r.direction()) < 0.0)
            emitted = hit_record.material_ptr->Emitted(hit_record.u,
                hit_record.v, hit_record.p);

        ScatterRecord scatter_record;  // set in Scatter, lifetime if world-hit
        if (depth < 50 && hit_record.material_ptr->Scatter(r, hit_record,
                                                           scatter_record)) {
            // Book 3, Chapter 8 hardcodes the Cornell Box light geometry
            // here in the oolor function. :(
            if (scatter_record.is_specular) {
                // TODO: I don't think this accounts for "fuzz" in pdf
                // Maybe it's not so much "is_specular" that we should be
                // testing. But "do_mixture_pdf" should be the exception,
                // to be activated when a pdf is implemented.
                // And note: the next thing implemented in the book is
                // cone pdf from sampling a sphere, so maybe that could actually
                // be used to construct a pdf that accounts for the fuzzing.
                // (though it will be more expensive).
                return scatter_record.attenuation
                    * ColorForRay(scatter_record.specular_ray, world,
                                  light_shape_raw_ptr, depth + 1);
            } else if (scatter_record.do_mixture_pdf){
                // So PBFs have a Value and Generate function.
                // Materials have a ScatteringPdf
                // and a Scatter function that returns a scatter record
                // that includes a pdf.

                // This is feeling pretty Lambert specific - to even use
                // the hitable pdf at all, let alone mixture...
                HitablePDF hitable_pdf(light_shape_raw_ptr, hit_record.p);
                MixturePDF mixture_pdf(&hitable_pdf,
                                       scatter_record.pdf_ptr.get());
                // To neutralize the pdf, the pdf_value needs to be
                // equal to the hit record's material_ptr ScatteringPdf,
                // but I don't know how to make sure they're correlated.
                Ray scattered = Ray(hit_record.p, mixture_pdf.Generate(),
                    r.time());
                float pdf_value = mixture_pdf.Value(scattered.direction(), r.time());
                return emitted
                    + scatter_record.attenuation
                        * hit_record.material_ptr->ScatteringPdf(r, hit_record, scattered)
                        * ColorForRay(scattered, world, light_shape_raw_ptr, depth + 1) / pdf_value;
            } else {
                Ray scattered = Ray(hit_record.p, scatter_record.pdf_ptr->Generate(), r.time());
                float pdf_value =
                    scatter_record.pdf_ptr->Value(scattered.direction(), r.time());
                return emitted
                    + scatter_record.attenuation
                        * hit_record.material_ptr->ScatteringPdf(r, hit_record, scattered)
                        * ColorForRay(scattered, world, light_shape_raw_ptr, depth + 1) / pdf_value;
            }
        } else
            return emitted;
    } else {
        return {0.0f, 0.0f, 0.0f};
    }
}