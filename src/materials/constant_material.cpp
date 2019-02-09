//
// Created by Steve Hwan on 10/28/18.
//

#include "materials/constant_material.h"


bool ConstantMaterial::Scatter(const Ray &r_in, const HitRecord &hit_record,
                               ScatterRecord scatter_record) const {
    // special 0, 0 Ray - i don't really want to continue.
    scatter_record.is_specular = false;
    scattered = Ray(Vec3(0.0, 0.0, 0.0), Vec3(0.0, 0.0, 0.0), r_in.time());
    scatter_record.attenuation = albedo_->Value(hit_record.u, hit_record.v,
                                                hit_record.p);
    scatter_record.do_mixture_pdf = false;
    return true;
}
