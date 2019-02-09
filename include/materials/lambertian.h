//
// Created by Steve Hwan on 10/2/18.
//

#ifndef RAYTRACER1_LAMBERTIAN_H
#define RAYTRACER1_LAMBERTIAN_H

#include "materials/material.h"
#include "ray_engine/hitable.h"
#include "ray_engine/ray.h"
#include "textures/texture.h"

class Lambertian : public Material {
 public:
    explicit Lambertian(Texture *albedo) : albedo_(albedo) {}
    float ScatteringPdf(const Ray &r_in, const HitRecord &hit_record,
                        const Ray &scattered) const override;
    bool Scatter(const Ray &r_in, const HitRecord &hit_record,
                 ScatterRecord &scatter_record) const override;
 private:
    Texture *albedo_;
};


#endif //RAYTRACER1_LAMBERTIAN_H
