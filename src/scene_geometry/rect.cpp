//
// Created by Steve Hwan on 10/30/18.
//

#include <cfloat>

#include "scene_geometry/rect.h"

#include "vec3.h"

bool XYRect::Hit(const Ray &r, float t_min, float t_max,
                 HitRecord &rec) const {
    float t = (fixed_val_ - r.origin().z()) / r.direction().z();
    if (t < t_min || t > t_max)
        return false;

    float x = r.origin().x() + t * r.direction().x();
    float y = r.origin().y() + t * r.direction().y();
    if (x < x_min_ || x > x_max_ || y < y_min_ || y > y_max_)
        return false;

    rec.u = (x - x_min_) / (x_max_ - x_min_);
    rec.v = (y - y_min_) / (y_max_ - y_min_);
    rec.t = t;
    rec.material_ptr = material_ptr_;
    rec.p = r.PointAtParameter(t);
    rec.normal = Vec3(0.0f, 0.0f, 1.0f);

    return true;
}

bool XZRect::Hit(const Ray &r, float t_min, float t_max,
                 HitRecord &rec) const {
    float t = (fixed_val_ - r.origin().y()) / r.direction().y();
    if (t < t_min || t > t_max)
        return false;

    float x = r.origin().x() + t * r.direction().x();
    float z = r.origin().z() + t * r.direction().z();
    if (x < x_min_ || x > x_max_ || z < z_min_ || z > z_max_)
        return false;

    rec.u = (x - x_min_) / (x_max_ - x_min_);
    rec.v = (z - z_min_) / (z_max_ - z_min_);
    rec.t = t;
    rec.material_ptr = material_ptr_;
    rec.p = r.PointAtParameter(t);
    rec.normal = Vec3(0.0f, 1.0f, 0.0f);

    return true;
}

float XZRect::PDFValue(const Vec3 &origin, const Vec3 &direction, float time)
        const {
    HitRecord rec;
    if (this->Hit(Ray(origin, direction, time), 0.001f, FLT_MAX, rec)) {
        float area = (x_max_ - x_min_) * (z_max_ - z_min_);
        float distance_squared = rec.t * rec.t * direction.squared_length();
        float cosine = fabs(dot(direction, rec.normal) / direction.length());
        return distance_squared / (cosine * area);
    } else
        return 0.0f;
}
Vec3 XZRect::Random(const Vec3 &origin) const {
    Vec3 random_point = Vec3(x_min_ + drand48() * (x_max_ - x_min_),
        fixed_val_, z_min_ + drand48() * (z_max_ - z_min_));
    return random_point - origin;
}

bool YZRect::Hit(const Ray &r, float t_min, float t_max,
                 HitRecord &rec) const {
    float t = (fixed_val_ - r.origin().x()) / r.direction().x();
    if (t < t_min || t > t_max)
        return false;

    float y = r.origin().y() + t * r.direction().y();
    float z = r.origin().z() + t * r.direction().z();
    if (y < y_min_ || y > y_max_ || z < z_min_ || z > z_max_)
        return false;

    rec.u = (y - y_min_) / (y_max_ - y_min_);
    rec.v = (z - z_min_) / (z_max_ - z_min_);
    rec.t = t;
    rec.material_ptr = material_ptr_;
    rec.p = r.PointAtParameter(t);
    rec.normal = Vec3(1.0f, 0.0f, 0.0f);

    return true;
}

bool XYRect::BoundingBox(float t_min, float t_max, AABB &box) const {
    box = AABB(Vec3(x_min_, y_min_, fixed_val_ - 0.0001f),
               Vec3(x_max_, y_max_, fixed_val_ + 0.0001f));
    return true;
}

bool XZRect::BoundingBox(float t_min, float t_max, AABB &box) const {
    box = AABB(Vec3(x_min_, fixed_val_ - 0.0001f, z_min_),
               Vec3(x_max_, fixed_val_ + 0.0001f, z_max_));
    return true;
}

bool YZRect::BoundingBox(float t_min, float t_max, AABB &box) const {
    box = AABB(Vec3(fixed_val_ - 0.0001f, y_min_, z_min_),
               Vec3(fixed_val_ + 0.0001f, y_max_, z_max_));
    return true;
}


