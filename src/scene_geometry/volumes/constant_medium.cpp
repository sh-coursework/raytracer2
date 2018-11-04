//
// Created by Steve Hwan on 11/3/18.
//

#include "scene_geometry/volumes/constant_medium.h"

#include <cfloat>
#include <cmath>
#include <iostream>

#include "ray_engine/hitable.h"


bool ConstantMedium::Hit(const Ray &r, float t_min, float t_max, HitRecord &rec) const {
    auto db = (drand48() < 0.00001);  // I think this was setting up for something
    db = false;  // instead of mostly false, now always false.
    HitRecord rec_1, rec_2;
    if (boundary_->Hit(r, -FLT_MAX, FLT_MAX, rec_1)) {
        if (boundary_->Hit(r, rec_1.t + 0.0001f, FLT_MAX, rec_2)) {
            if (db)
                std::cerr << "\nt0 t1 " << rec_1.t << " " << rec_2.t << "\n";
            if (rec_1.t < t_min)
                rec_1.t = t_min;
            if (rec_2.t > t_max)
                rec_2.t = t_max;
            if (rec_1.t >= rec_2.t)
                return false;
            if (rec_1.t < 0)
                rec_1.t = 0;
            auto distance_inside_bondary = (rec_2.t - rec_1.t)
                    * r.direction().length();
            auto hit_distance = -(1/density_) * float(log(drand48()));
            if (hit_distance < distance_inside_bondary) {
                if (db)
                    std::cerr << "hit_distance = " << hit_distance << "\n";
                rec.t = rec_1.t + hit_distance / r.direction().length();
                if (db)
                    std::cerr << "rec.t = " << rec.t << "\n";
                rec.p = r.PointAtParameter(rec.t);
                if (db)
                    std::cerr << "rec.p = " << rec.p << "\n";
                rec.normal = Vec3(1.0f, 0.0f, 0.0f);  // arbitrary
                rec.material_ptr = phase_function_;
                return true;
            }
        }
    }

    return false;
}

bool ConstantMedium::BoundingBox(float t_min, float t_max, AABB &box) const {
    return boundary_->BoundingBox(t_min, t_max, box);
}

