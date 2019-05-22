//
// Created by Steve Hwan on 10/2/18.
//

#ifndef RAYTRACER1_MATERIAL_H
#define RAYTRACER1_MATERIAL_H

#include <memory>
#include <tuple>

#include "vec3.h"
#include "ray_engine/ray.h"
#include "materials/pdf_base.h"
#include "ray_engine/hitable.h"
#include "render_context.h"

// 5/21/2019 sh-coursework - removed ScatterRecord,
// making that part of the material so it won't need
// to allocate/destroy a unique_ptr on every ray bounce.

// Pure abstract base class
// ScatteringPdf is like distributing incident light.
// ScatteringRecord pdf_ptr above is to compensate
// for the sampling distribution.
class Material {
public:
    virtual bool DoScatter(const Ray &r_in, const HitRecord &hit_record) const {
      return true;
    }
    virtual float ScatteringPdf(const Ray &r_in, const HitRecord &hit_record,
                                const Ray &scattered) const {
      return 1.0f;
    }
    virtual Vec3 Attenuation(const HitRecord &hit_record) const {
      return {1.0f, 1.0f, 1.0f};
    }
    virtual std::tuple<Ray, float>
    ScatteringContribution(const RenderContext &render_context, const Ray &r_in, const HitRecord &hit_record) const {
        return std::make_tuple(Ray(), 1.0f);
    }

    virtual Vec3 Emitted(float u, float v, const Vec3& p) const
            { return {0.0f, 0.0f, 0.0f}; }
};


#endif //RAYTRACER1_MATERIAL_H
