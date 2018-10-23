//
// Created by Steve Hwan on 10/22/18.
//

#ifndef RAYTRACER2_TEXTURE_H
#define RAYTRACER2_TEXTURE_H

#include "vec3.h"

class Texture {
public:
    virtual vec3 value(float u, float v, const vec3& p) const = 0;
};

#endif //RAYTRACER2_TEXTURE_H
