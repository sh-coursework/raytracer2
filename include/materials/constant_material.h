//
// Created by Steve Hwan on 10/28/18.
//

#ifndef RAYTRACER2_CONSTANT_MATERIAL_H
#define RAYTRACER2_CONSTANT_MATERIAL_H

#include "materials/material.h"
#include "ray_engine/hitable.h"
#include "ray_engine/ray.h"
#include "textures/texture.h"

class ConstantMaterial : public Material {
 public:
    explicit ConstantMaterial(Texture *a) : albedo_(a) {}
    bool Scatter(const Ray &r_in, const HitRecord &hit_record,
                 ScatterRecord scatter_record) const override;
 private:
    Texture *albedo_;
};

#endif //RAYTRACER2_CONSTANT_MATERIAL_H
