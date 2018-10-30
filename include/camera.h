//
// Created by Steve Hwan on 10/2/18.
//

#ifndef RAYTRACER1_CAMERA_H
#define RAYTRACER1_CAMERA_H

#include "vec3.h"
#include "ray_engine/ray.h"

class Camera {
public:
    Camera() = default;
    Camera(Vec3 lookfrom ,Vec3 lookat, Vec3 vup, float vfov, float aspect,
           float aperture, float focus_dist,
           float t0, float t1);  // vfov is top to bottom in degrees
    Ray GetRay(float u, float v);

    Vec3 origin_;
    Vec3 lower_left_corner_;
    Vec3 horizontal_;
    Vec3 vertical_;
    Vec3 u_, v_, w_;
    float time_0_, time_1_;
    float lens_radius_;
};


#endif //RAYTRACER1_CAMERA_H
