//
// Created by Steve Hwan on 10/30/18.
//

#ifndef RAYTRACER2_ROTATE_H
#define RAYTRACER2_ROTATE_H

#include "scene_geometry/transforms/rotate_base.h"
using RotateY = RotateBase<0, 2, 1>;  // Primary axis is last one (1)
using RotateX = RotateBase<2, 1, 0>;  // Primary axis is last one (1)
using RotateZ = RotateBase<1, 0, 2>;  // Primary axis is last one (1)

#endif //RAYTRACER2_ROTATE_H
