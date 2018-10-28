//
// Created by Steve Hwan on 10/2/18.
//

#include "materials/metal.h"


metal::metal(Texture *a, float f) : albedo(a) {
    if (f < 1)
        fuzz = f;
    else
        fuzz = 1;
}

bool metal::scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
    vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
    scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere(), r_in.time());
    attenuation = albedo->value(rec.u, rec.v, rec.p);
    return (dot(scattered.direction(), rec.normal) > 0.0f);
}
