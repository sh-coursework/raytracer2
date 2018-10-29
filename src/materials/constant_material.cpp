//
// Created by Steve Hwan on 10/28/18.
//

#include "materials/constant_material.h"


bool ConstantMaterial::scatter(const Ray &r_in, const HitRecord &rec,
                               Vec3 &attenuation, Ray &scattered) const {
    // special 0, 0 Ray - i don't really want to continue.
    scattered = Ray(Vec3(0.0, 0.0, 0.0), Vec3(0.0, 0.0, 0.0), r_in.time());
    attenuation = albedo->value(rec.u, rec.v, rec.p);
    return true;
}
