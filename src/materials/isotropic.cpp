//
// Created by Steve Hwan on 11/3/18.
//

#include "materials/isotropic.h"

#include <memory>

#include "materials/random_sphere_pdf.h"

Vec3 Isotropic::Attenuation(const HitRecord &hit_record) const {
    return albedo_->Value(hit_record.u, hit_record.v, hit_record.p);
}

std::tuple<Ray, float>
Isotropic::ScatteringContribution(const RenderContext &render_context, const Ray &r_in, const HitRecord &hit_record) const {
    // TODO: pdf is just a placeholder in isotropic
    auto pdf = RandomSpherePDF();
    auto scattered = Ray(hit_record.p, pdf.Generate(), r_in.time());
    float coefficient = ScatteringPdf(r_in, hit_record, scattered)
                        / pdf.Value(scattered.direction(), r_in.time());

    return std::make_tuple(scattered, coefficient);
}

