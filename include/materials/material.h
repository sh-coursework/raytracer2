//
// Created by Steve Hwan on 10/2/18.
//

#ifndef RAYTRACER1_MATERIAL_H
#define RAYTRACER1_MATERIAL_H


#include "ray_engine/hitable.h"


class Material {
public:
    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const = 0;
};


Vec3 reflect(const Vec3& v, const Vec3& n);
bool refract(const Vec3& v, const Vec3& n, float ni_over_nt, Vec3& refracted);
float schlick(float cosine, float ref_idx);


#endif //RAYTRACER1_MATERIAL_H
