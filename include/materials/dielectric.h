//
// Created by Steve Hwan on 10/2/18.
//

#ifndef RAYTRACER1_DIELECTRIC_H
#define RAYTRACER1_DIELECTRIC_H

#include "vec3.h"
#include "material.h"
#include "ray_engine/hitable.h"
#include "render_context.h"

class Dielectric : public Material {
 public:
    explicit Dielectric(float ri) : ref_idx_(ri) {}
    // No longer doing scatter record thing
//    bool DoScatter(const Ray &r_in, const HitRecord &hit_record) const override;
    Vec3 Attenuation(const HitRecord &hit_record) const override;
    std::tuple<Ray, float>
    ScatteringContribution(const RenderContext &render_context, const Ray &r_in, const HitRecord &hit_record) const override;
 private:
    float ref_idx_;
};


#endif //RAYTRACER1_DIELECTRIC_H
