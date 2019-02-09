//
// Created by svhwan on 2/3/2019.
//

#include "materials/mixture_pdf.h"

#include <cmath>

float MixturePDF::Value(const Vec3 &direction) const {
  return 0.5f * pdfs_[0]->Value(direction) + 0.5f * pdfs_[1]->Value(direction);
}

Vec3 MixturePDF::Generate() const {
  if (drand48() < 0.5)
    return pdfs_[0]->Generate();
  else
    return pdfs_[1]->Generate();
}
