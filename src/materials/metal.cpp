//
// Created by Steve Hwan on 10/2/18.
//

#include "materials/metal.h"

Metal::Metal(Texture *a, float f) : albedo_(a) {
    if (f < 1)
        fuzz_ = f;
    else
        fuzz_ = 1;
}

bool Metal::Scatter(const Ray &r_in, const HitRecord &hit_record,
                    ScatterRecord &scatter_record) const {
    Vec3 reflected = reflect(unit_vector(r_in.direction()), hit_record.normal);
    scatter_record.specular_ray = Ray(hit_record.p,
                                      reflected + fuzz_ * random_in_unit_sphere(),
                                      r_in.time());
    scatter_record.attenuation = albedo_->Value(hit_record.u, hit_record.v,
                                                hit_record.p);
    scatter_record.is_specular = true;
    scatter_record.pdf_ptr = nullptr;
    return true;
}
