//
// Created by Steve Hwan on 10/2/18.
//

#ifndef RAYTRACER1_METAL_H
#define RAYTRACER1_METAL_H

#include "vec3.h"
#include "ray_engine/ray.h"
#include "ray_engine/hitable.h"
#include "materials/material.h"
#include "textures/texture.h"

class Metal : public Material {
public:
    Metal(Texture *a, float f);
    bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation,
                 Ray& scattered) const override;

    Texture *albedo;
    float fuzz;
};


#endif //RAYTRACER1_METAL_H
