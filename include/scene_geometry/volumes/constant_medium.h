//
// Created by Steve Hwan on 11/3/18.
//

#ifndef RAYTRACER2_CONSTANT_MEDIUM_H
#define RAYTRACER2_CONSTANT_MEDIUM_H

#include "ray_engine/aabb.h"
#include "ray_engine/ray.h"
#include "ray_engine/hitable.h"

#include "textures/texture.h"
#include "materials/isotropic.h"


class ConstantMedium : public Hitable {
public:
    ConstantMedium(Hitable *boundary, float density, Texture *medium)
            : boundary_(boundary), density_(density)
        { phase_function_ = new Isotropic(medium); };

    bool Hit(const Ray &r, float t_min, float t_max,
            HitRecord &rec) const override;
    bool BoundingBox(float t_min, float t_max, AABB &box) const override;

    Hitable *boundary_;
    float density_;
    Material *phase_function_;
};


#endif //RAYTRACER2_CONSTANT_MEDIUM_H
