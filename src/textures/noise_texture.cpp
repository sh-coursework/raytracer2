//
// Created by Steve Hwan on 10/23/18.
//

#include "textures/noise_texture.h"

#include <cmath>


Vec3
NoiseTexture::value(float u, float v, const Vec3& p) const
{
    return Vec3(1.0f, 1.0f, 1.0f)
            * 0.5f * (1.0f + noise.Noise(noise_scale * p));
}


Vec3 TurbulenceTexture::value(float u, float v, const Vec3& p) const
{
    return Vec3(1.0f, 1.0f, 1.0f) * noise.Turbulence(noise_scale * p);
}


Vec3 MarblelikeTexture::value(float u, float v, const Vec3& p) const {
    return Vec3(1.0f, 1.0f, 1.0f)
            * 0.5f * (1.0f + sin(noise_scale * p.z() + 10.0f * noise.Turbulence(p)));
}





