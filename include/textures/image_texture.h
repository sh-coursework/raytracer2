//
// Created by Steve Hwan on 10/28/18.
//

#ifndef RAYTRACER2_IMAGE_TEXTURE_H
#define RAYTRACER2_IMAGE_TEXTURE_H


#include <string>
#include <vector>

#include "vec3.h"
#include "textures/texture.h"


class ImageTexture : public Texture
{
public:
    explicit ImageTexture(std::string image_file);
    Vec3 value(float u, float v, const Vec3& p) const override;

    std::string image_file_;
    int image_resx, image_resy;
    std::vector<unsigned char> pixels;
};


#endif //RAYTRACER2_IMAGE_TEXTURE_H
