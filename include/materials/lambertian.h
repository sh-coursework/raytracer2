//
// Created by Steve Hwan on 10/2/18.
//

#ifndef RAYTRACER1_LAMBERTIAN_H
#define RAYTRACER1_LAMBERTIAN_H

#include "ray_engine/hitable.h"
#include "materials/material.h"
#include "textures/texture.h"

class Lambertian : public Material {
public:
    explicit Lambertian(Texture *a) : albedo_(a) {}
    bool Scatter(const Ray &r_in, const HitRecord &rec, Vec3 &attenuation,
                 Ray &scattered) const override;

    Texture *albedo_;
};


#endif //RAYTRACER1_LAMBERTIAN_H
