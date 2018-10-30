//
// Created by Steve Hwan on 10/27/18.
//

#include "scene_generation.h"

#include <cmath>
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


Hitable *RandomScene(float t_min, float t_max, bool use_bvh) {
    // Hitable is an abstract base, so I wouldn't know how/what to allocate
    // up front, and there's not really a default placeholder.
    // So I want the vector of pointers, not vector of hitables.
    std::vector<std::shared_ptr<Hitable>> scene_list;

    Texture *checker = new CheckerTexture(
            new ConstantTexture(Vec3(0.2, 0.3, 0.1)),
            new ConstantTexture(Vec3(0.9, 0.9, 0.9))
            );
    scene_list.push_back( std::shared_ptr<Hitable>(
            new Sphere(Vec3(0.0f, -1000.0f, 0.0f), 1000.0f,
                       new Lambertian(checker))
     ));

    for (auto a: boost::irange(-10, 10)) {
        for (auto b: boost::irange(-10, 10)) {
            auto choose_mat = float(drand48());
            auto center = Vec3(a + 0.9f * drand48(), 0.2f, b + 0.9f * drand48());
            if (choose_mat < 0.8) {  // diffuse
                scene_list.push_back( std::shared_ptr<Hitable>(
                        new MovingSphere(center,
                                center + Vec3(0, 0.5 * drand48(), 0),
                                0.0, 1.0,
                                0.2f,
                                new Lambertian(new ConstantTexture(
                                        Vec3(drand48() * drand48(),
                                                drand48() * drand48(),
                                                drand48() * drand48()))))
                ));
            } else if (choose_mat < 0.95) {  // metal
                scene_list.push_back( std::shared_ptr<Hitable>(
                        new Sphere(center, 0.2,
                                new Metal(new ConstantTexture(
                                        Vec3(0.5 * (1.0f + drand48()),
                                                0.5 * (1.0f + drand48()),
                                                0.5 * (1.0f + drand48()))),
                                        0.5 * drand48()))
                ));
            } else {  // glass
                scene_list.push_back( std::shared_ptr<Hitable>(
                        new Sphere(center, 0.2f, new Dielectric(1.5f))
                ));
            }
        }
    }

    scene_list.push_back( std::shared_ptr<Hitable>(
            new Sphere(Vec3(0.0f, 1.0f, 0.0f), 1.0, new Dielectric(1.5))
    ));
    scene_list.push_back( std::shared_ptr<Hitable>(
            new Sphere(Vec3(-4.0f, 1.0f, 0.0f), 1.0f, new Lambertian(
                    new ConstantTexture(Vec3(0.4f, 0.2f, 0.2f))))
    ));
    scene_list.push_back( std::shared_ptr<Hitable>(
            new Sphere(Vec3(4.0f, 1.0f, 0.0f), 1.0f, new Metal(
                    new ConstantTexture(Vec3(0.7f, 0.6f, 0.5f)), 0.0f))
    ));

    if (use_bvh)
        return new BVHNode(scene_list.begin(), scene_list.end(), t_min, t_max);
    else
        return new HitableList(scene_list);
}

Hitable * TwoSpheres() {
    Texture *checker = new CheckerTexture(
            new ConstantTexture(Vec3(0.2, 0.3, 0.1)),
            new ConstantTexture(Vec3(0.9, 0.9, 0.9))
    );
    std::vector<std::shared_ptr<Hitable>> scene_list;
    scene_list.push_back( std::shared_ptr<Hitable>(
            new Sphere(Vec3(0, -10, 0), 10, new Lambertian(checker))
    ));
    scene_list.push_back( std::shared_ptr<Hitable>(
            new Sphere(Vec3(0, 10, 0), 10, new Lambertian(checker))
    ));

    return new HitableList(scene_list);
}

// Maybe change this to smart pointers so the assignement
// to lambertian can be reference counted, and the ownership
// better managed.
Hitable *TwoNoiseSpheres(Texture *noise_textured) {
    std::vector<std::shared_ptr<Hitable>> scene_list;
    // For now, assume C++11, not 14 so no "make_unique"
    scene_list.push_back( std::shared_ptr<Hitable>(
            new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(noise_textured))
    ));
    scene_list.push_back( std::shared_ptr<Hitable>(
            new Sphere(Vec3(0, 2, 0), 2, new Lambertian(noise_textured))
    ));

    return new HitableList(scene_list);
}

Hitable *TwoPerlinSpheres() {
    Texture *noise_textured = new NoiseTexture(4.0f);
    return TwoNoiseSpheres(noise_textured);
}

Hitable *TwoTurbulenceSpheres() {
    Texture *noise_textured = new TurbulenceTexture(4.0f);
    return TwoNoiseSpheres(noise_textured);
}

Hitable *TwoMarblelikeSpheres() {
    Texture *noise_textured = new MarblelikeTexture(4.0f);
    return TwoNoiseSpheres(noise_textured);
}

Hitable *ImageSphereTests() {
    std::string test_filename = "test_data/land_ocean_ice_cloud_2048.jpg";
    Texture *noise_textured = new MarblelikeTexture(4.0f);
    Texture *image_textured = new ImageTexture(test_filename);

    std::vector<std::shared_ptr<Hitable>> scene_list;
    // For now, assume C++11, not 14 so no "make_unique"
    // Big sphere - let's keep the marble
    scene_list.push_back( std::shared_ptr<Hitable>(
            new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(noise_textured))
    ));

    scene_list.push_back( std::shared_ptr<Hitable>(
            new Sphere(Vec3(0, 2, 0), 2, new ConstantMaterial(image_textured))
    ));

    return new HitableList(scene_list);
}


//////////////////////////////////////////////////////////////////////////
/// Cameras
//////////////////////////////////////////////////////////////////////////


Camera &&RandomSceneCam(const RenderSettings &render_settings)
{
    auto lookfrom = Vec3(13.0f, 2.0f, 3.0f);
    auto lookat = Vec3(0.0f, 0.0f, 0.0f);
    auto dist_to_focus = (lookfrom - lookat).length();
    auto aperture = 0.0f;
    auto cam = Camera(lookfrom, lookat,
            Vec3(0.0f, 1.0f, 0.0f),
            20.0f,
            float(render_settings.resolution_x_)
                    / float(render_settings.resolution_y_),
            aperture, dist_to_focus,
            render_settings.shutter_open_, render_settings.shutter_close_);
    return std::move(cam); // seems to complain if I do std::move and if I don't
}


Camera &&TwoSphereCam(const RenderSettings &render_settings)
{
    auto two_sphere_lookfrom = Vec3(13.0f, 2.0f, 3.0f);
    auto two_sphere_lookat = Vec3(0.0f, 0.0f, 0.0f);
    auto two_sphere_dist_to_focus = 10.0f;
    auto two_sphere_aperture = 0.0f;
    auto cam = Camera(two_sphere_lookfrom, two_sphere_lookat,
            Vec3(0.0, 1.0, 0.0),
            20.0f,
            float(render_settings.resolution_x_)
                    / float(render_settings.resolution_y_),
            two_sphere_aperture, two_sphere_dist_to_focus,
            render_settings.shutter_open_, render_settings.shutter_close_);
    return std::move(cam); // seems to complain if I do std::move and if I don't
}


Camera &&ImageSphereTestCam(const RenderSettings &render_settings)
{
    auto two_sphere_lookfrom = Vec3(15.0f, 2.0f, 3.0f);
    auto two_sphere_lookat = Vec3(0.0f, 2.0f, 0.0f);
    auto two_sphere_dist_to_focus = 10.0f;
    auto two_sphere_aperture = 0.0f;
    auto cam = Camera(two_sphere_lookfrom, two_sphere_lookat,
            Vec3(0.0, 1.0, 0.0),
            20.0f,
            float(render_settings.resolution_x_)
                    / float(render_settings.resolution_y_),
            two_sphere_aperture, two_sphere_dist_to_focus,
            render_settings.shutter_open_, render_settings.shutter_close_);
    return std::move(cam); // seems to complain if I do std::move and if I don't
}


//////////////////////////////////////////////////////////////////////////
/// Scene generation
//////////////////////////////////////////////////////////////////////////

// One day, I'd like to get the scene from file,
// like USD and/or alembic or something.
// But for now, just get used to getting it from a module
// outside main.


std::tuple<Hitable *, Camera> GetScene(const RenderSettings &render_settings) {
//    auto w_tmp = TwoMarblelikeSpheres();
//    auto c_tmp = TwoSphereCam(render_settings);

//    auto w_tmp = ImageSphereTests();
//    auto c_tmp = ImageSphereTestCam(render_settings);

    auto w_tmp = RandomScene(0.0, 1.0, true);
    auto c_tmp = RandomSceneCam(render_settings);

    return std::make_tuple(w_tmp, c_tmp);
}