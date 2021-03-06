//
// Created by Steve Hwan on 10/21/18.
//

#ifndef RAYTRACER2_RAY_ENGINE_H
#define RAYTRACER2_RAY_ENGINE_H

#include <memory>

#include "vec3.h"
#include "ray_engine/ray.h"
#include "ray_engine/hitable.h"
#include "render_context.h"

// This is the crux of the ray tracing engine
// Oooh... a raw pointer?  Well, I don't plan to change the pointer,
// and really even the contents are read-only for the hit calculation.
// And there's really no need to keep changing ownership of the
// unique_ptr passing it up and down through the recursive calls.
Vec3 ColorForRay(const Ray &r, const RenderContext &render_context,
                 int depth);

Vec3 DeNaN(const Vec3 &new_color);

#endif //RAYTRACER2_RAY_ENGINE_H
