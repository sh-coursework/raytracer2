//
// Created by svhwan on 2/3/2019.
//

#ifndef RAYTRACER2_PDFBASE_H
#define RAYTRACER2_PDFBASE_H

#include "vec3.h"

class PDFBase {
 public:
  virtual float Value(const Vec3 &direction) const = 0;
  virtual Vec3 Generate() const = 0;
};

#endif //RAYTRACER2_PDFBASE_H
