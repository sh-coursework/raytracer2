//
// Created by Steve Hwan on 10/21/18.
//

#ifndef RAYTRACER2_RENDER_SETTINGS_H
#define RAYTRACER2_RENDER_SETTINGS_H

#include <memory>
#include <string>

#include <boost/program_options.hpp>


// Just a data container - i'm okay with giving
// public read/write to everything.
// It will also make it easier to have an arg parser populate it.
struct RenderSettings {
    RenderSettings() : resolution_x(200), resolution_y(100),
                    num_channels(3),
                    number_samples_per_pixel(100),
                    use_bvh(true),
                    image_filename("temp.jpg"),
                    shutter_open(0.0f), shutter_close(1.0f)
                    {};

    int resolution_x, resolution_y;
    int num_channels;  // Maybe channels should be less direct.
    int number_samples_per_pixel;
    bool use_bvh;
    std::string image_filename;
    float shutter_open, shutter_close;

};


std::unique_ptr<boost::program_options::options_description>
GetRenderSettingsParser(RenderSettings &render_settings);


#endif //RAYTRACER2_RENDER_SETTINGS_H
