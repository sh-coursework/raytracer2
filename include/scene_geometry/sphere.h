//
// Created by Steve Hwan on 10/1/18.
//

#ifndef RAYTRACER1_SPHERE_H
#define RAYTRACER1_SPHERE_H

#include "materials/material.h"
#include "ray_engine/aabb.h"
#include "ray_engine/hitable.h"
#include "ray_engine/ray.h"
#include "vec3.h"

class Sphere : public Hitable {
 public:
  Sphere() = default;
  Sphere(Vec3 cen, float r, Material *m) : center_(cen), radius_(r),
                                             material_ptr_(m) {};
  bool Hit(const Ray &r, float t_min, float t_max,
           HitRecord &rec) const override;
  bool BoundingBox(float t_min, float t_max, AABB &box) const override;
  float PDFValue(const Vec3 &origin, const Vec3 &direction, float time)
    const override;
  Vec3 Random(const Vec3 &origin) const override;
 private:
  Vec3 center_;
  float radius_;
  Material *material_ptr_;  // I think I actually own the material ptr,
                              // but I don't really want to.
};

void GetSphereUV(const Vec3 &p, float& u, float &v);


#endif //RAYTRACER1_SPHERE_H
