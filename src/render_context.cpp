//
// Created by svhwan on 5/12/2019.
//

#include "render_context.h"

RenderContext::RenderContext() : hitable_light_list() {
  render_settings = RenderSettings();
  world_ptr = nullptr;
  camera_ptr = nullptr;
  // Can't think of a meaningful cam initialization.
}
