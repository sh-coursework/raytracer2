//
// Created by Steve Hwan on 10/2/18.
//

#ifndef RAYTRACER1_MATERIAL_H
#define RAYTRACER1_MATERIAL_H

#include "materials/pdf_base.h"
#include "ray_engine/hitable.h"
#include "ray_engine/ray.h"
#include "vec3.h"

struct ScatterRecord {
  Ray specular_ray;
  bool is_specular;
  Vec3 attenuation;
  bool do_mixture_pdf;  // It's really a hack in lambert.'
  PDFBase *pdf_ptr;  // TODO: find a way to make these smart pointers
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
