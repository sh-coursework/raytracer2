//
// Created by Steve Hwan on 10/2/18.
//

#include "materials/dielectric.h"


bool Dielectric::Scatter(const Ray &r_in, const HitRecord &rec,
                         Vec3 &attenuation, Ray &scattered) const {
    Vec3 outward_normal;
    auto reflected = reflect(r_in.direction(), rec.normal);
    float ni_over_nt;
    attenuation = Vec3(1.0f, 1.0f, 1.0f);  // book said 1,1,0, but it was tinted.
    Vec3 refracted;
    float reflect_prob;
    float cosine;
    if (dot(r_in.direction(), rec.normal) > 0.0f) {
        outward_normal = -rec.normal;
        ni_over_nt = ref_idx_;
        cosine = ref_idx_ * dot(r_in.direction(), rec.normal)
                 / r_in.direction().length();
    } else {
        outward_normal = rec.normal;
        ni_over_nt = 1.0f / ref_idx_;
        cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
    }
    if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
        reflect_prob = schlick(cosine, ref_idx_);
    } else {
        scattered = Ray(rec.p, reflected, r_in.time());
        reflect_prob = 1.0;
    }
    if (drand48() < reflect_prob) {
        scattered = Ray(rec.p, reflected, r_in.time());
    } else {
        scattered = Ray(rec.p, refracted, r_in.time());
    }
    return true;
}
