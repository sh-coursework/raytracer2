//
// Created by svhwan on 2/3/2019.
//

#ifndef RAYTRACER2_HITABLEPDF_H
#define RAYTRACER2_HITABLEPDF_H

#include <memory>

#include "pdf_base.h"
#include "ray_engine/hitable.h"
#include "vec3.h"

class HitablePDF : public PDFBase {
 public:
  HitablePDF(const Hitable * const hitable_ptr, const Vec3 &origin)
      : hitable_ptr_(hitable_ptr), origin_(origin) {}
  float Value(const Vec3 &direction, float time) const override;
  Vec3 Generate() const override;
 private:
  Vec3 origin_;
  const Hitable * const hitable_ptr_;
};

#endif //RAYTRACER2_HITABLEPDF_H
