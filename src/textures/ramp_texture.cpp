//
// Created by svhwan on 6/16/2019.
//

#include "textures/ramp_texture.h"

RampTexture::RampTexture(const Vec3 &color_0, const Vec3 &color_1)
  : color0_(color_0), color1_(color_1) {}

Vec3 RampTexture::Value(float u, float v, const Vec3 &p) const {
  return (1.0f - v) * color0_ + v * color1_;
}
