//
// Created by Steve Hwan on 10/2/18.
//

#ifndef RAYTRACER1_DIELECTRIC_H
#define RAYTRACER1_DIELECTRIC_H

#include "material.h"
#include "ray_engine/hitable.h"
#include "vec3.h"

class Dielectric : public Material {
 public:
    explicit Dielectric(float ri) : ref_idx_(ri) {}
    bool Scatter(const Ray &r_in, const HitRecord &hit_record,
                 ScatterRecord &scatter_record) const override;
 private:
    float ref_idx_;
};


#endif //RAYTRACER1_DIELECTRIC_H
