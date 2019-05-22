//
// Created by Steve Hwan on 10/2/18.
//

#ifndef RAYTRACER1_LAMBERTIAN_H
#define RAYTRACER1_LAMBERTIAN_H

#include "ray_engine/ray.h"
#include "materials/material.h"
#include "ray_engine/hitable.h"
#include "textures/texture.h"
#include "render_context.h"

class Lambertian : public Material {
 public:
    explicit Lambertian(Texture *albedo) : albedo_(albedo) {}
    float ScatteringPdf(const Ray &r_in, const HitRecord &hit_record,
                        const Ray &scattered) const override;
    // No longer doing scatter record thing
//    bool DoScatter(const Ray &r_in, const HitRecord &hit_record) const override;
    Vec3 Attenuation(const HitRecord &hit_record) const override;
    std::tuple<Ray, float>
    ScatteringContribution(const RenderContext &render_context, const Ray &r_in, const HitRecord &hit_record) const override;
 private:
    Texture *albedo_;
};


#endif //RAYTRACER1_LAMBERTIAN_H
