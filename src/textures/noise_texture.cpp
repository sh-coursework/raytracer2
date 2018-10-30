//
// Created by Steve Hwan on 10/23/18.
//

#include "textures/noise_texture.h"

#include <cmath>


Vec3
NoiseTexture::Value(float u, float v, const Vec3 &p) const
{
    return Vec3(1.0f, 1.0f, 1.0f)
            * 0.5f * (1.0f + noise_.Noise(noise_scale_ * p));
}


Vec3 TurbulenceTexture::Value(float u, float v, const Vec3 &p) const
{
    return Vec3(1.0f, 1.0f, 1.0f) * noise_.Turbulence(noise_scale_ * p);
}


Vec3 MarblelikeTexture::Value(float u, float v, const Vec3 &p) const {
    return Vec3(1.0f, 1.0f, 1.0f)
            * 0.5f * (1.0f
                    + sin(noise_scale_ * p.z() + 10.0f * noise_.Turbulence(p)));
}





