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

bool Metal::Scatter(const Ray &r_in, const HitRecord &rec, Vec3 &attenuation,
                    Ray &scattered) const {
    Vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
    scattered = Ray(rec.p, reflected + fuzz_ * random_in_unit_sphere(),
                    r_in.time());
    attenuation = albedo_->Value(rec.u, rec.v, rec.p);
    return (dot(scattered.direction(), rec.normal) > 0.0f);
}
