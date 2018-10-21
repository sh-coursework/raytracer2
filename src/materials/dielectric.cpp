//
// Created by Steve Hwan on 10/2/18.
//

#include "materials/dielectric.h"

bool dielectric::scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
    vec3 outward_normal;
    auto reflected = reflect(r_in.direction(), rec.normal);
    float ni_over_nt;
    attenuation = vec3(1.0f, 1.0f, 1.0f);  // book said 1,1,0, but it was tinted.
    vec3 refracted;
    float reflect_prob;
    float cosine;
    if (dot(r_in.direction(), rec.normal) > 0.0f) {
        outward_normal = -rec.normal;
        ni_over_nt = ref_idx;
        cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
    } else {
        outward_normal = rec.normal;
        ni_over_nt = 1.0f / ref_idx;
        cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
    }
    if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
        reflect_prob = schlick(cosine, ref_idx);
    } else {
        scattered = ray(rec.p, reflected, r_in.time());
        reflect_prob = 1.0;
    }
    if (drand48() < reflect_prob) {
        scattered = ray(rec.p, reflected, r_in.time());
    } else {
        scattered = ray(rec.p, refracted, r_in.time());
    }
    return true;
}
