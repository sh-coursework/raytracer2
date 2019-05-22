//
// Created by Steve Hwan on 10/29/18.
//

#ifndef RAYTRACER2_DIFFUSE_LIGHT_H
#define RAYTRACER2_DIFFUSE_LIGHT_H

#include "vec3.h"
#include "textures/texture.h"
#include "materials/material.h"
#include "render_context.h"

class DiffuseLight : public Material {
 public:
    explicit DiffuseLight(Texture *emission) : emission_(emission) {};
    bool DoScatter(const Ray &r_in, const HitRecord &hit_record) const override
            { return false; }
    Vec3 Emitted(float u, float v, const Vec3 &p) const override
            { return emission_->Value(u, v, p); }
 private:
    Texture *emission_;
};


#endif //RAYTRACER2_DIFFUSE_LIGHT_H
