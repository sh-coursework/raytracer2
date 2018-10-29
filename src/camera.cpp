//
// Created by Steve Hwan on 10/2/18.
//

#include <cmath>

#include "camera.h"


Vec3 random_in_unit_disk() {
    Vec3 p;
    do {
        p = 2.0 * Vec3(drand48(), drand48(), 0.0f) - Vec3(1.0f, 1.0f, 0.0f);
    } while (dot(p, p) >= 1.0);
    return p;
}

Camera::Camera(Vec3 lookfrom, Vec3 lookat, Vec3 vup, float vfov,
               float aspect, float aperture, float focus_dist,
               float t0, float t1) {  // vfov is top to bottom in degrees
    time0 = t0;
    time1 = t1;

    lens_radius = aperture / 2.0f;
    auto theta = vfov * float(M_PI) / 180.f;
    auto half_height = tanf(theta/2.0f);
    auto half_width = aspect * half_height;

    origin = lookfrom;
    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    lower_left_corner = origin - half_width * focus_dist * u
            - half_height * focus_dist * v - focus_dist * w;
    horizontal = 2.0f * half_width * focus_dist * u;
    vertical = 2.0f * half_height * focus_dist * v;
}

Ray Camera::get_ray(float s, float t) {
    auto rd = lens_radius * random_in_unit_disk();
    auto offset = u * rd.x() + v * rd.y();
    auto time = time0 + float(drand48() * (time1 - time0));
    return {origin + offset,
            lower_left_corner + s * horizontal + t * vertical - origin - offset,
            time};
}