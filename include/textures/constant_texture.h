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
    explicit ConstantTexture(Vec3 c) : color_(c) {}
    Vec3 Value(float u, float v, const Vec3 &p) const override
            { return color_; }

    Vec3 color_;
};

#endif //RAYTRACER2_CONSTANT_TEXTURE_H
