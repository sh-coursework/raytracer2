//
// Created by Steve Hwan on 10/2/18.
//

#ifndef RAYTRACER1_DIELECTRIC_H
#define RAYTRACER1_DIELECTRIC_H

#include "ray_engine/hitable.h"
#include "material.h"

class Dielectric : public Material {
public:
    explicit Dielectric(float ri) : ref_idx(ri) {}
    bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const override;

    float ref_idx;
};


#endif //RAYTRACER1_DIELECTRIC_H
