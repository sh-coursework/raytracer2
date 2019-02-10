//
// Created by svhwan on 2/3/2019.
//

#include "materials/cosine_pdf.h"

float CosinePDF::Value(const Vec3 &direction, float time) const {
  float cosine = dot(unit_vector(direction), uvw_.w());
  if (cosine > 0)
    return cosine / float(M_PI);
  else
    return 0;
}

Vec3 CosinePDF::Generate() const {
  return uvw_.Local(random_cosine_direction());
}
