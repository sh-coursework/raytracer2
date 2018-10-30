//
// Created by Steve Hwan on 10/2/18.
//

#include <cmath>

#include "camera.h"


Camera::Camera(Vec3 lookfrom, Vec3 lookat, Vec3 vup, float vfov,
               float aspect, float aperture, float focus_dist,
               float t0, float t1) {  // vfov is top to bottom in degrees
    time_0_ = t0;
    time_1_ = t1;

    lens_radius_ = aperture / 2.0f;
    auto theta = vfov * float(M_PI) / 180.f;
    auto half_height = tanf(theta/2.0f);
    auto half_width = aspect * half_height;

    origin_ = lookfrom;
    w_ = unit_vector(lookfrom - lookat);
    u_ = unit_vector(cross(vup, w_));
    v_ = cross(w_, u_);

    lower_left_corner_ = origin_ - half_width * focus_dist * u_
            - half_height * focus_dist * v_ - focus_dist * w_;
    horizontal_ = 2.0f * half_width * focus_dist * u_;
    vertical_ = 2.0f * half_height * focus_dist * v_;
}

Ray Camera::GetRay(float s, float t) {
    auto rd = lens_radius_ * random_in_unit_disk();
    auto offset = u_ * rd.x() + v_ * rd.y();
    auto time = time_0_ + float(drand48() * (time_1_ - time_0_));
    return {origin_ + offset,
            lower_left_corner_ + s * horizontal_ + t * vertical_ - origin_ - offset,
            time};
}