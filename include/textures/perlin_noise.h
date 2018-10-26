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

inline float trilinear_interp(float c[2][2][2], float u, float v, float w)
{
    float accum = 0.0f;
    for ( int i: {0, 1})
        for ( int j: {0, 1})
            for ( int k: {0, 1})
                accum += (i * u + (1 - i) * (1.0f - u))
                         * (j * v + (1 - j) * (1.0f - v))
                         * (k * w + (1 - k) * (1.0f - w))
                         * c[i][j][k];
    return accum;
}



#endif //RAYTRACER2_PERLIN_NOISE_H
