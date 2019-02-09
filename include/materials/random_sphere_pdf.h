//
// Created by svhwan on 2/7/2019.
//

#ifndef RAYTRACER2_RANDOMSPHEREPDF_H
#define RAYTRACER2_RANDOMSPHEREPDF_H

#include "materials/pdf_base.h"
#include "vec3.h"

class RandomSpherePDF : public PDFBase {
 public:
  float Value(const Vec3 &direction) const override;
  Vec3 Generate() const override;
};

#endif //RAYTRACER2_RANDOMSPHEREPDF_H
