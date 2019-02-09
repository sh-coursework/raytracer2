//
// Created by Steve Hwan on 11/3/18.
//

#include "materials/isotropic.h"

#include "materials/random_sphere_pdf.h"

// TODO: pdf is just a placeholder in isotropic
bool Isotropic::Scatter(const Ray &r_in, const HitRecord &hit_record,
                        ScatterRecord &scatter_record) const {
    scatter_record.is_specular = false;
    scatter_record.attenuation = albedo_->Value(hit_record.u, hit_record.v,
        hit_record.p);
    scatter_record.do_mixture_pdf = false;
    scatter_record.pdf_ptr = new RandomSpherePDF();
    return true;
}

