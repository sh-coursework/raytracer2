//
// Created by Steve Hwan on 11/3/18.
//

#ifndef RAYTRACER2_ISOTROPIC_H
#define RAYTRACER2_ISOTROPIC_H

#include "materials/material.h"
#include "ray_engine/ray.h"
#include "textures/texture.h"
#include "vec3.h"

class Isotropic : public Material {
 public:
    explicit Isotropic(Texture *albedo) : albedo_(albedo) {};
    bool Scatter(const Ray &r_in, const HitRecord &hit_record,
                 ScatterRecord &scatter_record) const override;
 private:
    Texture *albedo_;
};

#endif //RAYTRACER2_ISOTROPIC_H
