//
// Created by Steve Hwan on 10/21/18.
//

#ifndef RAYTRACER2_RAY_ENGINE_H
#define RAYTRACER2_RAY_ENGINE_H

#include "vec3.h"
#include "ray_engine/hitable.h"
#include "ray_engine/ray.h"

//class ray_engine {
//
//};

// This is the crux of the ray tracing engine
vec3 color(const ray& r, hitable *world, int depth);




#endif //RAYTRACER2_RAY_ENGINE_H
