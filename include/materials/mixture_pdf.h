//
// Created by svhwan on 2/3/2019.
//

#ifndef RAYTRACER2_MIXTUREPDF_H
#define RAYTRACER2_MIXTUREPDF_H

#include "pdf_base.h"
#include "vec3.h"

// TODO: Seriously, audit the pointer on this.

class MixturePDF : public PDFBase {
 public:
  MixturePDF(PDFBase *pdf_0, PDFBase *pdf_1)
    {pdfs_[0] = pdf_0; pdfs_[1] = pdf_1; }
  float Value(const Vec3 &direction) const override;
  Vec3 Generate() const override;
 private:
  PDFBase * pdfs_[2];
};

#endif //RAYTRACER2_MIXTUREPDF_H
