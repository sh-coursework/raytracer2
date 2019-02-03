#include <ctime>
#include <cfloat>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <memory>
#include <string>
#include <tuple>

#include <boost/range/irange.hpp>
#include <boost/program_options.hpp>
#include <OpenImageIO/imageio.h>

#include "ray_engine/ray.h"
#include "ray_engine/ray_engine.h"
#include "camera.h"
#include "render_settings.h"
#include "scene_generation.h"


namespace
{
    const size_t SUCCESS = 0;
}


int
main(int argc, char** argv) {
    ///////////////////////////////////////////////////////////////////////
    // Options and Render Settings processing

    auto generic_options = boost::program_options::options_description(
            "Generic options");
    generic_options.add_options()
            ("help,h", "produce help message")
            ("rendersettingsfile,r",
                    boost::program_options::value<std::string>(),
                    "render settings file")
            ;
    auto render_settings = RenderSettings();
    // Returns a unique_ptr
    auto render_settings_options = GetRenderSettingsParser(render_settings);

    auto cmdline_options = boost::program_options::options_description();
    cmdline_options.add(generic_options).add(*render_settings_options);

    boost::program_options::variables_map parse_results;
    boost::program_options::store(
            boost::program_options::parse_command_line(
                    argc, argv, cmdline_options),
            parse_results
            );
    // --help option
    if (parse_results.count("help")) {
        std::cout << "Command Line Options" << std::endl
            << cmdline_options << std::endl;
        return SUCCESS;
    }
    if (parse_results.count("rendersettingsfile")) {
        std::string render_settings_filename
                = parse_results["rendersettingsfile"].as<std::string>();
        std::ifstream render_settings_file_stream(
                render_settings_filename.c_str());
        auto render_settings_file_options
                = boost::program_options::options_description();
        render_settings_file_options.add(*render_settings_options);
        boost::program_options::store(
                boost::program_options::parse_config_file(
                        render_settings_file_stream,
                        render_settings_file_options),
                        parse_results
        );
    }

    boost::program_options::notify(parse_results);
    // After this, maybe consider pulling in a yaml file to override
    // render settings?
    // It may be extraneous because the boost parser already gives me the
    // ability to read a config file - for not, that's probably good enough...

    ///////////////////////////////////////////////////////////////////////
    // Build scene

    auto start_time = std::clock();

    std::unique_ptr<Hitable> world_ptr;
    std::unique_ptr<Camera> cam_ptr;
    std::tie(world_ptr, cam_ptr) = GetScene(render_settings);
    auto cam = *cam_ptr;  // Copying a Camera, I guess.  But I'm about to
                          // dereference it like literally a million times.
    auto world_raw_ptr = world_ptr.get();  // If I'm not changing ownership,
                          // I think Herb Sutter says raw pointer are beautiful.

    auto world_built_time = std::clock();
    std::cout << "Time to create scene: "
              << (world_built_time - start_time) / double(CLOCKS_PER_SEC)
              << "\n";

    ///////////////////////////////////////////////////////////////////////
    // Get to the rendering and writing images


    float pixels[render_settings.resolution_x_ * render_settings.resolution_y_
            * render_settings.num_channels_];

    // OIIO::ImageOutput *oiio_out = OIIO::ImageOutput::create(
    auto oiio_out = OIIO::ImageOutput::create(
            render_settings.image_filename_);
    if (! oiio_out)
        return WEXITED;

    // Yes, I could do this directly, I guess, but I'm planning ahead
    // to pull this chunk out to its own function and maybe do better
    // tiling or scanline.
    auto resolution_x = render_settings.resolution_x_;
    auto resolution_y = render_settings.resolution_y_;
    auto number_samples_per_pixel = render_settings.number_samples_per_pixel_;
    // Run though pixels in the image, and write to stdout
    for (auto row_count: boost::irange(0, resolution_y)) {
        auto row_num = resolution_y - 1 - row_count;
        for (auto column_num: boost::irange(0, resolution_x)) {
            auto pixel_color = Vec3(0.0f, 0.0f, 0.0f);

            for (auto sample_num: boost::irange(0, number_samples_per_pixel)) {
                auto u = float(column_num + drand48()) / float(resolution_x);
                auto v = float(row_num + drand48()) / float(resolution_y);

                auto r = cam.GetRay(u, v);
                auto p = r.PointAtParameter(2.0f);  // p not used
                pixel_color += ColorForRay(r, world_raw_ptr, 0);
            }
            pixel_color /= float(number_samples_per_pixel);

            // "write" image to stdout
            // "gamma correct" - maybe in the future this will be opencolor lut?
            pixel_color = Vec3(sqrtf(pixel_color[0]), sqrtf(pixel_color[1]),
                    sqrtf(pixel_color[2]));
            auto pixel_base_index = ((resolution_y - 1 - row_num) * resolution_x
                    + column_num) * render_settings.num_channels_;
            pixels[pixel_base_index    ] = pixel_color[0];
            pixels[pixel_base_index + 1] = pixel_color[1];
            pixels[pixel_base_index + 2] = pixel_color[2];
        }
        if (0 == row_count % 10)
            std::cout << "\nRender time: ("
                << std::setw(6) << std::setprecision(2) << std::fixed
                << 100.0 * double(row_count + 1) / resolution_y
                << "%) "
                << (std::clock() - world_built_time) / double(CLOCKS_PER_SEC)
                << " seconds";
        else
            std::cout << ".";
    }
    std::cout << "\n";
    auto render_end_time = std::clock();
    std::cout << "Time to render scene: "
            << (render_end_time - world_built_time) / double(CLOCKS_PER_SEC)
            << "\n";

    // Write Image
    // Note: ppm is m_pnm_type 3 - probably 6-3.
    // in a spec, how do I set spec.get_int_attribute ?
    OIIO::ImageSpec spec(resolution_x, resolution_y,
            render_settings.num_channels_, OIIO::TypeDesc::FLOAT);
    spec.attribute("pnm:binary", 0);  // int - just for ppm to turn off binary
    oiio_out->open(render_settings.image_filename_, spec);
    oiio_out->write_image(OIIO::TypeDesc::FLOAT, pixels);
    oiio_out->close();
//    OIIO::ImageOutput::destroy(oiio_out);
}