//
// Created by Steve Hwan on 10/1/18.
//

#ifndef RAYTRACER1_HITABLE_H
#define RAYTRACER1_HITABLE_H

#include "vec3.h"
#include "ray_engine/aabb.h"
#include "ray_engine/ray.h"

class Material;  // avoid circular Hitable / material

// TODO: Do I get a move assign for free?  Note the pointer.
struct HitRecord {
    float t;
    Vec3 p;
    Vec3 normal;
    float u, v;
    Material *material_ptr;
};


class Hitable {
public:
    virtual bool Hit(const Ray &r, float t_min, float t_max,
                     HitRecord &rec) const = 0;
    // Note: BoundingBox really only comes into play in BVHNode.
    virtual bool BoundingBox(float t_min, float t_max, AABB &box) const = 0;
    virtual float PDFValue(const Vec3 &origin, const Vec3 &direction,
                           float time) const
            {return 0.0;}
    virtual Vec3 Random(const Vec3 & origin) const
            {return {1, 0, 0}; }
};


#endif //RAYTRACER1_HITABLE_H
