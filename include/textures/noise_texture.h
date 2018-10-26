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
    NoiseTexture() {}
    NoiseTexture(float sc) : noise_scale(sc) {};
    virtual vec3 value(float u, float v, const vec3& p) const;

    PerlinNoise noise;
    float noise_scale;
};


#endif //RAYTRACER2_NOISE_TEXTURE_H
