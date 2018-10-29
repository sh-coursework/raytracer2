//
// Created by Steve Hwan on 10/22/18.
//

#ifndef RAYTRACER2_CONSTANT_TEXTURE_H
#define RAYTRACER2_CONSTANT_TEXTURE_H

#include "vec3.h"
#include "textures/texture.h"

class ConstantTexture : public Texture
{
public:
    ConstantTexture() = default;
    explicit ConstantTexture(vec3 c) : color(c) {}
    vec3 value(float u, float v, const vec3& p) const override
    { return color; }

    vec3 color;
};

#endif //RAYTRACER2_CONSTANT_TEXTURE_H
