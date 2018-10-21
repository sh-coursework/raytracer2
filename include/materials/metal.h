//
// Created by Steve Hwan on 10/2/18.
//

#ifndef RAYTRACER1_METAL_H
#define RAYTRACER1_METAL_H

#include "ray_engine/hitable.h"
#include "material.h"

class metal : public material {
public:
//    metal(const vec3& a, float f) : albedo(a) { if {f < }}
    metal(const vec3& a, float f);
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const;

    vec3 albedo;
    float fuzz;
};


#endif //RAYTRACER1_METAL_H
