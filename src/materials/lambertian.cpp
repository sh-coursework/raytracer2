//
// Created by Steve Hwan on 10/2/18.
//

#include "materials/lambertian.h"

#include <memory>
#include <render_context.h>

#include "ortho_normal_basis.h"
#include "materials/cosine_pdf.h"
#include <materials/hitable_pdf.h>
#include <materials/mixture_pdf.h>
#include "vec3.h"

float Lambertian::ScatteringPdf(const Ray &r_in, const HitRecord &hit_record,
                                const Ray &scattered) const {
    float cosine = dot(hit_record.normal, unit_vector(scattered.direction()));
    if (cosine < 0.0f)
        cosine = 0;
    return cosine / float(M_PI);
}

Vec3 Lambertian::Attenuation(const HitRecord &hit_record) const {
    return albedo_->Value(hit_record.u, hit_record.v, hit_record.p);
}

std::tuple<Ray, float> Lambertian::ScatteringContribution(const RenderContext &render_context,
                                                          const Ray &r_in,
                                                          const HitRecord &hit_record) const {
    auto cosine_pdf = CosinePDF(hit_record.normal);

    // Uncomment to use mixture_pdf
//    HitablePDF hitable_pdf(render_context.hitable_light_list_ptr.get(), hit_record.p);
//    MixturePDF mixture_pdf(&hitable_pdf, &cosine_pdf);
//    Ray scattered = Ray(hit_record.p, mixture_pdf.Generate(), r_in.time());
//    float coefficient =
//            ScatteringPdf(r_in, hit_record, scattered)
//            / mixture_pdf.Value(scattered.direction(), r_in.time());

    auto scattered = Ray(hit_record.p, cosine_pdf.Generate(), r_in.time());
    float coefficient =
            ScatteringPdf(r_in, hit_record, scattered)
            / cosine_pdf.Value(scattered.direction(), r_in.time());
    return std::make_tuple(scattered, coefficient);
}
