//
// Created by svhwan on 2/3/2019.
//

#ifndef RAYTRACER2_COSINEPDF_H
#define RAYTRACER2_COSINEPDF_H

#include "vec3.h"
#include <materials/pdf_base.h>
#include <ortho_normal_basis.h>

// TODO: It makes me a little uncomfortable that the constructor
// TODO: is only defined here, and not in the base class.
class CosinePDF : public PDFBase {
 public:
  explicit CosinePDF(const Vec3 &w) { uvw_.BuildFromW(w); }
  float Value(const Vec3 &direction) const override;
  Vec3 Generate() const override;

  OrthoNormalBasis uvw_;
};

#endif //RAYTRACER2_COSINEPDF_H
