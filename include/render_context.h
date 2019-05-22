//
// Created by svhwan on 5/12/2019.
//

#ifndef RAYTRACER2_RENDERCONTEXT_H
#define RAYTRACER2_RENDERCONTEXT_H

#include <memory>

#include "camera.h"
#include "ray_engine/hitable.h"
#include "ray_engine/hitable_list.h"
#include "render_settings.h"


class RenderContext {
public:
  RenderContext();
  RenderSettings render_settings;
  std::unique_ptr<Hitable> world_ptr;
  std::unique_ptr<HitableList> hitable_light_list_ptr;
  std::unique_ptr<Camera> camera_ptr;
  // How to represent dielectric list?


};


#endif //RAYTRACER2_RENDERCONTEXT_H
