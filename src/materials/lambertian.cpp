//
// Created by Steve Hwan on 10/2/18.
//

#include "vec3.h"
#include "materials/lambertian.h"

bool Lambertian::Scatter(const Ray &r_in, const HitRecord &rec,
                         Vec3 &attenuation, Ray &scattered,
                         float &pdf) const {
//    auto target = rec.p + rec.normal + random_in_unit_sphere();
////    scattered = Ray(rec.p, target - rec.p, r_in.time());
//    scattered = Ray(rec.p, unit_vector(target - rec.p), r_in.time());
//    attenuation = albedo_->Value(rec.u, rec.v, rec.p);
//    pdf = dot(rec.normal, scattered.direction()) / float(M_PI);

    Vec3 direction;
    do {
        direction = random_in_unit_sphere();
    } while (dot(direction, rec.normal) < 0 );
    scattered = Ray(rec.p, unit_vector(direction), r_in.time());
    attenuation = albedo_->Value(rec.u, rec.v, rec.p);
    pdf = 0.5f / float(M_PI);
    return true;
}

float Lambertian::ScatteringPdf(const Ray &r_in, const HitRecord &rec,
                    const Ray &scattered) const {
    float cosine = dot(rec.normal, unit_vector(scattered.direction()));
    if (cosine < 0.0f)
        cosine = 0;
    return cosine / float(M_PI);
}
