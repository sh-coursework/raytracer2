//
// Created by Steve Hwan on 10/23/18.
//

#ifndef RAYTRACER2_NOISE_TEXTURE_H
#define RAYTRACER2_NOISE_TEXTURE_H


#include "vec3.h"
#include "textures/texture.h"
#include "textures/perlin_noise.h"


class NoiseTexture : public Texture {
public:
    NoiseTexture() = default;
    explicit NoiseTexture(float sc) : noise_scale_(sc) {};
    Vec3 Value(float u, float v, const Vec3 &p) const override;

    PerlinNoise noise_;
    float noise_scale_;
};

class TurbulenceTexture : public NoiseTexture {
public:
    explicit TurbulenceTexture(float sc) : NoiseTexture(sc) {};
    Vec3 Value(float u, float v, const Vec3 &p) const override;
};

class MarblelikeTexture : public NoiseTexture {
public:
    explicit MarblelikeTexture(float sc) : NoiseTexture(sc) {};
    Vec3 Value(float u, float v, const Vec3 &p) const override;
};


#endif //RAYTRACER2_NOISE_TEXTURE_H
