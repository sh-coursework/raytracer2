//
// Created by Steve Hwan on 10/2/18.
//

#include "materials/dielectric.h"

#include <memory>

#include "materials/dielectric_pdf.h"


Vec3 Dielectric::Attenuation(const HitRecord &hit_record) const {
    return {1.0f, 1.0f, 1.0f};  // book said 1,1,0,
    // but it was tinted.
}

std::tuple<Ray, float> Dielectric::ScatteringContribution(const RenderContext &render_context, const Ray &r_in,
                                                          const HitRecord &hit_record) const {
    // TODO: pdf is just a placeholder in dielectric
    auto pdf = DielectricPDF(ref_idx_, r_in.direction(), hit_record.normal);
    auto scattered = Ray(hit_record.p, pdf.Generate(), r_in.time());
    float coefficient = ScatteringPdf(r_in, hit_record, scattered)
            / pdf.Value(scattered.direction(), r_in.time());

    return std::make_tuple(scattered, coefficient);
}

