//
// Created by Steve Hwan on 10/2/18.
//

#ifndef RAYTRACER1_DIELECTRIC_H
#define RAYTRACER1_DIELECTRIC_H

#include "ray_engine/hitable.h"
#include "material.h"

class dielectric : public material {
public:
    explicit dielectric(float ri) : ref_idx(ri) {}
    bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override;

    float ref_idx;
};


#endif //RAYTRACER1_DIELECTRIC_H
