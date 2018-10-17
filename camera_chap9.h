//
// Created by Steve Hwan on 10/2/18.
//

#ifndef RAYTRACER1_CAMERA_H
#define RAYTRACER1_CAMERA_H

#include "ray.h"

class camera {
public:
    camera();
    ray get_ray(float u, float v);

    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
};


#endif //RAYTRACER1_CAMERA_H
