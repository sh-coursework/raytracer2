//
// Created by svhwan on 2/3/2019.
//

#include "materials/hitable_pdf.h"

float HitablePDF::Value(const Vec3 &direction) const {
  return hitable_ptr_->PDFValue(origin_, direction);
}

Vec3 HitablePDF::Generate() const {
  return hitable_ptr_->Random(origin_);
}
