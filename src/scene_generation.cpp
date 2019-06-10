//
// Created by Steve Hwan on 10/27/18.
//

#include "scene_generation.h"

#include <cmath>
#include <memory>
#include <vector>

#include "vec3.h"
#include "ray_engine/hitable.h"
#include "ray_engine/hitable_list.h"
#include "ray_engine/bvh_root.h"
#include "scene_geometry/sphere.h"
#include "scene_geometry/moving_sphere.h"
#include "scene_geometry/rect.h"
#include "scene_geometry/box.h"
#include "scene_geometry/transforms/flip_normals.h"
#include "scene_geometry/transforms/translate.h"
#include "scene_geometry/transforms/rotate.h"
#include "scene_geometry/volumes/constant_medium.h"
#include "textures/texture.h"
#include "textures/constant_texture.h"
#include "textures/checker_texture.h"
#include "textures/noise_texture.h"
#include "textures/image_texture.h"
#include "materials/lambertian.h"
#include "materials/metal.h"
#include "materials/dielectric.h"
//#include "materials/constant_material.h"
#include "materials/diffuse_light.h"


void RandomScene(RenderContext &render_context,
                 float t_min, float t_max, bool use_bvh) {
    // Hitable is an abstract base, so I wouldn't know how/what to allocate
    // up front, and there's not really a default placeholder.
    // So I want the vector of pointers, not vector of hitables.
    std::vector<std::shared_ptr<Hitable>> scene_list;

    Texture *checker = new CheckerTexture(
            new ConstantTexture(Vec3(0.2, 0.3, 0.1)),
            new ConstantTexture(Vec3(0.9, 0.9, 0.9))
            );
    scene_list.push_back(std::make_shared<Sphere>(
            Vec3(0.0f, -1000.0f, 0.0f), 1000.0f,
                       new Lambertian(checker)
     ));

    for (auto a: boost::irange(-10, 10)) {
        for (auto b: boost::irange(-10, 10)) {
            auto choose_mat = float(drand48());
            auto center = Vec3(a + 0.9f * drand48(), 0.2f, b + 0.9f * drand48());
            if (choose_mat < 0.8) {  // diffuse
                scene_list.push_back(std::make_shared<MovingSphere>(
                        center, center + Vec3(0, 0.5 * drand48(), 0),
                        0.0, 1.0, 0.2f,
                        new Lambertian(new ConstantTexture(
                                Vec3(drand48() * drand48(),
                                     drand48() * drand48(),
                                     drand48() * drand48())))
                ));
            } else if (choose_mat < 0.95) {  // metal
                scene_list.push_back(std::make_shared<Sphere>(
                        center, 0.2,
                        new Metal(new ConstantTexture(
                                Vec3(0.5 * (1.0f + drand48()),
                                     0.5 * (1.0f + drand48()),
                                     0.5 * (1.0f + drand48()))),
                                0.5 * drand48())
                ));
            } else {  // glass
                scene_list.push_back(std::make_shared<Sphere>(
                        center, 0.2f, new Dielectric(1.5f)
                ));
            }
        }
    }

    scene_list.push_back(std::make_shared<Sphere>(
            Vec3(0.0f, 1.0f, 0.0f), 1.0, new Dielectric(1.5)
    ));
    scene_list.push_back(std::make_shared<Sphere>(
            Vec3(-4.0f, 1.0f, 0.0f), 1.0f, new Lambertian(
                    new ConstantTexture(Vec3(0.4f, 0.2f, 0.2f)))
    ));
    scene_list.push_back(std::make_shared<Sphere>(
            Vec3(4.0f, 1.0f, 0.0f), 1.0f, new Metal(
                    new ConstantTexture(Vec3(0.7f, 0.6f, 0.5f)), 0.0f)
    ));

    if (use_bvh)
        render_context.world_ptr = std::unique_ptr<BVHRoot>(new BVHRoot(scene_list, t_min, t_max));
    else
        render_context.world_ptr = std::unique_ptr<HitableList>(new HitableList(scene_list));
}

void TwoSpheres(RenderContext &render_context) {
    Texture *checker = new CheckerTexture(
            new ConstantTexture(Vec3(0.2, 0.3, 0.1)),
            new ConstantTexture(Vec3(0.9, 0.9, 0.9))
    );
    std::vector<std::shared_ptr<Hitable>> scene_list;
    scene_list.push_back(std::make_shared<Sphere>(
            Vec3(0, -10, 0), 10, new Lambertian(checker)
    ));
    scene_list.push_back(std::make_shared<Sphere>(
            Vec3(0, 10, 0), 10, new Lambertian(checker)
    ));

    render_context.world_ptr = std::unique_ptr<HitableList>(new HitableList(scene_list));
}

// Maybe change this to smart pointers so the assignement
// to lambertian can be reference counted, and the ownership
// better managed.
void TwoNoiseSpheres(RenderContext &render_context,
                     Texture *noise_textured) {
    std::vector<std::shared_ptr<Hitable>> scene_list;
    // For now, assume C++11, not 14 so no "make_unique"
    scene_list.push_back(std::make_shared<Sphere>(
            Vec3(0, -1000, 0), 1000, new Lambertian(noise_textured)
    ));
    scene_list.push_back(std::make_shared<Sphere>(
            Vec3(0, 2, 0), 2, new Lambertian(noise_textured)
    ));

    render_context.world_ptr = std::unique_ptr<HitableList>(new HitableList(scene_list));
}

void TwoPerlinSpheres(RenderContext &render_context) {
    Texture *noise_textured = new NoiseTexture(4.0f);
    TwoNoiseSpheres(render_context, noise_textured);
}

void TwoTurbulenceSpheres(RenderContext &render_context) {
    Texture *noise_textured = new TurbulenceTexture(4.0f);
    TwoNoiseSpheres(render_context, noise_textured);
}

void TwoMarblelikeSpheres(RenderContext &render_context) {
    Texture *noise_textured = new MarblelikeTexture(4.0f);
    TwoNoiseSpheres(render_context, noise_textured);
}

void ImageSphereTests(RenderContext &render_context) {
    std::string test_filename = "test_data/land_ocean_ice_cloud_2048.jpg";
    Texture *noise_textured = new MarblelikeTexture(4.0f);
    Texture *image_textured = new ImageTexture(test_filename);

    std::vector<std::shared_ptr<Hitable>> scene_list;
    // For now, assume C++11, not 14 so no "make_unique"
    // Big sphere - let's keep the marble
    scene_list.push_back(std::make_shared<Sphere>(
            Vec3(0, -1000, 0), 1000, new Lambertian(noise_textured)
    ));
    auto light_geometry_ptr = std::make_shared<Sphere>(
      Vec3(0, 2, 0), 2, new DiffuseLight(image_textured));
    scene_list.push_back(light_geometry_ptr);
    render_context.hitable_light_list.push_back(light_geometry_ptr);

    render_context.world_ptr = std::unique_ptr<HitableList>(new HitableList(scene_list));
}

void SimpleLightTest(RenderContext &render_context) {
    std::string test_filename = "test_data/land_ocean_ice_cloud_2048.jpg";
    Texture *noise_textured = new MarblelikeTexture(4.0f);
    Texture *image_textured = new ImageTexture(test_filename);

    std::vector<std::shared_ptr<Hitable>> scene_list;
    // For now, assume C++11, not 14 so no "make_unique"
    // Big sphere - let's keep the marble
    scene_list.push_back(std::make_shared<Sphere>(
            Vec3(0, -1000, 0), 1000, new Lambertian(noise_textured)
    ));
    scene_list.push_back(std::make_shared<Sphere>(
            Vec3(0, 2, 0), 2, new Lambertian(image_textured)
    ));
    auto light_geometry_sphere_ptr = std::make_shared<Sphere>(
            Vec3(0, 7, 0), 2, new DiffuseLight(
                    new ConstantTexture(Vec3(4.0f, 4.0f, 4.0f)))
    );
    scene_list.push_back(light_geometry_sphere_ptr);
    auto light_geometry_plane_ptr = std::make_shared<XYRect>(
            3.0f, 5.0f, 1.0f, 3.0f, -2.0f, new DiffuseLight(
                    new ConstantTexture(Vec3(4.0f, 4.0f, 4.0f)))
    );
    scene_list.push_back(light_geometry_plane_ptr);

    render_context.hitable_light_list.push_back(light_geometry_sphere_ptr);
    render_context.hitable_light_list.push_back(light_geometry_plane_ptr);

    render_context.world_ptr = std::unique_ptr<HitableList>(new HitableList(scene_list));
}


void CornellBox(RenderContext &render_context) {
    std::vector<std::shared_ptr<Hitable>> scene_list;

    Material *red = new Lambertian(
            new ConstantTexture(Vec3(0.65f, 0.05f, 0.05f)));
    Material *white = new Lambertian(
            new ConstantTexture(Vec3(0.73f, 0.73f, 0.73f)));
    Material *green = new Lambertian(
            new ConstantTexture(Vec3(0.12f, 0.45f, 0.15f)));

    Material *light = new DiffuseLight(
            new ConstantTexture(Vec3(15.0f, 15.0f, 15.0f)));

    // light
    auto light_geometry_ptr = std::make_shared<XZRect>(213, 343, 227, 332, 554,
                                                       light);
    scene_list.push_back(
        std::make_shared<FlipNormals>(light_geometry_ptr)
    );
    render_context.hitable_light_list.push_back(light_geometry_ptr);

    // main room walls
    scene_list.push_back(
        std::make_shared<FlipNormals>(
            std::make_shared<YZRect>(0, 555, 0, 555, 555, green)
        )
    );
    scene_list.push_back(
        std::make_shared<YZRect>(0, 555, 0, 555, 0, red)
    );
    scene_list.push_back(
        std::make_shared<FlipNormals>(
            std::make_shared<XZRect>(0, 555, 0, 555, 555, white)
        )
    );
    scene_list.push_back(std::make_shared<XZRect>(
            0, 555, 0, 555, 0, white
    ));
    scene_list.push_back(
        std::make_shared<FlipNormals>(
            std::make_shared<XYRect>(0, 555, 0, 555, 555, white)
        )
    );

    // 2 boxes
    scene_list.push_back(
        std::make_shared<Translate>(Vec3(130, 0, 65),
            std::make_shared<RotateY>(-18,
                std::make_shared<Box>(
                    Vec3(0, 0, 0), Vec3(165, 165, 165), white)
            )
        )
    );
    scene_list.push_back(
        std::make_shared<Translate>(Vec3(265, 0, 295),
            std::make_shared<RotateY>(15,
                std::make_shared<Box>(
                    Vec3(0, 0, 0), Vec3(165, 330, 165), white)
            )
        )
    );

    render_context.world_ptr = std::unique_ptr<HitableList>(new HitableList(scene_list));
}


void CornellBoxAluminum(RenderContext &render_context) {
    std::vector<std::shared_ptr<Hitable>> scene_list;

    Material *red = new Lambertian(
        new ConstantTexture(Vec3(0.65f, 0.05f, 0.05f)));
    Material *white = new Lambertian(
        new ConstantTexture(Vec3(0.73f, 0.73f, 0.73f)));
    Material *green = new Lambertian(
        new ConstantTexture(Vec3(0.12f, 0.45f, 0.15f)));
    Material *alumninum = new Metal(
        new ConstantTexture(Vec3(0.8, 0.85, 0.88)), 0.0);

    Material *light = new DiffuseLight(
        new ConstantTexture(Vec3(15.0f, 15.0f, 15.0f)));

    // light
    auto light_geometry_ptr =
        std::make_shared<FlipNormals>(
            std::make_shared<XZRect>(213, 343, 227, 332, 554, light)
        );
    scene_list.push_back(light_geometry_ptr);
    render_context.hitable_light_list.push_back(light_geometry_ptr);

    // main room walls
    scene_list.push_back(
        std::make_shared<FlipNormals>(
            std::make_shared<YZRect>(0, 555, 0, 555, 555, green)
        )
    );
    scene_list.push_back(std::make_shared<YZRect>(0, 555, 0, 555, 0, red));
    scene_list.push_back(
        std::make_shared<FlipNormals>(
            std::make_shared<XZRect>(0, 555, 0, 555, 555, white)
        )
    );
    scene_list.push_back(std::make_shared<XZRect>(0, 555, 0, 555, 0, white));
    scene_list.push_back(
        std::make_shared<FlipNormals>(
            std::make_shared<XYRect>(0, 555, 0, 555, 555, white)
        )
    );

    // 2 boxes
    scene_list.push_back(
        std::make_shared<Translate>(Vec3(130, 0, 65),
            std::make_shared<RotateY>(-18,
                std::make_shared<Box>(Vec3(0, 0, 0), Vec3(165, 165, 165), white)
            )
        )
    );
    scene_list.push_back(
        std::make_shared<Translate>(Vec3(265, 0, 295),
              std::make_shared<RotateY>(15,
                    std::make_shared<Box>(
                        Vec3(0, 0, 0), Vec3(165, 330, 165), alumninum)
              )
        )
    );

    render_context.world_ptr = std::unique_ptr<HitableList>( new HitableList(scene_list));
}


void CornellBoxGlassSphere(RenderContext &render_context) {
  std::vector<std::shared_ptr<Hitable>> scene_list;

  Material *red = new Lambertian(
      new ConstantTexture(Vec3(0.65f, 0.05f, 0.05f)));
  Material *white = new Lambertian(
      new ConstantTexture(Vec3(0.73f, 0.73f, 0.73f)));
  Material *green = new Lambertian(
      new ConstantTexture(Vec3(0.12f, 0.45f, 0.15f)));

  Material *light = new DiffuseLight(
      new ConstantTexture(Vec3(15.0f, 15.0f, 15.0f)));

  // light
//  auto light_geometry_ptr =
//      std::make_shared<FlipNormals>(
//          std::make_shared<XZRect>(213, 343, 227, 332, 554, light)
//      );
  auto light_geometry_ptr =
      std::make_shared<XZRect>(213, 343, 227, 332, 554, light);
  auto flipped_light_geometry_ptr =
      std::make_shared<FlipNormals>(light_geometry_ptr);
  scene_list.push_back(flipped_light_geometry_ptr);
  render_context.hitable_light_list.push_back(flipped_light_geometry_ptr);

  // main room walls
  scene_list.push_back(
      std::make_shared<FlipNormals>(
          std::make_shared<YZRect>(0, 555, 0, 555, 555, green)
      )
  );
  scene_list.push_back(
      std::make_shared<YZRect>(0, 555, 0, 555, 0, red)
  );
  scene_list.push_back(
      std::make_shared<FlipNormals>(
          std::make_shared<XZRect>(0, 555, 0, 555, 555, white)
      )
  );
  scene_list.push_back(std::make_shared<XZRect>(0, 555, 0, 555, 0, white));
  scene_list.push_back(
      std::make_shared<FlipNormals>(
          std::make_shared<XYRect>(0, 555, 0, 555, 555, white)
      )
  );

  // tall box and a glass sphere
  auto glass_geometry_ptr =
      std::make_shared<Sphere>(Vec3(190, 90, 190), 90, new Dielectric(1.5f));
  scene_list.push_back(glass_geometry_ptr);
  // TODO: separate the dielectric from the lights.
  render_context.hitable_light_list.push_back(glass_geometry_ptr);
  scene_list.push_back(
      std::make_shared<Translate>(Vec3(265, 0, 295),
            std::make_shared<RotateY>(15,
                  std::make_shared<Box>(
                      Vec3(0, 0, 0), Vec3(165, 330, 165), white)
            )
      )
  );

  render_context.world_ptr = std::unique_ptr<HitableList>(new HitableList(scene_list));
}



void CornellSmoke(RenderContext &render_context) {
    std::vector<std::shared_ptr<Hitable>> scene_list;

    Material *red = new Lambertian(
            new ConstantTexture(Vec3(0.65f, 0.05f, 0.05f)));
    Material *white = new Lambertian(
            new ConstantTexture(Vec3(0.73f, 0.73f, 0.73f)));
    Material *green = new Lambertian(
            new ConstantTexture(Vec3(0.12f, 0.45f, 0.15f)));

    Material *light = new DiffuseLight(
            new ConstantTexture(Vec3(7.0f, 7.0f, 7.0f)));

    // light
    auto light_geometry_ptr = std::make_shared<XZRect>(113, 443, 127, 432, 554, light);
    scene_list.push_back(light_geometry_ptr);
    render_context.hitable_light_list.push_back(light_geometry_ptr);

    // main room walls
    scene_list.push_back(
        std::make_shared<FlipNormals>(
            std::make_shared<YZRect>(0, 555, 0, 555, 555, green)
        )
    );
    scene_list.push_back(std::make_shared<YZRect>(0, 555, 0, 555, 0, red));
    scene_list.push_back(
        std::make_shared<FlipNormals>(
            std::make_shared<XZRect>(0, 555, 0, 555, 555, white))
    );
    scene_list.push_back(std::make_shared<XZRect>(0, 555, 0, 555, 0, white));
    scene_list.push_back(
        std::make_shared<FlipNormals>(
            std::make_shared<XYRect>(0, 555, 0, 555, 555, white)
        )
    );

    // 2 boxes
    std::shared_ptr<Translate> box_1 {new Translate(Vec3(130, 0, 65),
        std::make_shared<RotateY>(-18,
            std::make_shared<Box>(
                Vec3(0, 0, 0), Vec3(165, 165, 165), white)
        )
    ) };
    std::shared_ptr<Translate> box_2 {new Translate(Vec3(265, 0, 295),
        std::make_shared<RotateY>(15,
            std::make_shared<Box>(
                Vec3(0, 0, 0), Vec3(165, 330, 165), white)
        )
    )};
    scene_list.push_back(std::shared_ptr<Hitable>(
            new ConstantMedium(box_1, 0.01,
                    new ConstantTexture(Vec3(1.0, 1.0, 1.0)))
    ));
    scene_list.push_back(std::shared_ptr<Hitable>(
            new ConstantMedium(box_2, 0.01,
                    new ConstantTexture(Vec3(0.0, 0.0, 0.0)))
    ));

    render_context.world_ptr = std::unique_ptr<HitableList>(new HitableList(scene_list));
}


void TestAllBook2(RenderContext &render_context) {
    std::vector<std::shared_ptr<Hitable>> scene_list;

    std::vector<std::shared_ptr<Hitable>> box_list_1;
    std::vector<std::shared_ptr<Hitable>> box_list_2;

    Material *white = new Lambertian(
            new ConstantTexture(Vec3(0.73f, 0.73f, 0.73f)));
    Material *ground = new Lambertian(
            new ConstantTexture(Vec3(0.48f, 0.83f, 0.53f)));

    auto num_boxes = 20;
    for (auto i: boost::irange(0, num_boxes)) {
        for (auto j: boost::irange(0, num_boxes)) {
            auto w = 100.0f;
            auto x0 = -1000.0f + i * w;
            auto z0 = -1000.0f + j * w;
            auto y0 = 0.0f;
            auto x1 = x0 + w;
            auto y1 = 100.0f * float(drand48() + 0.01f);
            auto z1  = z0 + w;
            box_list_1.push_back( std::make_shared<Box>(
                    Vec3(x0, y0, z0), Vec3(x1, y1, z1), ground
                    ));
        }
    }

    scene_list.push_back(std::make_shared<BVHRoot>(box_list_1, 0, 1));

    // light
    Material *light = new DiffuseLight(
            new ConstantTexture(Vec3(7.0f, 7.0f, 7.0f)));

    auto light_geometry_ptr =
        std::make_shared<FlipNormals>(
            std::make_shared<XZRect>(123, 423, 147, 412, 554, light)
        );
    scene_list.push_back(light_geometry_ptr);
    render_context.hitable_light_list.push_back(light_geometry_ptr);

    Vec3 center(400.0f, 400.0f, 200.0f);
    scene_list.push_back(std::make_shared<MovingSphere>(
            center, center + Vec3(30.0f, 0.0f, 0.0f),
            0, 1, 50.0,
            new Lambertian(new ConstantTexture(Vec3(0.7f, 0.3f, 0.1f)))
    ));
    // TODO: separate the dielectric from the lights.
    auto dielectric_geometry_ptr = std::make_shared<Sphere>(
            Vec3(260.0f, 150.0f, 45.0f), 50.0f, new Dielectric(1.5f)
    );
    scene_list.push_back(dielectric_geometry_ptr);
//    render_context.hitable_light_list.push_back(dielectric_geometry_ptr);

    scene_list.push_back(std::make_shared<Sphere>(
            Vec3(0.0f, 150.0f, 145.0f), 50.0f,
            new Metal(new ConstantTexture(Vec3(0.2f, 0.4f, 0.9f)), 10.0f)
    ));

    std::shared_ptr<Sphere> boundary = std::make_shared<Sphere>(
        Vec3(360.0f, 150.0f, 145.0f), 70.0f, new Dielectric(1.5f));
    scene_list.push_back(boundary);
    scene_list.push_back(std::make_shared<ConstantMedium>(
            boundary, 0.2, new ConstantTexture(Vec3(0.2f, 0.4f, 0.9f))
    ));

    // This kind of scares me. I guess I've implicitly transferred ownership
    // of the old version to scene_list.  But still... reusing...
    boundary = std::make_shared<Sphere>(Vec3(0.0f, 0.0f, 5.0f), 5000.0f,
                                   new Dielectric(1.5f));
    scene_list.push_back(std::make_shared<ConstantMedium>(
            boundary, 0.0001, new ConstantTexture(Vec3(1.0f, 1.0f, 1.0f))
    ));

    std::string test_filename = "test_data/land_ocean_ice_cloud_2048.jpg";
    Texture *image_textured = new ImageTexture(test_filename);
    Material *earth_material = new Lambertian(image_textured);
    scene_list.push_back(std::make_shared<Sphere>(
            Vec3(400.0f, 200.0f, 300.0f), 80.0f, earth_material
    ));

    Texture *perlin_texture = new NoiseTexture(0.1);
    scene_list.push_back(std::make_shared<Sphere>(
            Vec3(220.0f, 280.0f, 300.0f), 80.0f, new Lambertian(perlin_texture)
    ));

    auto ns = 1000;
    for (auto j: boost::irange(0, ns)) {
        box_list_2.push_back(std::make_shared<Sphere>(
                Vec3(165.0 * drand48(), 165.0 * drand48(), 165.0 * drand48()),
                10.0f, white
        ));
    }
    scene_list.push_back(
        std::make_shared<Translate>(Vec3(-100, 270, 395),
            std::make_shared<RotateY>(15.0f,
                std::make_shared<BVHRoot>(box_list_2, 0.0, 1.0)
            )
        )
    );

    render_context.world_ptr = std::unique_ptr<HitableList>(new HitableList(scene_list));
}


void TestRotYBoxes(RenderContext &render_context) {
    std::vector<std::shared_ptr<Hitable>> scene_list;

    std::vector<std::shared_ptr<Hitable>> box_list_1;
    std::vector<std::shared_ptr<Hitable>> box_list_2;

    Material *ground = new Lambertian(
            new ConstantTexture(Vec3(0.48f, 0.83f, 0.53f)));

    auto num_boxes = 20;
    for (auto i: boost::irange(0, num_boxes)) {
        for (auto j: boost::irange(0, num_boxes)) {
            auto w = 100.0f;
            auto x0 = -1000.0f + i * w;
            auto z0 = -1000.0f + j * w;
            auto y0 = 400.0f * float(drand48() + 0.01f);
            auto x1 = x0 + w/2;
            auto y1 = y0 + w/3;
            auto z1  = z0 + w/2;
            auto roty = float(drand48() * 90);
            box_list_1.push_back(
              std::make_shared<RotateY>(roty,
                  std::make_shared<Box>(
                       Vec3(x0, y0, z0), Vec3(x1, y1, z1), ground
              )));
        }
    }

    scene_list.push_back(std::make_shared<BVHRoot>(box_list_1, 0, 1));

    // light
    Material *light = new DiffuseLight(
            new ConstantTexture(Vec3(7.0f, 7.0f, 7.0f)));

    auto light_geometry_ptr =
            std::make_shared<FlipNormals>(
                    std::make_shared<XZRect>(123, 423, 147, 412, 554, light)
            );
    scene_list.push_back(light_geometry_ptr);
    render_context.hitable_light_list.push_back(light_geometry_ptr);

    std::string test_filename = "test_data/land_ocean_ice_cloud_2048.jpg";
    Texture *image_textured = new ImageTexture(test_filename);
    Material *earth_material = new Lambertian(image_textured);
    scene_list.push_back(std::make_shared<Sphere>(
            Vec3(400.0f, 200.0f, 300.0f), 80.0f, earth_material
    ));

    Texture *perlin_texture = new NoiseTexture(0.1);
    scene_list.push_back(std::make_shared<Sphere>(
            Vec3(220.0f, 280.0f, 300.0f), 80.0f, new Lambertian(perlin_texture)
    ));

    render_context.world_ptr = std::unique_ptr<HitableList>(new HitableList(scene_list));
}


void TestRotBoxes2(RenderContext &render_context) {
    std::vector<std::shared_ptr<Hitable>> scene_list;

    std::vector<std::shared_ptr<Hitable>> box_list_1;
    std::vector<std::shared_ptr<Hitable>> box_list_2;

    Material *ground = new Lambertian(
            new ConstantTexture(Vec3(0.48f, 0.83f, 0.53f)));

    auto num_boxes = 20;
    for (auto i: boost::irange(0, num_boxes)) {
        for (auto j: boost::irange(0, num_boxes)) {
            auto w = 100.0f;
            auto x0 = -1000.0f + i * w;
            auto z0 = -1000.0f + j * w;
            auto y0 = 400.0f * float(drand48() + 0.01f);
            auto rot_angle = float(drand48() * 90);
            auto axis = drand48();
            if (axis < 0.333333) {
                box_list_1.push_back(
                        std::make_shared<Translate>(Vec3(x0, y0, z0),
                                                    std::make_shared<RotateY>(rot_angle,
                                                                              std::make_shared<Box>(
                                                                                      Vec3(-w / 4, -w / 6, -w / 4),
                                                                                      Vec3(w / 4, w / 6, w / 4), ground
                                                                              ))));
            } else if (axis < 0.6666666) {
                box_list_1.push_back(
                        std::make_shared<Translate>(Vec3(x0, y0, z0),
                                                    std::make_shared<RotateX>(rot_angle,
                                                                              std::make_shared<Box>(
                                                                                      Vec3(-w / 4, -w / 6, -w / 4),
                                                                                      Vec3(w / 4, w / 6, w / 4), ground
                                                                              ))));

            } else {
                box_list_1.push_back(
                        std::make_shared<Translate>(Vec3(x0, y0, z0),
                                                    std::make_shared<RotateZ>(rot_angle,
                                                                              std::make_shared<Box>(
                                                                                      Vec3(-w / 4, -w / 6, -w / 4),
                                                                                      Vec3(w / 4, w / 6, w / 4), ground
                                                                              ))));

            }
        }
    }

    scene_list.push_back(std::make_shared<BVHRoot>(box_list_1, 0, 1));

    // light
    Material *light = new DiffuseLight(
            new ConstantTexture(Vec3(7.0f, 7.0f, 7.0f)));

    auto light_geometry_ptr =
            std::make_shared<FlipNormals>(
                    std::make_shared<XZRect>(123, 423, 147, 412, 554, light)
            );
    scene_list.push_back(light_geometry_ptr);
    render_context.hitable_light_list.push_back(light_geometry_ptr);

    std::string test_filename = "test_data/land_ocean_ice_cloud_2048.jpg";
    Texture *image_textured = new ImageTexture(test_filename);
    Material *earth_material = new Lambertian(image_textured);
    scene_list.push_back(std::make_shared<Sphere>(
            Vec3(400.0f, 200.0f, 300.0f), 80.0f, earth_material
    ));

    Texture *perlin_texture = new NoiseTexture(0.1);
    scene_list.push_back(std::make_shared<Sphere>(
            Vec3(220.0f, 280.0f, 300.0f), 80.0f, new Lambertian(perlin_texture)
    ));

    render_context.world_ptr = std::unique_ptr<HitableList>(new HitableList(scene_list));
}


//////////////////////////////////////////////////////////////////////////
/// Cameras
//////////////////////////////////////////////////////////////////////////
// 11/24/2018 sh-coursework - changed from right handed references
// to (unique) pointer. It feels more factory-ish.


void RandomSceneCam(RenderContext &render_context)
{
  auto render_settings = render_context.render_settings;
  auto lookfrom = Vec3(13.0f, 2.0f, 3.0f);
  auto lookat = Vec3(0.0f, 0.0f, 0.0f);
  auto dist_to_focus = (lookfrom - lookat).length();
  auto aperture = 0.0f;
  render_context.camera_ptr = std::unique_ptr<Camera>(
      new Camera(lookfrom, lookat,
                 Vec3(0.0f, 1.0f, 0.0f),
                 20.0f,
                 float(render_settings.resolution_x_)
                     / float(render_settings.resolution_y_),
                 aperture, dist_to_focus,
                 render_settings.shutter_open_, render_settings.shutter_close_));
}


void TwoSphereCam(RenderContext &render_context)
{
  auto render_settings = render_context.render_settings;
  auto two_sphere_lookfrom = Vec3(13.0f, 2.0f, 3.0f);
  auto two_sphere_lookat = Vec3(0.0f, 0.0f, 0.0f);
  auto two_sphere_dist_to_focus = 10.0f;
  auto two_sphere_aperture = 0.0f;
  render_context.camera_ptr = std::unique_ptr<Camera>(
      new Camera(two_sphere_lookfrom, two_sphere_lookat,
            Vec3(0.0, 1.0, 0.0),
            20.0f,
            float(render_settings.resolution_x_)
                    / float(render_settings.resolution_y_),
            two_sphere_aperture, two_sphere_dist_to_focus,
            render_settings.shutter_open_, render_settings.shutter_close_));
}


void ImageSphereTestCam(RenderContext &render_context)
{
  auto render_settings = render_context.render_settings;
  auto two_sphere_lookfrom = Vec3(15.0f, 2.0f, 3.0f);
  auto two_sphere_lookat = Vec3(0.0f, 2.0f, 0.0f);
  auto two_sphere_dist_to_focus = 10.0f;
  auto two_sphere_aperture = 0.0f;
  render_context.camera_ptr = std::unique_ptr<Camera>(
      new Camera(two_sphere_lookfrom, two_sphere_lookat,
            Vec3(0.0, 1.0, 0.0),
            20.0f,
            float(render_settings.resolution_x_)
                    / float(render_settings.resolution_y_),
            two_sphere_aperture, two_sphere_dist_to_focus,
            render_settings.shutter_open_, render_settings.shutter_close_));
}


void SimpleLightTestCam(RenderContext &render_context)
{
  auto render_settings = render_context.render_settings;
  auto two_sphere_lookfrom = Vec3(22.0f, 2.0f, 3.0f);
  auto two_sphere_lookat = Vec3(0.0f, 2.0f, 0.0f);
  auto two_sphere_dist_to_focus = 10.0f;
  auto two_sphere_aperture = 0.0f;
  render_context.camera_ptr = std::unique_ptr<Camera>(
      new Camera(two_sphere_lookfrom, two_sphere_lookat,
            Vec3(0.0, 1.0, 0.0),
            20.0f,
            float(render_settings.resolution_x_)
                    / float(render_settings.resolution_y_),
            two_sphere_aperture, two_sphere_dist_to_focus,
            render_settings.shutter_open_, render_settings.shutter_close_));
}


void CornellBoxCam(RenderContext &render_context)
{
  auto render_settings = render_context.render_settings;
  auto cornell_box_lookfrom = Vec3(278.0f, 278.0f, -800.0f);
  auto cornell_box_lookat = Vec3(278.0f, 278.0f, 0.0f);
  auto cornell_box_dist_to_focus = 10.0f;
  auto cornell_box_aperture = 0.0f;
  render_context.camera_ptr = std::unique_ptr<Camera>(
      new Camera(
            cornell_box_lookfrom, cornell_box_lookat,
            Vec3(0.0, 1.0, 0.0),
            40.0f,
            float(render_settings.resolution_x_)
                    / float(render_settings.resolution_y_),
            cornell_box_aperture, cornell_box_dist_to_focus,
            render_settings.shutter_open_, render_settings.shutter_close_));
}


//////////////////////////////////////////////////////////////////////////
/// Scene generation
//////////////////////////////////////////////////////////////////////////

// One day, I'd like to get the scene from file,
// like USD and/or alembic or something.
// But for now, just get used to getting it from a module
// outside main.


//std::tuple<std::unique_ptr<Hitable>, std::unique_ptr<Camera>>
//GetScene(const RenderSettings &render_settings) {
void
GetScene(RenderContext &render_context) {
//    TwoMarblelikeSpheres(render_context);
//    TwoSphereCam(render_context);

//    ImageSphereTests(render_context);
//    ImageSphereTestCam(render_context);

//    SimpleLightTest(render_context);
//    SimpleLightTestCam(render_context);

//    RandomScene(render_context, 0.0, 1.0, true);
//    RandomSceneCam(render_context);

//    CornellBox(render_context);
//    CornellBoxCam(render_context);

//    CornellBoxAluminum(render_context);
//    CornellBoxCam(render_context);

//    CornellBoxGlassSphere(render_context);
//    CornellBoxCam(render_context);

//    CornellSmoke(render_context);
//    CornellBoxCam(render_context);

    TestAllBook2(render_context);
    CornellBoxCam(render_context);

//    TestRotYBoxes(render_context);
//    CornellBoxCam(render_context);

//    TestRotBoxes2(render_context);
//    CornellBoxCam(render_context);
}