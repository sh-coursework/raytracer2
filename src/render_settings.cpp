//
// Created by Steve Hwan on 10/21/18.
//


#include "render_settings.h"


// TODO: if I was more adventurous, maybe I should be returning a right handed ref?
std::unique_ptr<boost::program_options::options_description>
GetRenderSettingsParser(RenderSettings &render_settings)
{
    std::unique_ptr<boost::program_options::options_description>
        render_settings_options(new boost::program_options::options_description("Render Settings Options"));
    render_settings_options->add_options()
            ("resx", boost::program_options::value<int>(&render_settings.resolution_x)->default_value(200), "resolution x(width) in pixels")
            ("resy", boost::program_options::value<int>(&render_settings.resolution_y)->default_value(100), "resolution y(height) in pixels")
            ("nsamp", boost::program_options::value<int>(&render_settings.number_samples_per_pixel)->default_value(100))
            ("bvh", boost::program_options::value<bool>(&render_settings.use_bvh)->default_value(true), "use bounding volume hierarchy(default true)")
            ("imagename,o", boost::program_options::value<std::string>(&render_settings.image_filename)->default_value("temp.jpg"), "image filename")
            ("shutter_open", boost::program_options::value<float>(&render_settings.shutter_open)->default_value(0.0), "shutter open (default 0.0)")
            ("shutter_close", boost::program_options::value<float>(&render_settings.shutter_close)->default_value(1.0), "shutter close (default 1.0)")
            ;
    return render_settings_options;
}

