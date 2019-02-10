//
// Created by Steve Hwan on 10/1/18.
//

#include "scene_geometry/sphere.h"

#include <cfloat>
#include <cmath>

#include "ortho_normal_basis.h"

// Arguably, this could go in a math library (as could Vec3)
void GetSphereUV(const Vec3 &p, float& u, float &v) {
    auto phi = float(atan2(p.z(), p.x()));
    auto theta = float(asin(p.y()));
    u = 1.0 - (phi + M_PI) / (2 * M_PI);
    v = (theta + M_PI/2.0) / M_PI;
}

bool Sphere::Hit(const Ray &r, float t_min, float t_max, HitRecord &rec) const {
    auto oc = r.origin() - center_;
    auto a = dot(r.direction(), r.direction());
    auto b = dot(oc, r.direction());
    auto c = dot(oc, oc) - radius_ * radius_;
    auto discriminant = b * b - a * c;
    if (discriminant > 0) {
        // root 1
        auto temp = (-b - sqrtf(b*b - a*c)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.PointAtParameter(rec.t);
            GetSphereUV((rec.p - center_) / radius_, rec.u, rec.v);
            rec.normal = (rec.p - center_) / radius_;
            rec.material_ptr = material_ptr_;
            return true;
        }
        // root 2
        temp = (-b + sqrtf(b*b - a*c)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.PointAtParameter(rec.t);
            GetSphereUV((rec.p - center_) / radius_, rec.u, rec.v);
            rec.normal = (rec.p - center_) / radius_;
            rec.material_ptr = material_ptr_;
            return true;
        }
    }
    return false;
}

bool Sphere::BoundingBox(float t_min, float t_max, AABB &box) const {
    box = AABB(center_ - Vec3(radius_, radius_, radius_),
               center_ + Vec3(radius_, radius_, radius_));
    return true;
}

float Sphere::PDFValue(const Vec3 &origin, const Vec3 &direction, float time)
        const {
    HitRecord hit_record;
    if (this->Hit(Ray(origin, direction, time), 0.001f, FLT_MAX, hit_record)) {
        auto cos_theta_max = sqrt(1 -
            radius_ * radius_/(center_ - origin).squared_length());
        auto solid_angle = 2.0f * M_PI * (1.0f - cos_theta_max);
        return 1.0f / solid_angle;
    } else {
        return 0.0f;
    }
}

Vec3 Sphere::Random(const Vec3 &origin) const {
    auto direction = center_ - origin;
    float distance_squared = direction.squared_length();
    OrthoNormalBasis uvw;
    uvw.BuildFromW(direction);
    return uvw.Local(random_to_sphere(radius_, distance_squared));
}
