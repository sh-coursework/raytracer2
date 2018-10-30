//
// Created by Steve Hwan on 10/30/18.
//

#ifndef RAYTRACER2_BOX_H
#define RAYTRACER2_BOX_H

#include <vector>

#include "vec3.h"
#include "ray_engine/hitable.h"
#include "ray_engine/hitable_list.h"
#include "ray_engine/aabb.h"


class Box : public Hitable {
public:
    Box() = default;
    Box(const Vec3 &p_min, const Vec3 &p_max, Material *material_ptr);

    bool Hit(const Ray &r, float t_min, float t_max,
             HitRecord &rec) const override;
    bool BoundingBox(float t_min, float t_max, AABB &box) const override;

    Vec3 p_min_, p_max_;
    HitableList *hitables_;
};


#endif //RAYTRACER2_BOX_H
