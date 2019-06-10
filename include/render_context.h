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

  // Initially, the only thing that actually uses this is
  // the hitable pdf that is really only used in lambertian.
  // Not sure if it's best to have this as a HitableList as
  // opposed to the raw vector.  But I need to think through
  // how many times that would get unnecessarily instantiated.
  HitableList hitable_light_list;

  std::unique_ptr<Camera> camera_ptr;
  // How to represent dielectric list?


};


#endif //RAYTRACER2_RENDERCONTEXT_H
