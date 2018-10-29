//
// Created by Steve Hwan on 10/2/18.
//

#include "materials/metal.h"


Metal::Metal(Texture *a, float f) : albedo(a) {
    if (f < 1)
        fuzz = f;
    else
        fuzz = 1;
}

bool Metal::scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation,
                    Ray& scattered) const {
    Vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
    scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere(),
                    r_in.time());
    attenuation = albedo->value(rec.u, rec.v, rec.p);
    return (dot(scattered.direction(), rec.normal) > 0.0f);
}
