//
// Created by svhwan on 6/16/2019.
//

#ifndef RAYTRACER2_SRC_TEXTURES_RAMP_TEXTURE_H_
#define RAYTRACER2_SRC_TEXTURES_RAMP_TEXTURE_H_

#include "textures/texture.h"
#include "vec3.h"

class RampTexture : public Texture {
 public:
  RampTexture(const Vec3 &color_0, const Vec3 &color_1);
  Vec3 Value(float u, float v, const Vec3 &p) const override;
 private:
  Vec3 color0_, color1_;

};

#endif //RAYTRACER2_SRC_TEXTURES_RAMP_TEXTURE_H_
