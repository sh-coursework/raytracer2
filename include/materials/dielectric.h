//
// Created by Steve Hwan on 10/2/18.
//

#ifndef RAYTRACER1_DIELECTRIC_H
#define RAYTRACER1_DIELECTRIC_H

#include "vec3.h"
#include "ray_engine/hitable.h"
#include "material.h"

class Dielectric : public Material {
public:
    explicit Dielectric(float ri) : ref_idx_(ri) {}
    bool Scatter(const Ray &r_in, const HitRecord &rec, Vec3 &attenuation,
                 Ray &scattered, float &pdf) const override;

    float ref_idx_;
};


#endif //RAYTRACER1_DIELECTRIC_H
