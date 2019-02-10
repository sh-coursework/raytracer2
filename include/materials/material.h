//
// Created by Steve Hwan on 10/2/18.
//

#ifndef RAYTRACER1_MATERIAL_H
#define RAYTRACER1_MATERIAL_H

#include <memory>

#include "materials/pdf_base.h"
#include "ray_engine/hitable.h"
#include "ray_engine/ray.h"
#include "vec3.h"

// I don't know why the scatter record always needs to create a fresh
// pdf. Why isn't it just a part of the material with the incoming
// direction just another parameter?
// It feels like the pdf/brdf is a property of the material, not the
// individual scatter-on-hit. Maybe it would be more appropriate to
// at least make it a lambda?  Though that would still mean making a
// pointer-per-scatter, but to the closure, so I'd still need to
// track ownership.
struct ScatterRecord {
  Ray specular_ray;
  bool is_specular;
  Vec3 attenuation;
  bool do_mixture_pdf;  // It's really a hack in lambert.'
  std::unique_ptr<PDFBase> pdf_ptr;
};

// Pure abstract base class
class Material {
public:
    virtual bool Scatter(const Ray &r_in, const HitRecord &hit_record,
                         ScatterRecord &scatter_record) const {
      return false;
    }
    virtual float ScatteringPdf(const Ray &r_in, const HitRecord &hit_record,
                                const Ray &scattered) const {
      return 1.0f;
    }
    virtual Vec3 Emitted(float u, float v, const Vec3& p) const
            { return {0.0f, 0.0f, 0.0f}; }
};


#endif //RAYTRACER1_MATERIAL_H
