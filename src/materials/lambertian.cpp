//
// Created by Steve Hwan on 10/2/18.
//

#include "vec3.h"
#include "ortho_normal_basis.h"

#include "materials/lambertian.h"

bool Lambertian::Scatter(const Ray &r_in, const HitRecord &rec,
                         Vec3 &attenuation, Ray &scattered,
                         float &pdf) const {
    OrthoNormalBasis uvw;
    uvw.BuildFromW(rec.normal);
    Vec3 direction = uvw.Local(random_cosine_direction());
    scattered = Ray(rec.p, unit_vector(direction), r_in.time());
    attenuation = albedo_->Value(rec.u, rec.v, rec.p);
    pdf = dot(uvw.w(), scattered.direction()) / float(M_PI);
    return true;
}

float Lambertian::ScatteringPdf(const Ray &r_in, const HitRecord &rec,
                    const Ray &scattered) const {
    float cosine = dot(rec.normal, unit_vector(scattered.direction()));
    if (cosine < 0.0f)
        cosine = 0;
    return cosine / float(M_PI);
}
