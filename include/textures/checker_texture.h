//
// Created by Steve Hwan on 10/22/18.
//

#ifndef RAYTRACER2_CHECKER_TEXTURE_H
#define RAYTRACER2_CHECKER_TEXTURE_H

#include "vec3.h"
#include "texture.h"

class CheckerTexture : public Texture {
public:
    CheckerTexture() { odd = nullptr; even = nullptr; };
    CheckerTexture(Texture *texture0, Texture *texture1): even(texture0), odd(texture1) {}
    Vec3 value(float u, float v, const Vec3& p) const override;

    Texture *odd;
    Texture *even;
};


#endif //RAYTRACER2_CHECKER_TEXTURE_H
