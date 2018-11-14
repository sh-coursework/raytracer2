//
// Created by Steve Hwan on 11/3/18.
//

#ifndef RAYTRACER2_ISOTROPIC_H
#define RAYTRACER2_ISOTROPIC_H

#include "vec3.h"
#include "textures/texture.h"
#include "materials/material.h"


class Isotropic : public Material {
public:
    explicit Isotropic(Texture *albedo) : albedo_(albedo) {};
    bool Scatter(const Ray &r_in, const HitRecord &rec, Vec3 &attenuation,
            Ray &scattered) const override;


private:
    Texture *albedo_;
};


#endif //RAYTRACER2_ISOTROPIC_H
