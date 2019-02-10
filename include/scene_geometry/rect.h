//
// Created by Steve Hwan on 10/30/18.
//

#ifndef RAYTRACER2_RECT_H
#define RAYTRACER2_RECT_H

#include "ray_engine/ray.h"
#include "ray_engine/hitable.h"
#include "ray_engine/aabb.h"
#include "materials/material.h"


class XYRect : public Hitable {
public:
    XYRect() = default;
    XYRect(float x_min, float x_max, float y_min, float y_max, float fixed_val,
            Material *material_ptr)
                    : x_min_(x_min), x_max_(x_max), y_min_(y_min), y_max_(y_max),
                      fixed_val_(fixed_val), material_ptr_(material_ptr) {};
    bool Hit(const Ray &r, float t_min, float t_max,
             HitRecord &rec) const override;
    bool BoundingBox(float t_min, float t_max, AABB &box) const override;

    Material *material_ptr_;
    float x_min_, x_max_, y_min_, y_max_;
    float fixed_val_;
};


class XZRect : public Hitable {
public:
    XZRect() = default;
    XZRect(float x_min, float x_max, float z_min, float z_max, float fixed_val,
           Material *material_ptr)
            : x_min_(x_min), x_max_(x_max), z_min_(z_min), z_max_(z_max),
              fixed_val_(fixed_val), material_ptr_(material_ptr) {};
    bool Hit(const Ray &r, float t_min, float t_max,
             HitRecord &rec) const override;
    bool BoundingBox(float t_min, float t_max, AABB &box) const override;
  float PDFValue(const Vec3 &origin, const Vec3 &direction, float time)
    const override;
  Vec3 Random(const Vec3 &origin) const override;

  Material *material_ptr_;
    float x_min_, x_max_, z_min_, z_max_;
    float fixed_val_;
};


class YZRect : public Hitable {
public:
    YZRect() = default;
    YZRect(float y_min, float y_max, float z_min, float z_max, float fixed_val,
           Material *material_ptr)
            : y_min_(y_min), y_max_(y_max), z_min_(z_min), z_max_(z_max),
              fixed_val_(fixed_val), material_ptr_(material_ptr) {};
    bool Hit(const Ray &r, float t_min, float t_max,
             HitRecord &rec) const override;
    bool BoundingBox(float t_min, float t_max, AABB &box) const override;

    Material *material_ptr_;
    float y_min_, y_max_, z_min_, z_max_;
    float fixed_val_;
};


#endif //RAYTRACER2_RECT_H
