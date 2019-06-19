//
// Created by Steve Hwan on 10/21/18.
//

#include "ray_engine/ray_engine.h"

#include <cfloat>
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

// 6/12/2019 sh-coursework - removed lots of commented-out code from
// refactoring out ScatterRecord.
Vec3 ColorForRay(const Ray &r, const RenderContext &render_context,
                 int depth) {
    // Added Special 0,0 Ray - stop hit test and return full.
    // (But how expensive is this test on every ColorForRay call?)

    HitRecord hit_record;
    if (render_context.world_ptr->Hit(r, 0.001f, FLT_MAX, hit_record)) {
        Vec3 emitted {0.0f, 0.0f, 0.0f};
        if (dot(hit_record.normal, r.direction()) < 0.0)
            emitted = hit_record.material_ptr->Emitted(hit_record.u,
                hit_record.v, hit_record.p);

        if (depth < 50 && hit_record.material_ptr->DoScatter(r, hit_record)) {
            Ray scatter_ray;
            float coefficient;
            std::tie(scatter_ray, coefficient)
                    = hit_record.material_ptr->ScatteringContribution(render_context, r, hit_record);
            return emitted
                + hit_record.material_ptr->Attenuation(hit_record)
                   * coefficient
                   * ColorForRay(scatter_ray, render_context, depth + 1);
        } else
            return emitted;
    } else {
        // could optionally return a sky shader instead.  Book 1,2 tests assume it.
        // Not sure how that would be implemented though in terms of material.
        if (render_context.skydome_geo_ptr == nullptr) {
          return {0.0f, 0.0f, 0.0f};
        } else {
          if (render_context.skydome_geo_ptr->Hit(r, 0.001f, FLT_MAX, hit_record)) {
            return hit_record.material_ptr->Emitted(hit_record.u,
                                                    hit_record.v, hit_record.p);
          } else {
            return {0.0f, 0.0f, 0.0f};
          }

        }

    }
}