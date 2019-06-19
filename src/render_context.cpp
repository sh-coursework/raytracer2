//
// Created by svhwan on 5/12/2019.
//

#include "render_context.h"

RenderContext::RenderContext() : hitable_light_list(),
                                 texture_lists_(),
                                 material_lists_()
{
  render_settings = RenderSettings();
  world_ptr = nullptr;
  camera_ptr = nullptr;
  skydome_geo_ptr = nullptr;
}
