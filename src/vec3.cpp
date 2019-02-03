//
// Created by Steve Hwan on 10/1/18.
//

#include "vec3.h"


Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v-2*dot(v, n) * n;
}

bool refract(const Vec3& v, const Vec3& n, float ni_over_nt, Vec3& refracted) {
    auto uv = unit_vector(v);
    auto dt = dot(uv, n);
    auto discriminant = 1.0f - ni_over_nt * ni_over_nt * (1.0f - dt * dt);
    if (discriminant > 0) {
        refracted = ni_over_nt * (uv - n * dt) - n * sqrtf(discriminant);
        return true;
    } else {
        return false;
    }
}

float schlick(float cosine, float ref_idx) {
    auto r0 = (1.0f - ref_idx) / (1.0f + ref_idx);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * powf((1.0f - cosine), 5.0f);
}

Vec3 random_in_unit_disk() {
    Vec3 p;
    do {
        p = 2.0 * Vec3(drand48(), drand48(), 0.0f) - Vec3(1.0f, 1.0f, 0.0f);
    } while (dot(p, p) >= 1.0);
    return p;
}

Vec3 random_in_unit_sphere() {
    Vec3 p;
    do {
        p = 2.0f * Vec3(drand48(), drand48(), drand48())
                - Vec3(1.0f, 1.0f, 1.0f);
    } while (p.squared_length() >= 1.0);
    return p;
}

