//
// Created by Steve Hwan on 10/2/18.
//

#ifndef RAYTRACER1_CAMERA_H
#define RAYTRACER1_CAMERA_H

#include "vec3.h"
#include "ray_engine/ray.h"

Vec3 random_in_unit_disk();

class Camera {
public:
    Camera() = default;
    Camera(Vec3 lookfrom ,Vec3 lookat, Vec3 vup, float vfov, float aspect, float aperture, float focus_dist,
            float t0, float t1);  // vfov is top to bottom in degrees
    Ray get_ray(float u, float v);

    Vec3 origin;
    Vec3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 u, v, w;
    float time0, time1;
    float lens_radius;
};


#endif //RAYTRACER1_CAMERA_H
