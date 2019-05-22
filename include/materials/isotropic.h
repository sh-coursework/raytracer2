//
// Created by Steve Hwan on 11/3/18.
//

#ifndef RAYTRACER2_ISOTROPIC_H
#define RAYTRACER2_ISOTROPIC_H

#include "vec3.h"
#include "materials/material.h"
#include "ray_engine/ray.h"
#include "textures/texture.h"
#include "render_context.h"

class Isotropic : public Material {
 public:
    explicit Isotropic(Texture *albedo) : albedo_(albedo) {};
    // No longer doing scatter record thing
//    bool DoScatter(const Ray &r_in, const HitRecord &hit_record) const override;
    Vec3 Attenuation(const HitRecord &hit_record) const override;
    std::tuple<Ray, float>
    ScatteringContribution(const RenderContext &render_context, const Ray &r_in, const HitRecord &hit_record) const override;
 private:
    Texture *albedo_;
};

#endif //RAYTRACER2_ISOTROPIC_H
