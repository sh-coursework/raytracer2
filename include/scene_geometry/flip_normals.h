//
// Created by Steve Hwan on 10/30/18.
//

#ifndef RAYTRACER2_FLIP_NORMALS_H
#define RAYTRACER2_FLIP_NORMALS_H


#include "ray_engine/hitable.h"


class FlipNormals : public Hitable {
public:
    FlipNormals(Hitable *orig_hitable_ptr)
            : orig_hitable_ptr_(orig_hitable_ptr) {}

    bool Hit(const Ray &r, float t_min, float t_max,
             HitRecord &rec) const override;
    bool BoundingBox(float t_min, float t_max, AABB &box) const override;

    Hitable *orig_hitable_ptr_;
};


#endif //RAYTRACER2_FLIP_NORMALS_H
