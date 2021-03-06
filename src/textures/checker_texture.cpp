//
// Created by Steve Hwan on 10/22/18.
//

#include "textures/checker_texture.h"

Vec3 CheckerTexture::Value(float u, float v, const Vec3 &p) const
{
    auto sines = float(sin(10.0 * p.x()) * sin(10.0 * p.y())
                       * sin(10.0 * p.z()));
    if (sines < 0)
        return odd_->Value(u, v, p);
    else
        return even_->Value(u, v, p);
}
