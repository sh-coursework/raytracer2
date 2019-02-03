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
                         Vec3 &attenuation, Ray &scattered,
                         float &pdf) const {
      return false;
    }
    virtual float ScatteringPdf(const Ray &r_in, const HitRecord &rec,
        const Ray &scattered) const {
      return 1.0f;
    }
    virtual Vec3 Emitted(float u, float v, const Vec3& p) const
            { return {0.0f, 0.0f, 0.0f}; }
};


#endif //RAYTRACER1_MATERIAL_H
