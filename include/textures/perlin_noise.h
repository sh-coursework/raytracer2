//
// Created by Steve Hwan on 10/23/18.
//

#ifndef RAYTRACER2_PERLIN_NOISE_H
#define RAYTRACER2_PERLIN_NOISE_H

#include "vec3.h"


class PerlinNoise {
public:
    float Noise(const vec3& p) const;

    static float *ran_float;
    static int *perm_x;
    static int *perm_y;
    static int *perm_z;
};

static float*
PerlinGenerator();

void
Permute(int *p, int n);

static int*
PerlinGeneratePerm();


#endif //RAYTRACER2_PERLIN_NOISE_H
