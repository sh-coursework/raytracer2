//
// Created by Steve Hwan on 10/28/18.
//

#ifndef RAYTRACER2_CONSTANT_MATERIAL_H
#define RAYTRACER2_CONSTANT_MATERIAL_H


#include "ray_engine/hitable.h"
#include "materials/material.h"
#include "textures/texture.h"


class ConstantMaterial : public Material {
public:
    explicit ConstantMaterial(Texture *a) : albedo(a) {}
    bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const override;

    Texture *albedo;
};


#endif //RAYTRACER2_CONSTANT_MATERIAL_H
