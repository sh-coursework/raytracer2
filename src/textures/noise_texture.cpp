//
// Created by Steve Hwan on 10/23/18.
//

#include "textures/noise_texture.h"


vec3
NoiseTexture::value(float u, float v, const vec3& p) const
{
    return vec3(1.0f, 1.0f, 1.0f) * noise.Noise(noise_scale * p);
}
