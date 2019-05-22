//
// Created by Steve Hwan on 10/2/18.
//

#ifndef RAYTRACER1_METAL_H
#define RAYTRACER1_METAL_H

#include "vec3.h"
#include "materials/material.h"
#include "ray_engine/hitable.h"
#include "ray_engine/ray.h"
#include "textures/texture.h"
#include "render_context.h"

class Metal : public Material {
 public:
    Metal(Texture *a, float f);
    // No longer doing scatter record thing
//    bool DoScatter(const Ray &r_in, const HitRecord &hit_record) const override;
    Vec3 Attenuation(const HitRecord &hit_record) const override;
    std::tuple<Ray, float>
    ScatteringContribution(const RenderContext &render_context, const Ray &r_in, const HitRecord &hit_record) const override;

private:
    Texture *albedo_;
    float fuzz_;
};

#endif //RAYTRACER1_METAL_H
