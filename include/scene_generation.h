//
// Created by Steve Hwan on 10/27/18.
//

#ifndef RAYTRACER2_SCENE_GENERATION_H
#define RAYTRACER2_SCENE_GENERATION_H


#include <tuple>

#include "ray_engine/hitable.h"
#include "camera.h"
#include "render_settings.h"


std::tuple<hitable *, camera>
get_scene(const RenderSettings &render_settings);

#endif //RAYTRACER2_SCENE_GENERATION_H
