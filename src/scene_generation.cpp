//
// Created by Steve Hwan on 10/27/18.
//

#include <vector>

#include "vec3.h"

#include "ray_engine/hitable.h"
#include "ray_engine/bvh_node.h"
#include "ray_engine/hitable_list.h"

#include "scene_geometry/sphere.h"
#include "scene_geometry/moving_sphere.h"

#include "textures/texture.h"
#include "textures/constant_texture.h"
#include "textures/checker_texture.h"
#include "textures/noise_texture.h"
#include "textures/image_texture.h"

#include "materials/lambertian.h"
#include "materials/metal.h"
#include "materials/dielectric.h"
#include "materials/constant_material.h"

#include "scene_generation.h"


hitable *random_scene(float t_min, float t_max, bool use_bvh)
{
    // hitable is an abstract base, so I wouldn't know how/what to allocate
    // up front, and there's not really a default placeholder.
    // So I want the vector of pointers, not vector of hitables.
    std::vector<std::shared_ptr<hitable>> scene_list;

    Texture *checker = new CheckerTexture(
            new ConstantTexture(vec3(0.2, 0.3, 0.1)),
            new ConstantTexture(vec3(0.9, 0.9, 0.9))
            );
    scene_list.push_back( std::shared_ptr<hitable>(
            new sphere(vec3(0.0f, -1000.0f, 0.0f), 1000.0f, new lambertian(checker))
     ));

    for (auto a: boost::irange(-10, 10))
    {
        for (auto b: boost::irange(-10, 10))
        {
            auto choose_mat = float(drand48());
            auto center = vec3(a + 0.9f * drand48(), 0.2f, b + 0.9f * drand48());
            if (choose_mat < 0.8)  // diffuse
            {
                scene_list.push_back( std::shared_ptr<hitable>(
                        new moving_sphere(center, center + vec3(0, 0.5 * drand48(), 0),
                                0.0, 1.0,
                                0.2f,
                                new lambertian(new ConstantTexture(
                                        vec3(drand48() * drand48(), drand48() * drand48(), drand48() * drand48()))))
                ));
            } else if (choose_mat < 0.95)  // metal
            {
                scene_list.push_back( std::shared_ptr<hitable>(
                        new sphere(center, 0.2,
                                new metal(new ConstantTexture(
                                        vec3(0.5 * (1.0f + drand48()), 0.5 * (1.0f + drand48()), 0.5 * (1.0f + drand48()))),
                                        0.5 * drand48()))
                ));
            } else  // glass
            {
                scene_list.push_back( std::shared_ptr<hitable>(
                        new sphere(center, 0.2f, new dielectric(1.5f))
                ));
            }
        }
    }

    scene_list.push_back( std::shared_ptr<hitable>(
            new sphere(vec3(0.0f, 1.0f, 0.0f), 1.0, new dielectric(1.5))
    ));
    scene_list.push_back( std::shared_ptr<hitable>(
            new sphere(vec3(-4.0f, 1.0f, 0.0f), 1.0f, new lambertian(new ConstantTexture(vec3(0.4f, 0.2f, 0.2f))))
    ));
    scene_list.push_back( std::shared_ptr<hitable>(
            new sphere(vec3(4.0f, 1.0f, 0.0f), 1.0f, new metal(new ConstantTexture(vec3(0.7f, 0.6f, 0.5f)), 0.0f))
    ));

    if (use_bvh)
        return new bvh_node(scene_list.begin(), scene_list.end(), t_min, t_max);
    else
        return new hitable_list(scene_list);
}



hitable *
two_spheres()
{
    Texture *checker = new CheckerTexture(
            new ConstantTexture(vec3(0.2, 0.3, 0.1)),
            new ConstantTexture(vec3(0.9, 0.9, 0.9))
    );
    std::vector<std::shared_ptr<hitable>> scene_list;
    scene_list.push_back( std::shared_ptr<hitable>(
            new sphere(vec3(0, -10, 0), 10, new lambertian(checker))
    ));
    scene_list.push_back( std::shared_ptr<hitable>(
            new sphere(vec3(0, 10, 0), 10, new lambertian(checker))
    ));

    return new hitable_list(scene_list);
}

// Maybe change this to smart pointers so the assignement
// to lambertian can be reference counted, and the ownership
// better managed.
hitable *
two_noise_spheres(Texture *noise_textured)
{
    std::vector<std::shared_ptr<hitable>> scene_list;
    // For now, assume C++11, not 14 so no "make_unique"
    scene_list.push_back( std::shared_ptr<hitable>(
            new sphere(vec3(0, -1000, 0), 1000, new lambertian(noise_textured))
    ));
    scene_list.push_back( std::shared_ptr<hitable>(
            new sphere(vec3(0, 2, 0), 2, new lambertian(noise_textured))
    ));

    return new hitable_list(scene_list);
}

hitable *
two_perlin_spheres() {
    Texture *noise_textured = new NoiseTexture(4.0f);
    return two_noise_spheres(noise_textured);
}

hitable *
two_turbulence_spheres()
{
    Texture *noise_textured = new TurbulenceTexture(4.0f);
    return two_noise_spheres(noise_textured);
}

hitable *
two_marblelike_spheres()
{
    Texture *noise_textured = new MarblelikeTexture(4.0f);
    return two_noise_spheres(noise_textured);
}

hitable *
image_sphere_tests()
{
    std::string test_filename = "test_data/land_ocean_ice_cloud_2048.jpg";
    Texture *noise_textured = new MarblelikeTexture(4.0f);
    Texture *image_textured = new ImageTexture(test_filename);

    std::vector<std::shared_ptr<hitable>> scene_list;
    // For now, assume C++11, not 14 so no "make_unique"
    // Big sphere - let's keep the marble
    scene_list.push_back( std::shared_ptr<hitable>(
            new sphere(vec3(0, -1000, 0), 1000, new lambertian(noise_textured))
    ));

    scene_list.push_back( std::shared_ptr<hitable>(
            new sphere(vec3(0, 2, 0), 2, new ConstantMaterial(image_textured))
    ));

    return new hitable_list(scene_list);
}


//////////////////////////////////////////////////////////////////////////
/// Cameras
//////////////////////////////////////////////////////////////////////////


camera && random_scene_cam(const RenderSettings &render_settings)
{
    auto lookfrom = vec3(13.0f, 2.0f, 3.0f);
    auto lookat = vec3(0.0f, 0.0f, 0.0f);
    auto dist_to_focus = (lookfrom - lookat).length();
    auto aperture = 0.0f;
    auto cam = camera(lookfrom, lookat, vec3(0.0f, 1.0f, 0.0f),
                      20.0f,
                      float(render_settings.resolution_x) / float(render_settings.resolution_y),
                      aperture, dist_to_focus,
                      render_settings.shutter_open, render_settings.shutter_close);
    return std::move(cam); // seems to complain if I do std::move and if I don't
}


camera && two_sphere_cam(const RenderSettings &render_settings)
{
    auto two_sphere_lookfrom = vec3(13.0f, 2.0f, 3.0f);
    auto two_sphere_lookat = vec3(0.0f, 0.0f, 0.0f);
    auto two_sphere_dist_to_focus = 10.0f;
    auto two_sphere_aperture = 0.0f;
    auto cam = camera(two_sphere_lookfrom, two_sphere_lookat, vec3(0.0, 1.0, 0.0),
                  20.0f,
                  float(render_settings.resolution_x) / float(render_settings.resolution_y),
                  two_sphere_aperture, two_sphere_dist_to_focus,
                  render_settings.shutter_open, render_settings.shutter_close);
    return std::move(cam); // seems to complain if I do std::move and if I don't
}


camera && image_sphere_test_cam(const RenderSettings &render_settings)
{
    auto two_sphere_lookfrom = vec3(15.0f, 2.0f, 3.0f);
    auto two_sphere_lookat = vec3(0.0f, 2.0f, 0.0f);
    auto two_sphere_dist_to_focus = 10.0f;
    auto two_sphere_aperture = 0.0f;
    auto cam = camera(two_sphere_lookfrom, two_sphere_lookat, vec3(0.0, 1.0, 0.0),
                      20.0f,
                      float(render_settings.resolution_x) / float(render_settings.resolution_y),
                      two_sphere_aperture, two_sphere_dist_to_focus,
                      render_settings.shutter_open, render_settings.shutter_close);
    return std::move(cam); // seems to complain if I do std::move and if I don't
}


//////////////////////////////////////////////////////////////////////////
/// Scene generation
//////////////////////////////////////////////////////////////////////////

// One day, I'd like to get the scene from file,
// like USD and/or alembic or something.
// But for now, just get used to getting it from a module
// outside main.


std::tuple<hitable *, camera>
get_scene(const RenderSettings &render_settings)
{
//    auto w_tmp = two_marblelike_spheres();
//    auto c_tmp = two_sphere_cam(render_settings);

//    auto w_tmp = image_sphere_tests();
//    auto c_tmp = image_sphere_test_cam(render_settings);

    auto w_tmp = random_scene(0.0, 1.0, true);
    auto c_tmp = random_scene_cam(render_settings);
    return std::make_tuple(w_tmp, c_tmp);
}