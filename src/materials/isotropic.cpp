//
// Created by Steve Hwan on 11/3/18.
//

#include "materials/isotropic.h"


bool Isotropic::Scatter(const Ray &r_in, const HitRecord &rec,
        Vec3 &attenuation, Ray &scattered) const {
    scattered = Ray(rec.p, random_in_unit_sphere(), r_in.time());
    attenuation = albedo_->Value(rec.u, rec.v, rec.p);
    return true;
}

