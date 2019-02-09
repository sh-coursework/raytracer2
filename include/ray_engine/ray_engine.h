//
// Created by Steve Hwan on 10/21/18.
//

#ifndef RAYTRACER2_RAY_ENGINE_H
#define RAYTRACER2_RAY_ENGINE_H

#include <memory>

#include "ray_engine/hitable.h"
#include "ray_engine/ray.h"
#include "vec3.h"

// This is the crux of the ray tracing engine
// Oooh... a raw pointer?  Well, I don't plan to change the pointer,
// and really even the contents are read-only for the hit calculation.
// And there's really no need to keep changing ownership of the
// unique_ptr passing it up and down through the recursive calls.
Vec3 ColorForRay(const Ray &r, const Hitable *const world,
                 std::shared_ptr<Hitable> light_shape, int depth);


#endif //RAYTRACER2_RAY_ENGINE_H
