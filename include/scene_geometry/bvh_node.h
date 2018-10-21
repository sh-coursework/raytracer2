//
// Created by Steve Hwan on 10/9/18.
//

#ifndef RAYTRACER2_BVH_NODE_H
#define RAYTRACER2_BVH_NODE_H

#include "aabb.h"
#include "hitable.h"

class bvh_node : public hitable {
public:
    bvh_node() {}
    bvh_node(hitable **l, int n, float time0, float time1);
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
    virtual bool bounding_box(float t0, float t1, aabb& box) const;
    hitable *left;
    hitable *right;
    aabb box;
};

int box_x_compare (const void * a, const void * b);
int box_y_compare (const void * a, const void * b);
int box_z_compare (const void * a, const void * b);

#endif //RAYTRACER2_BVH_NODE_H
