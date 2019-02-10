//
// Created by svhwan on 2/7/2019.
//

#ifndef RAYTRACER2_DIELECTRICPDF_H
#define RAYTRACER2_DIELECTRICPDF_H

#include "pdf_base.h"
#include "vec3.h"

class DielectricPDF : public PDFBase {
 public:
  DielectricPDF(float ref_idx,
                Vec3 r_in_direction,
                Vec3 hit_record_normal);
  float Value(const Vec3 &direction, float time) const override;
  Vec3 Generate() const override;
 private:
  float ref_idx_;
  Vec3 r_in_direction_;
  Vec3 hit_record_normal_;
};

#endif //RAYTRACER2_DIELECTRICPDF_H
