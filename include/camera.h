//
// Created by Steve Hwan on 10/2/18.
//

#ifndef RAYTRACER1_CAMERA_H
#define RAYTRACER1_CAMERA_H

#include "ray_engine/ray.h"

vec3 random_in_unit_disk();

class camera {
public:
    camera() = default;
    camera(vec3 lookfrom , vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist,
            float t0, float t1);  // vfov is top to bottom in degrees
    ray get_ray(float u, float v);

    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    float time0, time1;
    float lens_radius;
};


#endif //RAYTRACER1_CAMERA_H
