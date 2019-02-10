//
// Created by Steve Hwan on 10/2/18.
//

#include "materials/lambertian.h"

#include <memory>

#include "ortho_normal_basis.h"
#include "materials/cosine_pdf.h"
#include "vec3.h"

bool Lambertian::Scatter(const Ray &r_in, const HitRecord &hit_record,
                         ScatterRecord &scatter_record) const {
    scatter_record.is_specular = false;
    scatter_record.attenuation = albedo_->Value(hit_record.u, hit_record.v,
        hit_record.p);
    scatter_record.do_mixture_pdf = true;
    scatter_record.pdf_ptr = std::unique_ptr<CosinePDF>(
        new CosinePDF(hit_record.normal));
    return true;
}

float Lambertian::ScatteringPdf(const Ray &r_in, const HitRecord &hit_record,
                                const Ray &scattered) const {
    float cosine = dot(hit_record.normal, unit_vector(scattered.direction()));
    if (cosine < 0.0f)
        cosine = 0;
    return cosine / float(M_PI);
}
