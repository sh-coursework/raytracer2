//
// Created by Steve Hwan on 10/22/18.
//

#ifndef RAYTRACER2_CHECKER_TEXTURE_H
#define RAYTRACER2_CHECKER_TEXTURE_H

#include "vec3.h"
#include "texture.h"

class CheckerTexture : public Texture {
public:
    CheckerTexture() { odd_ = nullptr; even_ = nullptr; };
    CheckerTexture(Texture *texture0, Texture *texture1) : even_(texture0),
                                                           odd_(texture1) {}
    Vec3 Value(float u, float v, const Vec3 &p) const override;

    Texture *odd_;
    Texture *even_;
};


#endif //RAYTRACER2_CHECKER_TEXTURE_H
