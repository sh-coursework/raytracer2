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
    RenderSettings() : resolution_x_(200), resolution_y_(100),
                    num_channels_(3),
                    number_samples_per_pixel_(100),
                    use_bvh_(true),
                    image_filename_("temp.jpg"),
                    shutter_open_(0.0f), shutter_close_(1.0f)
                    {};

    int resolution_x_, resolution_y_;
    int num_channels_;  // Maybe channels should be less direct.
    int number_samples_per_pixel_;
    bool use_bvh_;
    std::string image_filename_;
    float shutter_open_, shutter_close_;

};


std::unique_ptr<boost::program_options::options_description>
GetRenderSettingsParser(RenderSettings &render_settings);


#endif //RAYTRACER2_RENDER_SETTINGS_H
