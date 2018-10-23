//
// Created by Steve Hwan on 10/2/18.
//

#ifndef RAYTRACER1_LAMBERTIAN_H
#define RAYTRACER1_LAMBERTIAN_H

#include "ray_engine/hitable.h"
#include "materials/material.h"
#include "textures/texture.h"

class lambertian : public material {
public:
    lambertian(Texture *a) : albedo(a) {}
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const;

    Texture *albedo;
};


#endif //RAYTRACER1_LAMBERTIAN_H
