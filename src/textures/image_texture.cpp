//
// Created by Steve Hwan on 10/28/18.
//

#include "textures/image_texture.h"

#include <algorithm>

#include <OpenImageIO/imageio.h>


ImageTexture::ImageTexture(std::string image_file) :
        image_file_(std::move(image_file))
{
    OIIO::ImageInput *in = OIIO::ImageInput::open(image_file_);
    if (! in)
        return;
    const OIIO::ImageSpec &spec = in->spec();
    image_resx = spec.width;
    image_resy = spec.height;
    int channels = spec.nchannels;
    pixels.clear();
    int buffer_size = image_resx * image_resy * channels;
    pixels.reserve(size_t(buffer_size));
    in->read_image(OIIO::TypeDesc::UINT8, &pixels[0]);
    in->close();
    OIIO::ImageInput::destroy(in);
}

Vec3 ImageTexture::value(float u, float v, const Vec3& p) const
{
    auto i = std::max(0, std::min(image_resx - 1, int(u * image_resx)));
    auto j = std::max(0, std::min(image_resy - 1,
            int((1 - v) * float(image_resy - 0.001))));

    auto r = int(pixels[3 * i  + 3 * image_resx * j]    ) / 255.0f;
    auto g = int(pixels[3 * i  + 3 * image_resx * j + 1]) / 255.0f;
    auto b = int(pixels[3 * i  + 3 * image_resx * j + 2]) / 255.0f;
    return {r, g, b};

}
