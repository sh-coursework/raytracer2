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
    image_res_x_ = spec.width;
    image_res_y_ = spec.height;
    int channels = spec.nchannels;
    pixels_.clear();
    int buffer_size = image_res_x_ * image_res_y_ * channels;
    pixels_.reserve(size_t(buffer_size));
    in->read_image(OIIO::TypeDesc::UINT8, &pixels_[0]);
    in->close();
    OIIO::ImageInput::destroy(in);
}

Vec3 ImageTexture::Value(float u, float v, const Vec3 &p) const
{
    auto i = std::max(0, std::min(image_res_x_ - 1, int(u * image_res_x_)));
    auto j = std::max(0, std::min(image_res_y_ - 1,
            int((1 - v) * float(image_res_y_ - 0.001))));

    auto r = int(pixels_[(image_res_x_ * j + i) * 3]    ) / 255.0f;
    auto g = int(pixels_[(image_res_x_ * j + i) * 3 + 1]) / 255.0f;
    auto b = int(pixels_[(image_res_x_ * j + i) * 3 + 2]) / 255.0f;
    return {r, g, b};

}
