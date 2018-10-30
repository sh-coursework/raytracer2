//
// Created by Steve Hwan on 10/2/18.
//

#include "vec3.h"
#include "materials/lambertian.h"

bool Lambertian::Scatter(const Ray &r_in, const HitRecord &rec,
                         Vec3 &attenuation, Ray &scattered) const {
    auto target = rec.p + rec.normal + random_in_unit_sphere();
    scattered = Ray(rec.p, target - rec.p, r_in.time());
    attenuation = albedo_->Value(rec.u, rec.v, rec.p);
    return true;
}
