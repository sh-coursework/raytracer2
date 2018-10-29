//
// Created by Steve Hwan on 10/2/18.
//

#ifndef RAYTRACER1_METAL_H
#define RAYTRACER1_METAL_H

#include "ray_engine/hitable.h"
#include "materials/material.h"
#include "textures/texture.h"

class metal : public material {
public:
    metal(Texture *a, float f);
    bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override;

    Texture *albedo;
    float fuzz;
};


#endif //RAYTRACER1_METAL_H
