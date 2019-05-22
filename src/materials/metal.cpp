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

Vec3 Metal::Attenuation(const HitRecord &hit_record) const {
    return albedo_->Value(hit_record.u, hit_record.v, hit_record.p);
}

std::tuple<Ray, float>
Metal::ScatteringContribution(const RenderContext &render_context, const Ray &r_in, const HitRecord &hit_record) const {
    // No pdf implemented yet
    Vec3 reflected = reflect(unit_vector(r_in.direction()), hit_record.normal);
    return std::make_tuple(Ray(hit_record.p,
                               reflected + fuzz_ * random_in_unit_sphere(),
                               r_in.time()),
                           1.0f);
}

