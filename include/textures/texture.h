//
// Created by Steve Hwan on 10/22/18.
//

#ifndef RAYTRACER2_TEXTURE_H
#define RAYTRACER2_TEXTURE_H

#include "vec3.h"

class Texture {
public:
    virtual Vec3 Value(float u, float v, const Vec3 &p) const = 0;
};

#endif //RAYTRACER2_TEXTURE_H
