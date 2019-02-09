//
// Created by Steve Hwan on 10/2/18.
//

#ifndef RAYTRACER1_METAL_H
#define RAYTRACER1_METAL_H

#include "materials/material.h"
#include "ray_engine/hitable.h"
#include "ray_engine/ray.h"
#include "textures/texture.h"
#include "vec3.h"

class Metal : public Material {
 public:
    Metal(Texture *a, float f);
    bool Scatter(const Ray &r_in, const HitRecord &hit_record,
                 ScatterRecord &scatter_record) const override;
 private:
    Texture *albedo_;
    float fuzz_;
};

#endif //RAYTRACER1_METAL_H
