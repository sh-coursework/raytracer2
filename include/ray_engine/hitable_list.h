//
// Created by Steve Hwan on 10/1/18.
//

#ifndef RAYTRACER1_HITABLE_LIST_H
#define RAYTRACER1_HITABLE_LIST_H

#include "ray_engine/hitable.h"
#include "ray_engine/aabb.h"

class hitable_list : public hitable {
public:
    hitable_list() {}
    hitable_list(hitable **l, int n) {list = l; list_size = n; }
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
    virtual bool bounding_box(float t0, float t1, aabb& box) const;
    hitable **list;
    int list_size;
};


#endif //RAYTRACER1_HITABLE_LIST_H
