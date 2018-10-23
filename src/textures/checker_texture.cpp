//
// Created by Steve Hwan on 10/22/18.
//

#include "textures/checker_texture.h"

vec3 CheckerTexture::value(float u, float v, const vec3& p) const
{
    auto sines = float(sin(10.0 * p.x()) * sin(10.0 * p.y()) * sin(10.0 * p.z()));
    if (sines < 0)
        return odd->value(u, v, p);
    else
        return even->value(u, v, p);
}
