//
// Created by Steve Hwan on 10/27/18.
//

#include "scene_generation.h"

#include <cmath>
#include <memory>
#include <vector>
#include <textures/ramp_texture.h>

#include "vec3.h"
#include "ray_engine/hitable.h"
#include "ray_engine/hitable_list.h"
#include "ray_engine/bvh_root.h"
#include "scene_geometry/sphere.h"
#include "scene_geometry/moving_sphere.h"
#include "scene_geometry/skydome_geo.h"
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
#include "textures/ramp_texture.h"
#include "textures/image_texture.h"
#include "textures/texture_list.h"
#include "materials/lambertian.h"
#include "materials/metal.h"
#include "materials/dielectric.h"
//#include "materials/constant_material.h"
#include "materials/diffuse_light.h"
#include "materials/material_list.h"


void RandomScene(RenderContext &render_context,
                 float t_min, float t_max, bool use_bvh) {
  // Hitable is an abstract base, so I wouldn't know how/what to allocate
  // up front, and there's not really a default placeholder.
  // So I want the vector of pointers, not vector of hitables.
  std::vector<std::shared_ptr<Hitable>> scene_list;
  auto material_list_ptr = std::unique_ptr<MaterialList>(new MaterialList("global"));
  auto texture_list_ptr = std::unique_ptr<TextureList>(new TextureList("global"));

  auto constant_dark_green_tex = std::make_shared<ConstantTexture>(Vec3(0.2, 0.3, 0.1));
  auto constant_bright_gray_tex = std::make_shared<ConstantTexture>(Vec3(0.9, 0.9, 0.9));
  auto checker_tex = std::make_shared<CheckerTexture>(
      constant_dark_green_tex.get(),
      constant_bright_gray_tex.get()
  );
  texture_list_ptr->push_back(constant_dark_green_tex);
  texture_list_ptr->push_back(constant_bright_gray_tex);
  texture_list_ptr->push_back(checker_tex);
  auto checker_mtl = std::make_shared<Lambertian>(checker_tex.get());
  material_list_ptr->push_back(checker_mtl);
  scene_list.push_back(std::make_shared<Sphere>(
      Vec3(0.0f, -1000.0f, 0.0f), 1000.0f, checker_mtl.get()
  ));

  auto glass_mtl = std::make_shared<Dielectric>(1.5f);
  material_list_ptr->push_back(glass_mtl);
  for (auto a: boost::irange(-10, 10)) {
    for (auto b: boost::irange(-10, 10)) {
      auto choose_mat = float(drand48());
      auto center = Vec3(a + 0.9f * drand48(), 0.2f, b + 0.9f * drand48());
      if (choose_mat < 0.8) {  // diffuse
        auto random_lambert_color_tex = std::make_shared<ConstantTexture>(
            Vec3(drand48() * drand48(),
                 drand48() * drand48(),
                 drand48() * drand48()));
        texture_list_ptr->push_back(random_lambert_color_tex);
        auto random_lambert_mtl = std::make_shared<Lambertian>(random_lambert_color_tex.get());
        material_list_ptr->push_back(random_lambert_mtl);
        scene_list.push_back(std::make_shared<MovingSphere>(
            center, center + Vec3(0, 0.5 * drand48(), 0),
            0.0, 1.0, 0.2f, random_lambert_mtl.get()
        ));
      } else if (choose_mat < 0.95) {  // metal
        auto random_metal_color_tex = std::make_shared<ConstantTexture>(
            Vec3(0.5 * (1.0f + drand48()),
                 0.5 * (1.0f + drand48()),
                 0.5 * (1.0f + drand48())));
        texture_list_ptr->push_back(random_metal_color_tex);
        auto random_metal_mtl = std::make_shared<Metal>(random_metal_color_tex.get(),
                                                        0.5 * drand48());
        material_list_ptr->push_back(random_metal_mtl);
        scene_list.push_back(std::make_shared<Sphere>(
            center, 0.2, random_metal_mtl.get()));
      } else {  // glass
        scene_list.push_back(std::make_shared<Sphere>(
            center, 0.2f, glass_mtl.get()
        ));
      }
    }
  }

  scene_list.push_back(std::make_shared<Sphere>(
      Vec3(0.0f, 1.0f, 0.0f), 1.0, glass_mtl.get()
  ));
  auto red_diffuse_color_tex = std::make_shared<ConstantTexture>(Vec3(0.4f, 0.2f, 0.2f));
  texture_list_ptr->push_back(red_diffuse_color_tex);
  auto red_diffuse_mtl = std::make_shared<Lambertian>(red_diffuse_color_tex.get());
  material_list_ptr->push_back(red_diffuse_mtl);
  scene_list.push_back(std::make_shared<Sphere>(
      Vec3(-4.0f, 1.0f, 0.0f), 1.0f, red_diffuse_mtl.get()
  ));
  auto slightly_red_metal_color_tex = std::make_shared<ConstantTexture>(Vec3(0.7f, 0.6f, 0.5f));
  texture_list_ptr->push_back(slightly_red_metal_color_tex);
  auto slightly_red_metal_mtl = std::make_shared<Metal>(
      slightly_red_metal_color_tex.get(), 0.0f);
  material_list_ptr->push_back(slightly_red_metal_mtl);
  scene_list.push_back(std::make_shared<Sphere>(
      Vec3(4.0f, 1.0f, 0.0f), 1.0f,
          slightly_red_metal_mtl.get()
  ));

  auto skydome_tex = std::make_shared<RampTexture>(
      Vec3(1.0, 1.0, 1.0), Vec3(0.5, 0.7, 1.0));
  texture_list_ptr->push_back(skydome_tex);
  auto skydome_mtl = std::make_shared<DiffuseLight>(skydome_tex.get());
  material_list_ptr->push_back(skydome_mtl);
  render_context.skydome_geo_ptr = std::unique_ptr<SkydomeGeo>(
      new SkydomeGeo(skydome_mtl.get()));

  if (use_bvh)
    render_context.world_ptr = std::unique_ptr<BVHRoot>(new BVHRoot(scene_list, t_min, t_max));
  else
    render_context.world_ptr = std::unique_ptr<HitableList>(new HitableList(scene_list));
  render_context.texture_lists_.push_back(std::move(texture_list_ptr));
  render_context.material_lists_.push_back(std::move(material_list_ptr));
}


void TwoSpheres(RenderContext &render_context) {
  std::vector<std::shared_ptr<Hitable>> scene_list;
  auto material_list_ptr = std::unique_ptr<MaterialList>(new MaterialList("global"));
  auto texture_list_ptr = std::unique_ptr<TextureList>(new TextureList("global"));

  auto constant_dark_green_tex = std::make_shared<ConstantTexture>(Vec3(0.2, 0.3, 0.1));
  auto constant_bright_gray_tex = std::make_shared<ConstantTexture>(Vec3(0.9, 0.9, 0.9));
  auto checker_tex = std::make_shared<CheckerTexture>(
      constant_dark_green_tex.get(),
      constant_bright_gray_tex.get()
  );
  texture_list_ptr->push_back(constant_dark_green_tex);
  texture_list_ptr->push_back(constant_bright_gray_tex);
  texture_list_ptr->push_back(checker_tex);
  auto checker_mtl = std::make_shared<Lambertian>(checker_tex.get());
  material_list_ptr->push_back(checker_mtl);

  scene_list.push_back(std::make_shared<Sphere>(
      Vec3(0, -10, 0), 10, checker_mtl.get()
  ));
  scene_list.push_back(std::make_shared<Sphere>(
      Vec3(0, 10, 0), 10, checker_mtl.get()
  ));

  auto skydome_tex = std::make_shared<RampTexture>(
      Vec3(1.0, 1.0, 1.0), Vec3(0.5, 0.7, 1.0));
  texture_list_ptr->push_back(skydome_tex);
  auto skydome_mtl = std::make_shared<DiffuseLight>(skydome_tex.get());
  material_list_ptr->push_back(skydome_mtl);
  render_context.skydome_geo_ptr = std::unique_ptr<SkydomeGeo>(
      new SkydomeGeo(skydome_mtl.get()));

  render_context.world_ptr = std::unique_ptr<HitableList>(
      new HitableList(scene_list));
  render_context.texture_lists_.push_back(std::move(texture_list_ptr));
  render_context.material_lists_.push_back(std::move(material_list_ptr));
}

// Using shared_ptr because this actually affects ownership.
void TwoNoiseSpheres(RenderContext &render_context,
                     std::shared_ptr<Texture> noise_textured) {
  std::vector<std::shared_ptr<Hitable>> scene_list;
  auto material_list_ptr = std::unique_ptr<MaterialList>(new MaterialList("global"));
  auto texture_list_ptr = std::unique_ptr<TextureList>(new TextureList("global"));
  texture_list_ptr->push_back(noise_textured);

  auto noise_mtl = std::make_shared<Lambertian>(noise_textured.get());
  material_list_ptr->push_back(noise_mtl);

  // For now, assume C++11, not 14 so no "make_unique"
  scene_list.push_back(std::make_shared<Sphere>(
      Vec3(0, -1000, 0), 1000, noise_mtl.get()
  ));
  scene_list.push_back(std::make_shared<Sphere>(
      Vec3(0, 2, 0), 2, noise_mtl.get()
  ));

  auto skydome_tex = std::make_shared<RampTexture>(
      Vec3(1.0, 1.0, 1.0), Vec3(0.5, 0.7, 1.0));
  texture_list_ptr->push_back(skydome_tex);
  auto skydome_mtl = std::make_shared<DiffuseLight>(skydome_tex.get());
  material_list_ptr->push_back(skydome_mtl);
  render_context.skydome_geo_ptr = std::unique_ptr<SkydomeGeo>(
      new SkydomeGeo(skydome_mtl.get()));

  render_context.world_ptr = std::unique_ptr<HitableList>(new HitableList(scene_list));
  render_context.texture_lists_.push_back(std::move(texture_list_ptr));
  render_context.material_lists_.push_back(std::move(material_list_ptr));
}

void TwoPerlinSpheres(RenderContext &render_context) {
  auto noise_textured = std::make_shared<NoiseTexture>(4.0f);
  TwoNoiseSpheres(render_context, noise_textured);
}

void TwoTurbulenceSpheres(RenderContext &render_context) {
  auto noise_textured = std::make_shared<TurbulenceTexture>(4.0f);
  TwoNoiseSpheres(render_context, noise_textured);
}

void TwoMarblelikeSpheres(RenderContext &render_context) {
  auto noise_textured = std::make_shared<MarblelikeTexture>(4.0f);
  TwoNoiseSpheres(render_context, noise_textured);
}

void ImageSphereTests(RenderContext &render_context) {
  std::vector<std::shared_ptr<Hitable>> scene_list;
  auto material_list_ptr = std::unique_ptr<MaterialList>(new MaterialList("global"));
  auto texture_list_ptr = std::unique_ptr<TextureList>(new TextureList("global"));

  std::string test_filename = "test_data/land_ocean_ice_cloud_2048.jpg";
  auto noise_textured = std::make_shared<MarblelikeTexture>(4.0f);
  auto image_textured = std::make_shared<ImageTexture>(test_filename);
  texture_list_ptr->push_back(noise_textured);
  texture_list_ptr->push_back(image_textured);
  auto noise_mtl = std::make_shared<Lambertian>(noise_textured.get());
  material_list_ptr->push_back(noise_mtl);
  auto image_lgt_mtl = std::make_shared<DiffuseLight>(image_textured.get());
  material_list_ptr->push_back(image_lgt_mtl);

  // For now, assume C++11, not 14 so no "make_unique"
  // Big sphere - let's keep the marble
  scene_list.push_back(std::make_shared<Sphere>(
      Vec3(0, -1000, 0), 1000, noise_mtl.get()
  ));
  auto light_geometry_ptr = std::make_shared<Sphere>(
      Vec3(0, 2, 0), 2, image_lgt_mtl.get());
  scene_list.push_back(light_geometry_ptr);
  render_context.hitable_light_list.push_back(light_geometry_ptr);

  render_context.world_ptr = std::unique_ptr<HitableList>(new HitableList(scene_list));
  render_context.texture_lists_.push_back(std::move(texture_list_ptr));
  render_context.material_lists_.push_back(std::move(material_list_ptr));
}

void SimpleLightTest(RenderContext &render_context) {
  std::vector<std::shared_ptr<Hitable>> scene_list;
  auto material_list_ptr = std::unique_ptr<MaterialList>(new MaterialList("global"));
  auto texture_list_ptr = std::unique_ptr<TextureList>(new TextureList("global"));

  std::string test_filename = "test_data/land_ocean_ice_cloud_2048.jpg";
  auto noise_textured = std::make_shared<MarblelikeTexture>(4.0f);
  auto noise_mtl = std::make_shared<Lambertian>(noise_textured.get());
  texture_list_ptr->push_back(noise_textured);
  material_list_ptr->push_back(noise_mtl);
  auto image_textured = std::make_shared<ImageTexture>(test_filename);
  auto image_mtl = std::make_shared<Lambertian>(image_textured.get());
  texture_list_ptr->push_back(image_textured);
  material_list_ptr->push_back(image_mtl);

  // For now, assume C++11, not 14 so no "make_unique"
  // Big sphere - let's keep the marble
  scene_list.push_back(std::make_shared<Sphere>(
      Vec3(0, -1000, 0), 1000, noise_mtl.get()
  ));
  scene_list.push_back(std::make_shared<Sphere>(
      Vec3(0, 2, 0), 2, image_mtl.get()
  ));

  auto bright_constant_white_tex = std::make_shared<ConstantTexture>(Vec3(4.0f, 4.0f, 4.0f));
  texture_list_ptr->push_back(bright_constant_white_tex);
  auto bright_constant_white_light = std::make_shared<DiffuseLight>(bright_constant_white_tex.get());
  material_list_ptr->push_back(bright_constant_white_light);
  auto light_geometry_sphere_ptr = std::make_shared<Sphere>(
      Vec3(0, 7, 0), 2, bright_constant_white_light.get()
  );
  scene_list.push_back(light_geometry_sphere_ptr);
  auto light_geometry_plane_ptr = std::make_shared<XYRect>(
      3.0f, 5.0f, 1.0f, 3.0f, -2.0f, bright_constant_white_light.get()
  );
  scene_list.push_back(light_geometry_plane_ptr);

  render_context.hitable_light_list.push_back(light_geometry_sphere_ptr);
  render_context.hitable_light_list.push_back(light_geometry_plane_ptr);

  render_context.world_ptr = std::unique_ptr<HitableList>(new HitableList(scene_list));
  render_context.texture_lists_.push_back(std::move(texture_list_ptr));
  render_context.material_lists_.push_back(std::move(material_list_ptr));
}


void CornellBox(RenderContext &render_context) {
  std::vector<std::shared_ptr<Hitable>> scene_list;
  auto material_list_ptr = std::unique_ptr<MaterialList>(new MaterialList("global"));
  auto texture_list_ptr = std::unique_ptr<TextureList>(new TextureList("global"));

  auto red_tex = std::make_shared<ConstantTexture>(Vec3(0.65f, 0.05f, 0.05f));
  auto white_tex = std::make_shared<ConstantTexture>(Vec3(0.73f, 0.73f, 0.73f));
  auto green_tex = std::make_shared<ConstantTexture>(Vec3(0.12f, 0.45f, 0.15f));
  texture_list_ptr->push_back(red_tex);
  texture_list_ptr->push_back(white_tex);
  texture_list_ptr->push_back(green_tex);

  auto red_mtl = std::make_shared<Lambertian>(red_tex.get());
  auto white_mtl = std::make_shared<Lambertian>(white_tex.get());
  auto green_mtl = std::make_shared<Lambertian>(green_tex.get());
  material_list_ptr->push_back(red_mtl);
  material_list_ptr->push_back(white_mtl);
  material_list_ptr->push_back(green_mtl);

  auto bright_white15_tex = std::make_shared<ConstantTexture>(Vec3(15.0f, 15.0f, 15.0f));
  texture_list_ptr->push_back(bright_white15_tex);
  auto light_mtl = std::make_shared<DiffuseLight>(bright_white15_tex.get());
  material_list_ptr->push_back(light_mtl);

  // light
  auto light_geometry_ptr = std::make_shared<XZRect>(213, 343, 227, 332, 554,
                                                     light_mtl.get());
  scene_list.push_back(
      std::make_shared<FlipNormals>(light_geometry_ptr)
  );
  render_context.hitable_light_list.push_back(light_geometry_ptr);

  // main room walls
  scene_list.push_back(
      std::make_shared<FlipNormals>(
          std::make_shared<YZRect>(0, 555, 0, 555, 555, green_mtl.get())
      )
  );
  scene_list.push_back(
      std::make_shared<YZRect>(0, 555, 0, 555, 0, red_mtl.get())
  );
  scene_list.push_back(
      std::make_shared<FlipNormals>(
          std::make_shared<XZRect>(0, 555, 0, 555, 555, white_mtl.get())
      )
  );
  scene_list.push_back(std::make_shared<XZRect>(
      0, 555, 0, 555, 0, white_mtl.get()
  ));
  scene_list.push_back(
      std::make_shared<FlipNormals>(
          std::make_shared<XYRect>(0, 555, 0, 555, 555, white_mtl.get())
      )
  );

  // 2 boxes
  scene_list.push_back(
      std::make_shared<Translate>(
          Vec3(130, 0, 65), std::make_shared<RotateY>(
              -18, std::make_shared<Box>(
                  Vec3(0, 0, 0), Vec3(165, 165, 165), white_mtl.get())
          )
      )
  );
  scene_list.push_back(
      std::make_shared<Translate>(
          Vec3(265, 0, 295), std::make_shared<RotateY>(
              15, std::make_shared<Box>(
                  Vec3(0, 0, 0), Vec3(165, 330, 165), white_mtl.get())
          )
      )
  );

  render_context.world_ptr = std::unique_ptr<HitableList>(new HitableList(scene_list));
  render_context.texture_lists_.push_back(std::move(texture_list_ptr));
  render_context.material_lists_.push_back(std::move(material_list_ptr));
}


void CornellBoxAluminum(RenderContext &render_context) {
  std::vector<std::shared_ptr<Hitable>> scene_list;
  auto material_list_ptr = std::unique_ptr<MaterialList>(new MaterialList("global"));
  auto texture_list_ptr = std::unique_ptr<TextureList>(new TextureList("global"));

  auto red_tex = std::make_shared<ConstantTexture>(Vec3(0.65f, 0.05f, 0.05f));
  auto white_tex = std::make_shared<ConstantTexture>(Vec3(0.73f, 0.73f, 0.73f));
  auto green_tex = std::make_shared<ConstantTexture>(Vec3(0.12f, 0.45f, 0.15f));
  auto metal_tex = std::make_shared<ConstantTexture>(Vec3(0.8f, 0.85f, 0.88f));
  texture_list_ptr->push_back(red_tex);
  texture_list_ptr->push_back(white_tex);
  texture_list_ptr->push_back(green_tex);
  texture_list_ptr->push_back(metal_tex);

  auto red_mtl = std::make_shared<Lambertian>(red_tex.get());
  auto white_mtl = std::make_shared<Lambertian>(white_tex.get());
  auto green_mtl = std::make_shared<Lambertian>(green_tex.get());
  auto alumninum_mtl = std::make_shared<Metal>(metal_tex.get(), 0.0);
  material_list_ptr->push_back(red_mtl);
  material_list_ptr->push_back(white_mtl);
  material_list_ptr->push_back(green_mtl);
  material_list_ptr->push_back(alumninum_mtl);

  auto bright_white15_tex = std::make_shared<ConstantTexture>(Vec3(15.0f, 15.0f, 15.0f));
  texture_list_ptr->push_back(bright_white15_tex);
  auto light_mtl = std::make_shared<DiffuseLight>(bright_white15_tex.get());
  material_list_ptr->push_back(light_mtl);

  // light
  auto light_geometry_ptr =
      std::make_shared<FlipNormals>(
          std::make_shared<XZRect>(213, 343, 227, 332, 554, light_mtl.get())
      );
  scene_list.push_back(light_geometry_ptr);
  render_context.hitable_light_list.push_back(light_geometry_ptr);

  // main room walls
  scene_list.push_back(
      std::make_shared<FlipNormals>(
          std::make_shared<YZRect>(0, 555, 0, 555, 555, green_mtl.get())
      )
  );
  scene_list.push_back(std::make_shared<YZRect>(0, 555, 0, 555, 0, red_mtl.get()));
  scene_list.push_back(
      std::make_shared<FlipNormals>(
          std::make_shared<XZRect>(0, 555, 0, 555, 555, white_mtl.get())
      )
  );
  scene_list.push_back(std::make_shared<XZRect>(0, 555, 0, 555, 0, white_mtl.get()));
  scene_list.push_back(
      std::make_shared<FlipNormals>(
          std::make_shared<XYRect>(0, 555, 0, 555, 555, white_mtl.get())
      )
  );

  // 2 boxes
  scene_list.push_back(
      std::make_shared<Translate>(
          Vec3(130, 0, 65), std::make_shared<RotateY>(
              -18, std::make_shared<Box>(Vec3(0, 0, 0), Vec3(165, 165, 165), white_mtl.get())
          )
      )
  );
  scene_list.push_back(
      std::make_shared<Translate>(
          Vec3(265, 0, 295), std::make_shared<RotateY>(
              15, std::make_shared<Box>(
                  Vec3(0, 0, 0), Vec3(165, 330, 165), alumninum_mtl.get())
          )
      )
  );

  render_context.world_ptr = std::unique_ptr<HitableList>(new HitableList(scene_list));
  render_context.texture_lists_.push_back(std::move(texture_list_ptr));
  render_context.material_lists_.push_back(std::move(material_list_ptr));
}


void CornellBoxGlassSphere(RenderContext &render_context) {
  std::vector<std::shared_ptr<Hitable>> scene_list;
  auto material_list_ptr = std::unique_ptr<MaterialList>(new MaterialList("global"));
  auto texture_list_ptr = std::unique_ptr<TextureList>(new TextureList("global"));

  auto red_tex = std::make_shared<ConstantTexture>(Vec3(0.65f, 0.05f, 0.05f));
  auto white_tex = std::make_shared<ConstantTexture>(Vec3(0.73f, 0.73f, 0.73f));
  auto green_tex = std::make_shared<ConstantTexture>(Vec3(0.12f, 0.45f, 0.15f));
  texture_list_ptr->push_back(red_tex);
  texture_list_ptr->push_back(white_tex);
  texture_list_ptr->push_back(green_tex);

  auto red_mtl = std::make_shared<Lambertian>(red_tex.get());
  auto white_mtl = std::make_shared<Lambertian>(white_tex.get());
  auto green_mtl = std::make_shared<Lambertian>(green_tex.get());
  auto glass_mtl = std::make_shared<Dielectric>(1.5f);
  material_list_ptr->push_back(red_mtl);
  material_list_ptr->push_back(white_mtl);
  material_list_ptr->push_back(green_mtl);
  material_list_ptr->push_back(glass_mtl);

  auto bright_white15_tex = std::make_shared<ConstantTexture>(Vec3(15.0f, 15.0f, 15.0f));
  texture_list_ptr->push_back(bright_white15_tex);
  auto light_mtl = std::make_shared<DiffuseLight>(bright_white15_tex.get());
  material_list_ptr->push_back(light_mtl);

  // light
  auto light_geometry_ptr =
      std::make_shared<XZRect>(213, 343, 227, 332, 554, light_mtl.get());
  auto flipped_light_geometry_ptr =
      std::make_shared<FlipNormals>(light_geometry_ptr);
  scene_list.push_back(flipped_light_geometry_ptr);
  render_context.hitable_light_list.push_back(flipped_light_geometry_ptr);

  // main room walls
  scene_list.push_back(
      std::make_shared<FlipNormals>(
          std::make_shared<YZRect>(0, 555, 0, 555, 555, green_mtl.get())
      )
  );
  scene_list.push_back(
      std::make_shared<YZRect>(0, 555, 0, 555, 0, red_mtl.get())
  );
  scene_list.push_back(
      std::make_shared<FlipNormals>(
          std::make_shared<XZRect>(0, 555, 0, 555, 555, white_mtl.get())
      )
  );
  scene_list.push_back(std::make_shared<XZRect>(0, 555, 0, 555, 0, white_mtl.get()));
  scene_list.push_back(
      std::make_shared<FlipNormals>(
          std::make_shared<XYRect>(0, 555, 0, 555, 555, white_mtl.get())
      )
  );

  // tall box and a glass sphere
  auto glass_geometry_ptr =
      std::make_shared<Sphere>(Vec3(190, 90, 190), 90, glass_mtl.get());
  scene_list.push_back(glass_geometry_ptr);
  // TODO: separate the dielectric from the lights.
  render_context.hitable_light_list.push_back(glass_geometry_ptr);
  scene_list.push_back(
      std::make_shared<Translate>(Vec3(265, 0, 295),
            std::make_shared<RotateY>(15,
                  std::make_shared<Box>(
                      Vec3(0, 0, 0), Vec3(165, 330, 165), white_mtl.get())
            )
      )
  );

  render_context.world_ptr = std::unique_ptr<HitableList>(new HitableList(scene_list));
  render_context.texture_lists_.push_back(std::move(texture_list_ptr));
  render_context.material_lists_.push_back(std::move(material_list_ptr));
}


void CornellSmoke(RenderContext &render_context) {
  std::vector<std::shared_ptr<Hitable>> scene_list;
  auto material_list_ptr = std::unique_ptr<MaterialList>(new MaterialList("global"));
  auto texture_list_ptr = std::unique_ptr<TextureList>(new TextureList("global"));

  auto red_tex = std::make_shared<ConstantTexture>(Vec3(0.65f, 0.05f, 0.05f));
  auto white_tex = std::make_shared<ConstantTexture>(Vec3(0.73f, 0.73f, 0.73f));
  auto green_tex = std::make_shared<ConstantTexture>(Vec3(0.12f, 0.45f, 0.15f));
  texture_list_ptr->push_back(red_tex);
  texture_list_ptr->push_back(white_tex);
  texture_list_ptr->push_back(green_tex);

  auto red_mtl = std::make_shared<Lambertian>(red_tex.get());
  auto white_mtl = std::make_shared<Lambertian>(white_tex.get());
  auto green_mtl = std::make_shared<Lambertian>(green_tex.get());
  auto glass_mtl = std::make_shared<Dielectric>(1.5f);
  material_list_ptr->push_back(red_mtl);
  material_list_ptr->push_back(white_mtl);
  material_list_ptr->push_back(green_mtl);
  material_list_ptr->push_back(glass_mtl);

  auto bright_white7_tex = std::make_shared<ConstantTexture>(Vec3(7.0f, 7.0f, 7.0f));
  texture_list_ptr->push_back(bright_white7_tex);
  auto light_mtl = std::make_shared<DiffuseLight>(bright_white7_tex.get());
  material_list_ptr->push_back(light_mtl);

  // light
  auto light_geometry_ptr = std::make_shared<XZRect>(113, 443, 127, 432, 554, light_mtl.get());
  auto flipped_light_geometry_ptr =
      std::make_shared<FlipNormals>(light_geometry_ptr);
  scene_list.push_back(flipped_light_geometry_ptr);
  render_context.hitable_light_list.push_back(flipped_light_geometry_ptr);

  // main room walls
  scene_list.push_back(
      std::make_shared<FlipNormals>(
          std::make_shared<YZRect>(0, 555, 0, 555, 555, green_mtl.get())
      )
  );
  scene_list.push_back(std::make_shared<YZRect>(0, 555, 0, 555, 0, red_mtl.get()));
  scene_list.push_back(
      std::make_shared<FlipNormals>(
          std::make_shared<XZRect>(0, 555, 0, 555, 555, white_mtl.get()))
  );
  scene_list.push_back(std::make_shared<XZRect>(0, 555, 0, 555, 0, white_mtl.get()));
  scene_list.push_back(
      std::make_shared<FlipNormals>(
          std::make_shared<XYRect>(0, 555, 0, 555, 555, white_mtl.get())
      )
  );

  // 2 boxes
  std::shared_ptr<Translate> box_1 {new Translate(
      Vec3(130, 0, 65), std::make_shared<RotateY>(
          -18, std::make_shared<Box>(
              Vec3(0, 0, 0), Vec3(165, 165, 165), white_mtl.get())
      )
  ) };
  std::shared_ptr<Translate> box_2 {new Translate(
      Vec3(265, 0, 295), std::make_shared<RotateY>(
          15, std::make_shared<Box>(
              Vec3(0, 0, 0), Vec3(165, 330, 165), white_mtl.get())
      )
  )};

  auto white1_tex = std::make_shared<ConstantTexture>(Vec3(1.0, 1.0, 1.0));
  auto black_tex = std::make_shared<ConstantTexture>(Vec3(0.0, 0.0, 0.0));
  texture_list_ptr->push_back(white1_tex);
  texture_list_ptr->push_back(black_tex);
  scene_list.push_back(std::make_shared<ConstantMedium>(box_1, 0.01, white1_tex.get()));
  scene_list.push_back(std::make_shared<ConstantMedium>(box_2, 0.01, black_tex.get()));

  render_context.world_ptr = std::unique_ptr<HitableList>(new HitableList(scene_list));
  render_context.texture_lists_.push_back(std::move(texture_list_ptr));
  render_context.material_lists_.push_back(std::move(material_list_ptr));
}


void TestAllBook2(RenderContext &render_context) {
  std::vector<std::shared_ptr<Hitable>> scene_list;
  auto material_list_ptr = std::unique_ptr<MaterialList>(new MaterialList("global"));
  auto texture_list_ptr = std::unique_ptr<TextureList>(new TextureList("global"));

  std::vector<std::shared_ptr<Hitable>> box_list_1;
  std::vector<std::shared_ptr<Hitable>> box_list_2;

  auto constant_gray_tex = std::make_shared<ConstantTexture>(Vec3(0.73f, 0.73f, 0.73f));
  texture_list_ptr->push_back(constant_gray_tex);
  auto constant_green_tex = std::make_shared<ConstantTexture>(Vec3(0.48f, 0.83f, 0.53f));;
  texture_list_ptr->push_back(constant_green_tex);
  auto white_mtl = std::make_shared<Lambertian>(constant_gray_tex.get());
  auto ground_mtl = std::make_shared<Lambertian>(constant_green_tex.get());

  material_list_ptr->push_back(white_mtl);
  material_list_ptr->push_back(ground_mtl);

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
          Vec3(x0, y0, z0), Vec3(x1, y1, z1), ground_mtl.get()
      ));
    }
  }

  scene_list.push_back(std::make_shared<BVHRoot>(box_list_1, 0, 1));

  auto bright_white7_tex = std::make_shared<ConstantTexture>(Vec3(7.0f, 7.0f, 7.0f));
  texture_list_ptr->push_back(bright_white7_tex);
  auto light_material = std::make_shared<DiffuseLight>(bright_white7_tex.get());
  material_list_ptr->push_back(light_material);

  auto light_geometry_ptr =
      std::make_shared<FlipNormals>(
          std::make_shared<XZRect>(123, 423, 147, 412, 554, light_material.get())
      );
  scene_list.push_back(light_geometry_ptr);
  render_context.hitable_light_list.push_back(light_geometry_ptr);

  auto constant_red_tex = std::make_shared<ConstantTexture>(Vec3(0.7f, 0.3f, 0.1f));
  texture_list_ptr->push_back(constant_red_tex);
  auto red_lambert_mtl = std::make_shared<Lambertian>(constant_red_tex.get());
  material_list_ptr->push_back(red_lambert_mtl);
  Vec3 center(400.0f, 400.0f, 200.0f);
  scene_list.push_back(std::make_shared<MovingSphere>(
      center, center + Vec3(30.0f, 0.0f, 0.0f),
      0, 1, 50.0,
      red_lambert_mtl.get()
  ));
  // TODO: separate the dielectric from the lights.
  auto glass_mtl = std::make_shared<Dielectric>(1.5f);
  material_list_ptr->push_back(glass_mtl);
  auto dielectric_geometry_ptr = std::make_shared<Sphere>(
      Vec3(260.0f, 150.0f, 45.0f), 50.0f, glass_mtl.get()
  );
  scene_list.push_back(dielectric_geometry_ptr);
//    render_context.hitable_light_list.push_back(dielectric_geometry_ptr);

  auto constant_blue_tex = std::make_shared<ConstantTexture>(Vec3(0.2f, 0.4f, 0.9f));
  texture_list_ptr->push_back(constant_blue_tex);
  auto blue_metal_mte = std::make_shared<Metal>(constant_blue_tex.get(), 10.0f);
  material_list_ptr->push_back(blue_metal_mte);
  scene_list.push_back(std::make_shared<Sphere>(
      Vec3(0.0f, 150.0f, 145.0f), 50.0f, blue_metal_mte.get()
  ));

  std::shared_ptr<Sphere> boundary = std::make_shared<Sphere>(
      Vec3(360.0f, 150.0f, 145.0f), 70.0f, glass_mtl.get());
  scene_list.push_back(boundary);
  scene_list.push_back(std::make_shared<ConstantMedium>(
      boundary, 0.2, constant_blue_tex.get()
  ));

  auto constant_white1_tex = std::make_shared<ConstantTexture>(Vec3(1.0f, 1.0f, 1.0f));
  texture_list_ptr->push_back(constant_white1_tex);
  boundary = std::make_shared<Sphere>(Vec3(0.0f, 0.0f, 5.0f), 5000.0f,
                                      glass_mtl.get());
  scene_list.push_back(std::make_shared<ConstantMedium>(
      boundary, 0.0001, constant_white1_tex.get()
  ));

  std::string test_filename = "test_data/land_ocean_ice_cloud_2048.jpg";
  auto earth_image_tex = std::make_shared<ImageTexture>(test_filename);
  texture_list_ptr->push_back(earth_image_tex);
  auto earth_mtl = std::make_shared<Lambertian>(earth_image_tex.get());
  material_list_ptr->push_back(earth_mtl);
  scene_list.push_back(std::make_shared<Sphere>(
      Vec3(400.0f, 200.0f, 300.0f), 80.0f, earth_mtl.get()
  ));

  auto perlin_texture = std::make_shared<NoiseTexture>(0.1);
  texture_list_ptr->push_back(perlin_texture);
  auto noisy_mtl = std::make_shared<Lambertian>(perlin_texture.get());
  material_list_ptr->push_back(noisy_mtl);
  scene_list.push_back(std::make_shared<Sphere>(
      Vec3(220.0f, 280.0f, 300.0f), 80.0f, noisy_mtl.get()
  ));

  auto ns = 1000;
  for (auto j: boost::irange(0, ns)) {
    box_list_2.push_back(std::make_shared<Sphere>(
        Vec3(165.0 * drand48(), 165.0 * drand48(), 165.0 * drand48()),
        10.0f, white_mtl.get()
    ));
  }
  scene_list.push_back(
      std::make_shared<Translate>(
          Vec3(-100, 270, 395), std::make_shared<RotateY>(
              15.0f, std::make_shared<BVHRoot>(box_list_2, 0.0, 1.0)
          )
      )
  );

  render_context.world_ptr = std::unique_ptr<HitableList>(new HitableList(scene_list));
  render_context.texture_lists_.push_back(std::move(texture_list_ptr));
  render_context.material_lists_.push_back(std::move(material_list_ptr));
}


void TestRotYBoxes(RenderContext &render_context) {
  std::vector<std::shared_ptr<Hitable>> scene_list;
  auto material_list_ptr = std::unique_ptr<MaterialList>(new MaterialList("global"));
  auto texture_list_ptr = std::unique_ptr<TextureList>(new TextureList("global"));

  std::vector<std::shared_ptr<Hitable>> box_list_1;
  std::vector<std::shared_ptr<Hitable>> box_list_2;

  auto constant_green_tex = std::make_shared<ConstantTexture>(Vec3(0.48f, 0.83f, 0.53f));
  texture_list_ptr->push_back(constant_green_tex);
  auto green_ground_mtl = std::make_shared<Lambertian>(constant_green_tex.get());
  material_list_ptr->push_back(green_ground_mtl);

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
          std::make_shared<RotateY>(
              roty, std::make_shared<Box>(
                  Vec3(x0, y0, z0), Vec3(x1, y1, z1),
                  green_ground_mtl.get()
              )));
    }
  }

  scene_list.push_back(std::make_shared<BVHRoot>(box_list_1, 0, 1));

  auto constant_white7_tex = std::make_shared<ConstantTexture>(Vec3(7.0f, 7.0f, 7.0f));
  texture_list_ptr->push_back(constant_white7_tex);
  auto light_white7_mtl = std::make_shared<DiffuseLight>(constant_white7_tex.get());
  material_list_ptr->push_back(light_white7_mtl);

  auto light_geometry_ptr =
      std::make_shared<FlipNormals>(
          std::make_shared<XZRect>(123, 423, 147, 412, 554, light_white7_mtl.get())
      );
  scene_list.push_back(light_geometry_ptr);
  render_context.hitable_light_list.push_back(light_geometry_ptr);

  std::string test_filename = "test_data/land_ocean_ice_cloud_2048.jpg";
  auto earth_image_tex = std::make_shared<ImageTexture>(test_filename);
  texture_list_ptr->push_back(earth_image_tex);
  auto earth_mat = std::make_shared<Lambertian>(earth_image_tex.get());
  material_list_ptr->push_back(earth_mat);
  scene_list.push_back(std::make_shared<Sphere>(
      Vec3(400.0f, 200.0f, 300.0f), 80.0f, earth_mat.get()
  ));

  auto perlin_texture = std::make_shared<NoiseTexture>(0.1);
  texture_list_ptr->push_back(perlin_texture);
  auto perlin_noisy_mat = std::make_shared<Lambertian>(perlin_texture.get());
  material_list_ptr->push_back(perlin_noisy_mat);
  scene_list.push_back(std::make_shared<Sphere>(
      Vec3(220.0f, 280.0f, 300.0f), 80.0f, perlin_noisy_mat.get()
  ));

  render_context.world_ptr = std::unique_ptr<HitableList>(new HitableList(scene_list));
  render_context.texture_lists_.push_back(std::move(texture_list_ptr));
  render_context.material_lists_.push_back(std::move(material_list_ptr));
}


void TestRotBoxes2(RenderContext &render_context) {
  std::vector<std::shared_ptr<Hitable>> scene_list;
  auto material_list_ptr = std::unique_ptr<MaterialList>(new MaterialList("global"));
  auto texture_list_ptr = std::unique_ptr<TextureList>(new TextureList("global"));

  std::vector<std::shared_ptr<Hitable>> box_list_1;
  std::vector<std::shared_ptr<Hitable>> box_list_2;

  auto constant_green_tex = std::make_shared<ConstantTexture>(Vec3(0.48f, 0.83f, 0.53f));
  texture_list_ptr->push_back(constant_green_tex);
  auto green_ground_mtl = std::make_shared<Lambertian>(constant_green_tex.get());
  material_list_ptr->push_back(green_ground_mtl);

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
            std::make_shared<Translate>(
                Vec3(x0, y0, z0), std::make_shared<RotateY>(
                    rot_angle, std::make_shared<Box>(
                        Vec3(-w / 4, -w / 6, -w / 4),
                        Vec3(w / 4, w / 6, w / 4), green_ground_mtl.get()
                    ))));
      } else if (axis < 0.6666666) {
        box_list_1.push_back(
            std::make_shared<Translate>(
                Vec3(x0, y0, z0), std::make_shared<RotateX>(
                    rot_angle, std::make_shared<Box>(
                        Vec3(-w / 4, -w / 6, -w / 4),
                        Vec3(w / 4, w / 6, w / 4), green_ground_mtl.get()
                    ))));

      } else {
        box_list_1.push_back(
            std::make_shared<Translate>(
                Vec3(x0, y0, z0), std::make_shared<RotateZ>(
                    rot_angle, std::make_shared<Box>(
                        Vec3(-w / 4, -w / 6, -w / 4),
                        Vec3(w / 4, w / 6, w / 4), green_ground_mtl.get()
                    ))));

      }
    }
  }

  scene_list.push_back(std::make_shared<BVHRoot>(box_list_1, 0, 1));

  auto constant_white7_tex = std::make_shared<ConstantTexture>(Vec3(7.0f, 7.0f, 7.0f));
  texture_list_ptr->push_back(constant_white7_tex);
  auto light_white7_mtl = std::make_shared<DiffuseLight>(constant_white7_tex.get());
  material_list_ptr->push_back(light_white7_mtl);

  auto light_geometry_ptr =
      std::make_shared<FlipNormals>(
          std::make_shared<XZRect>(123, 423, 147, 412, 554, light_white7_mtl.get())
      );
  scene_list.push_back(light_geometry_ptr);
  render_context.hitable_light_list.push_back(light_geometry_ptr);

  std::string test_filename = "test_data/land_ocean_ice_cloud_2048.jpg";
  auto earth_image_tex = std::make_shared<ImageTexture>(test_filename);
  texture_list_ptr->push_back(earth_image_tex);
  auto earth_mat = std::make_shared<Lambertian>(earth_image_tex.get());
  material_list_ptr->push_back(earth_mat);
  scene_list.push_back(std::make_shared<Sphere>(
      Vec3(400.0f, 200.0f, 300.0f), 80.0f, earth_mat.get()
  ));

  auto perlin_texture = std::make_shared<NoiseTexture>(0.1);
  texture_list_ptr->push_back(perlin_texture);
  auto perlin_noisy_mat = std::make_shared<Lambertian>(perlin_texture.get());
  material_list_ptr->push_back(perlin_noisy_mat);
  scene_list.push_back(std::make_shared<Sphere>(
      Vec3(220.0f, 280.0f, 300.0f), 80.0f, perlin_noisy_mat.get()
  ));

  render_context.world_ptr = std::unique_ptr<HitableList>(new HitableList(scene_list));
  render_context.texture_lists_.push_back(std::move(texture_list_ptr));
  render_context.material_lists_.push_back(std::move(material_list_ptr));
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
//  RandomScene(render_context, 0.0, 1.0, true);
//  RandomSceneCam(render_context);

//  TwoSpheres(render_context);
//  TwoSphereCam(render_context);

//  TwoMarblelikeSpheres(render_context);
//  TwoSphereCam(render_context);

//  TwoPerlinSpheres(render_context);
//  TwoSphereCam(render_context);

//  TwoTurbulenceSpheres(render_context);
//  TwoSphereCam(render_context);

//  TwoMarblelikeSpheres(render_context);
//  TwoSphereCam(render_context);

//  ImageSphereTests(render_context);
//  ImageSphereTestCam(render_context);

//  SimpleLightTest(render_context);
//  SimpleLightTestCam(render_context);

//  CornellBox(render_context);
//  CornellBoxCam(render_context);

//  CornellBoxAluminum(render_context);
//  CornellBoxCam(render_context);

//  CornellBoxGlassSphere(render_context);
//  CornellBoxCam(render_context);

//  CornellSmoke(render_context);
//  CornellBoxCam(render_context);

//  TestAllBook2(render_context);
//  CornellBoxCam(render_context);

//  TestRotYBoxes(render_context);
//  CornellBoxCam(render_context);

  TestRotBoxes2(render_context);
  CornellBoxCam(render_context);
}