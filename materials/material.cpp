//
// Created by Steve Hwan on 10/2/18.
//

#include "material.h"

vec3 reflect(const vec3& v, const vec3& n) {
    return v-2*dot(v, n) * n;
}

bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) {
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

