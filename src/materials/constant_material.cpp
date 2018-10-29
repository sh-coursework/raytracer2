//
// Created by Steve Hwan on 10/28/18.
//

#include "materials/constant_material.h"


bool ConstantMaterial::scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const
{
    // special 0, 0 ray - i don't really want to continue.
    scattered = ray(vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), r_in.time());
    attenuation = albedo->value(rec.u, rec.v, rec.p);
    return true;
}
