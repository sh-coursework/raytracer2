#include <ctime>
#include "float.h"
#include <iostream>

#include "boost/range/irange.hpp"

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


int main() {
    // TODO: I should start sending render settings to a yaml or something
    auto resolution_x = 200;
    auto resolution_y = 100;
    const auto channels = 3;
    auto number_samples_per_pixel = 10;
    auto use_bvh = true;
    const char *image_filename = "temp.jpg";

    auto t_min = 0.0f, t_max = 1.0f;

    auto start_time = std::clock();

    // Build world
    hitable *world = random_scene(t_min, t_max, use_bvh);

    auto world_built_time = std::clock();
    std::cerr << "Time to create scene: "
            << (world_built_time - start_time) / double(CLOCKS_PER_SEC)
            << "\n";

    // Camera
    auto lookfrom = vec3(13.0f, 2.0f, 3.0f);
    auto lookat = vec3(0.0f, 0.0f, 0.0f);
    auto dist_to_focus = (lookfrom - lookat).length();
    auto aperture = 0.0f;
    auto cam = camera(lookfrom, lookat, vec3(0.0f, 1.0f, 0.0f),
                      20.0f,
                      float(resolution_x) / float(resolution_y), aperture, dist_to_focus,
                      t_min, t_max);

    float pixels[resolution_x * resolution_y * channels];

    // original ppm
    std::cout << "P3\n" << resolution_x << " " << resolution_y << "\n255\n";

    OIIO::ImageOutput *oiio_out = OIIO::ImageOutput::create(image_filename);
    if (! oiio_out)
        return WEXITED;

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
            auto ir = int(255.99 * pixel_color[0]);
            auto ig = int(255.99 * pixel_color[1]);
            auto ib = int(255.99 * pixel_color[2]);
            std::cout << ir << " " << ig << " " << ib << "\n";
            auto pixel_base_index = ((resolution_y - 1 - row_num) * resolution_x + column_num) * channels;
            pixels[pixel_base_index    ] = pixel_color[0];
            pixels[pixel_base_index + 1] = pixel_color[1];
            pixels[pixel_base_index + 2] = pixel_color[2];
        }
        if (0 == row_num % 10)
            std::cerr << "\nRender time so far: " << (std::clock() - world_built_time) / double(CLOCKS_PER_SEC) << " ";
        else
            std::cerr << ".";
    }
    std::cerr << "\n";
    auto render_end_time = std::clock();
    std::cerr << "Time to render scene: "
            << (render_end_time - world_built_time) / double(CLOCKS_PER_SEC)
            << "\n";

    // Write Image
    // Note: ppm is m_pnm_type 3 - probably 6-3.  So how do I set pnm:binary == 0(false)
    // in a spec, how do I set spec.get_int_attribute ?
    OIIO::ImageSpec spec(resolution_x, resolution_y, channels, OIIO::TypeDesc::FLOAT);
    spec.attribute("pnm:binary", 0);  // int - just for ppm to turn off binary
    oiio_out->open(image_filename, spec);
    oiio_out->write_image(OIIO::TypeDesc::FLOAT, pixels);
    oiio_out->close();
    OIIO::ImageOutput::destroy(oiio_out);
}