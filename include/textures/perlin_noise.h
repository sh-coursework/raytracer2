//
// Created by Steve Hwan on 10/23/18.
//

#ifndef RAYTRACER2_PERLIN_NOISE_H
#define RAYTRACER2_PERLIN_NOISE_H

#include "vec3.h"


class PerlinNoise {
public:
    float Noise(const vec3& p) const;
    float Turbulence(const vec3& p, int depth=7) const;

    static vec3 *random_vec3;
    static int *perm_x;
    static int *perm_y;
    static int *perm_z;
};


static vec3*
PerlinGenerate();


void
Permute(int *p, int n);


static int*
PerlinGeneratePerm();


inline float
perlin_interp(vec3 c[2][2][2], float u, float v, float w)
{
    // Chapter 4 - move Hermite cubic smoothing here
    auto uu = u * u * (3.0 - 2.0 * u);
    auto vv = v * v * (3.0 - 2.0 * v);
    auto ww = w * w * (3.0 - 2.0 * w);
    auto accum = 0.0f;

    for (int i: {0, 1})
        for (int j: {0, 1})
            for (int k: {0, 1}) {
                // weight_v is the distance vector, c is gradient
                vec3 weight_v(u - i, v - j, w - k);
                accum += (i * uu + (1 - i) * (1.0f - uu))
                       * (j * vv + (1 - j) * (1.0f - vv))
                       * (k * ww + (1 - k) * (1.0f - ww))
                       * dot(c[i][j][k], weight_v);
            }
    return accum;
}



#endif //RAYTRACER2_PERLIN_NOISE_H
