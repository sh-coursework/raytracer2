//
// Created by Steve Hwan on 10/27/18.
//

#ifndef RAYTRACER2_SCENE_GENERATION_H
#define RAYTRACER2_SCENE_GENERATION_H


#include <memory>
#include <tuple>

#include "ray_engine/hitable.h"
#include "camera.h"
#include "render_settings.h"


std::tuple<std::unique_ptr<Hitable>, std::unique_ptr<Camera>>
GetScene(const RenderSettings &render_settings);

#endif //RAYTRACER2_SCENE_GENERATION_H
