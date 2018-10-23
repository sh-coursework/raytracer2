#include <ctime>
#include "float.h"
#include <iostream>
#include <fstream>

#include <boost/range/irange.hpp>
#include <boost/program_options.hpp>

#include <OpenImageIO/imageio.h>

#include "scene_geometry/sphere.h"
#include "scene_geometry/moving_sphere.h"
#include "ray_engine/hitable_list.h"
#include "ray_engine/bvh_node.h"
#include "ray_engine/ray.h"
#include "ray_engine/ray_engine.h"
#include "materials/lambertian.h"
#include "materials/metal.h"
#include "materials/dielectric.h"
#include "camera.h"
#include "render_settings.h"


namespace
{
    const size_t SUCCESS = 0;
}


hitable *random_scene(float t_min, float t_max, bool use_bvh) {
    // Should hitable be a unique pointer or something?
    // Would ownership transfer as a return value?
    auto n = 50000;
    hitable **list = new hitable*[n+1];  // array of (hitable *)
    list[0] = new sphere(vec3(0.0f, -1000.0f, 0.0f), 1000.0f, new lambertian(vec3(0.5f, 0.5f, 0.5f)));
    auto i = 1;

    for (auto a: boost::irange(-10, 10)) {
        for (auto b: boost::irange(-10, 10)) {
            auto choose_mat = float(drand48());
            auto center = vec3(a + 0.9f * drand48(), 0.2f, b + 0.9f * drand48());
            if (choose_mat < 0.8) { // diffuse
                list[i++] = new moving_sphere(center, center + vec3(0, 0.5 * drand48(), 0),
                        0.0, 1.0,
                        0.2f,
                        new lambertian(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())));
            } else if (choose_mat < 0.95) { // metal
                list[i++] = new sphere(center, 0.2,
                        new metal(vec3(0.5 * (1.0f + drand48()), 0.5*(1.0f + drand48()), 0.5*(1.0f + drand48())), 0.5 * drand48()));
            } else {  // glass
                list[i++] = new sphere(center, 0.2f, new dielectric(1.5f));
            }
        }
    }

    list[i++] = new sphere(vec3(0.0f, 1.0f, 0.0f), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4.0f, 1.0f, 0.0f), 1.0f, new lambertian(vec3(0.4f, 0.2f, 0.2f)));
    list[i++] = new sphere(vec3(4.0f, 1.0f, 0.0f), 1.0f, new metal(vec3(0.7f, 0.6f, 0.5f), 0.0f));

    if (use_bvh)
        return new bvh_node(list, i, t_min, t_max);
    else
        return new hitable_list(list, i);
}


int main(int argc, char** argv) {
    ///////////////////////////////////////////////////////////////////////
    // Options and Render Settings processing

    auto generic_options = boost::program_options::options_description("Generic options");
    generic_options.add_options()
            ("help,h", "produce help message")
            ("rendersettingsfile,r", boost::program_options::value<std::string>(), "render settings file")
            ;
    auto render_settings = RenderSettings();
    // Returns a unique_ptr
    auto render_settings_options = GetRenderSettingsParser(render_settings);

    auto cmdline_options = boost::program_options::options_description();
    cmdline_options.add(generic_options).add(*render_settings_options);

    boost::program_options::variables_map parse_results;
    boost::program_options::store(
            boost::program_options::parse_command_line(argc, argv, cmdline_options),
            parse_results
            );
    // --help option
    if (parse_results.count("help"))
    {
        std::cout << "Command Line Options" << std::endl
            << cmdline_options << std::endl;
        return SUCCESS;
    }
    if (parse_results.count("rendersettingsfile"))
    {
        std::string render_settings_filename = parse_results["rendersettingsfile"].as<std::string>();
        std::ifstream render_settings_file_stream(render_settings_filename.c_str());
        auto render_settings_file_options = boost::program_options::options_description();
        render_settings_file_options.add(*render_settings_options);
        boost::program_options::store(
                boost::program_options::parse_config_file(render_settings_file_stream,
                        render_settings_file_options),
                        parse_results
        );

    }

    boost::program_options::notify(parse_results);
    // After this, maybe consider pulling in a yaml file to override render settings?
    // It may be extraneous because the boost parser already gives me the
    // ability to read a config file - for not, that's probably good enough...

    ///////////////////////////////////////////////////////////////////////
    // Build scene

    auto start_time = std::clock();

    hitable *world = random_scene(render_settings.shutter_open, render_settings.shutter_close, render_settings.use_bvh);

    // Camera
    auto lookfrom = vec3(13.0f, 2.0f, 3.0f);
    auto lookat = vec3(0.0f, 0.0f, 0.0f);
    auto dist_to_focus = (lookfrom - lookat).length();
    auto aperture = 0.0f;
    auto cam = camera(lookfrom, lookat, vec3(0.0f, 1.0f, 0.0f),
                      20.0f,
                      float(render_settings.resolution_x) / float(render_settings.resolution_y), aperture, dist_to_focus,
                      render_settings.shutter_open, render_settings.shutter_close);

    auto world_built_time = std::clock();
    std::cout << "Time to create scene: "
              << (world_built_time - start_time) / double(CLOCKS_PER_SEC)
              << "\n";

    ///////////////////////////////////////////////////////////////////////
    // Get to the rendering and writing images


    float pixels[render_settings.resolution_x * render_settings.resolution_y * render_settings.num_channels];

    OIIO::ImageOutput *oiio_out = OIIO::ImageOutput::create(render_settings.image_filename);
    if (! oiio_out)
        return WEXITED;

    // Yes, I could do this directly, I guess, but I'm planning ahead
    // to pull this chunk out to its own function and maybe do better
    // tiling or scanline.
    auto resolution_x = render_settings.resolution_x;
    auto resolution_y = render_settings.resolution_y;
    auto number_samples_per_pixel = render_settings.number_samples_per_pixel;
    // Run though pixels in the image, and write to stdout
    for (auto row_num: boost::irange(resolution_y - 1, -1, -1)) {  // unclear with -1 as the bottom
        for (auto column_num: boost::irange(0, resolution_x)) {
            auto pixel_color = vec3(0.0f, 0.0f, 0.0f);

            for (auto sample_num: boost::irange(0, number_samples_per_pixel)) {
                auto u = float(column_num + drand48()) / float(resolution_x);
                auto v = float(row_num + drand48()) / float(resolution_y);

                auto r = cam.get_ray(u, v);
                auto p = r.point_at_parameter(2.0f);  // p not used
                pixel_color += color(r, world, 0);
            }
            pixel_color /= float(number_samples_per_pixel);

            // "write" image to stdout
            // "gamma correct" - maybe in the future this will be opencolor lut?
            pixel_color = vec3(sqrtf(pixel_color[0]), sqrtf(pixel_color[1]), sqrtf(pixel_color[2]));
            auto pixel_base_index = ((resolution_y - 1 - row_num) * resolution_x + column_num) * render_settings.num_channels;
            pixels[pixel_base_index    ] = pixel_color[0];
            pixels[pixel_base_index + 1] = pixel_color[1];
            pixels[pixel_base_index + 2] = pixel_color[2];
        }
        if (0 == row_num % 10)
            std::cout << "\nRender time so far: " << (std::clock() - world_built_time) / double(CLOCKS_PER_SEC) << " ";
        else
            std::cout << ".";
    }
    std::cout << "\n";
    auto render_end_time = std::clock();
    std::cout << "Time to render scene: "
            << (render_end_time - world_built_time) / double(CLOCKS_PER_SEC)
            << "\n";

    // Write Image
    // Note: ppm is m_pnm_type 3 - probably 6-3.  So how do I set pnm:binary == 0(false)
    // in a spec, how do I set spec.get_int_attribute ?
    OIIO::ImageSpec spec(resolution_x, resolution_y, render_settings.num_channels, OIIO::TypeDesc::FLOAT);
    spec.attribute("pnm:binary", 0);  // int - just for ppm to turn off binary
    oiio_out->open(render_settings.image_filename, spec);
    oiio_out->write_image(OIIO::TypeDesc::FLOAT, pixels);
    oiio_out->close();
    OIIO::ImageOutput::destroy(oiio_out);
}