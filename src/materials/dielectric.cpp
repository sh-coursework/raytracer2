//
// Created by Steve Hwan on 10/2/18.
//

#include "materials/dielectric.h"

#include <memory>

#include "materials/dielectric_pdf.h"

// TODO: pdf is just a placeholder in dielectric
bool Dielectric::Scatter(const Ray &r_in, const HitRecord &hit_record, ScatterRecord &scatter_record) const {
    scatter_record.is_specular = false;
    scatter_record.attenuation = Vec3(1.0f, 1.0f, 1.0f);  // book said 1,1,0,
                                                          // but it was tinted.
    scatter_record.do_mixture_pdf = false;
    scatter_record.pdf_ptr = std::unique_ptr<DielectricPDF>(
        new DielectricPDF(ref_idx_, r_in.direction(),
                          hit_record.normal));
    return true;
}
