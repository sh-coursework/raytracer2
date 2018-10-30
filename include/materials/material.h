//
// Created by Steve Hwan on 10/2/18.
//

#ifndef RAYTRACER1_MATERIAL_H
#define RAYTRACER1_MATERIAL_H


#include "ray_engine/hitable.h"


// Pure abstract base class
class Material {
public:
    virtual bool Scatter(const Ray &r_in, const HitRecord &rec,
                         Vec3 &attenuation, Ray &scattered) const = 0;
};


#endif //RAYTRACER1_MATERIAL_H
