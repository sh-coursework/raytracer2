//
// Created by Steve Hwan on 10/23/18.
//

#include <cmath>

#include "textures/noise_texture.h"


vec3
NoiseTexture::value(float u, float v, const vec3& p) const
{
    return vec3(1.0f, 1.0f, 1.0f)
            * 0.5f * (1.0f + noise.Noise(noise_scale * p));
}


vec3 TurbulenceTexture::value(float u, float v, const vec3& p) const
{
    return vec3(1.0f, 1.0f, 1.0f) * noise.Turbulence(noise_scale * p);
}


vec3 MarblelikeTexture::value(float u, float v, const vec3& p) const {
    return vec3(1.0f, 1.0f, 1.0f)
            * 0.5f * (1.0f + sin(noise_scale * p.z() + 10.0f * noise.Turbulence(p)));
}





