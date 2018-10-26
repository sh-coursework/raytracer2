//
// Created by Steve Hwan on 10/23/18.
//

#include <math.h>
#include <boost/range/irange.hpp>

#include "textures/perlin_noise.h"


float
PerlinNoise::Noise(const vec3& p) const
{
    auto u = p.x() - float(floor(p.x()));
    auto v = p.y() - float(floor(p.y()));
    auto w = p.z() - float(floor(p.z()));
    // More Chapter 4 - Hermite cubic smoothing
    u = u * u * (3 - 2 * u);
    v = v * v * (3 - 2 * v);
    w = w * w * (3 - 2 * w);
    // Later in Chapter 4 - smoothed version
    auto i = int(floor(p.x()));
    auto j = int(floor(p.y()));
    auto k = int(floor(p.z()));
    float c[2][2][2];
    for (auto di: {0, 1})
        for (auto dj: {0, 1})
            for (auto dk: {0, 1})
                c[di][dj][dk] = ran_float[perm_x[(i + di) & 255]
                                        ^ perm_y[(j + dj) & 255]
                                        ^ perm_z[(k + dk) & 255]];
    return trilinear_interp(c, u, v, w);
}

static float*
PerlinGenerator()
{
    auto * p = new float[256];
    for (auto i: boost::irange(0, 256))
        p[i] = drand48();
    return p;
}

void
Permute(int *p, int n)
{
    for (int i = n-1; i > 0; i--)
    {
        auto target = int(drand48() * (i + 1));
        auto tmp = p[i];
        p[i] = p[target];
        p[target] = tmp;
    }
}

static int*
PerlinGeneratePerm()
{
    int * p = new int[256];
    for (auto i: boost::irange(0, 256))
        p[i] = i;
    Permute(p, 256);
    return p;
}

float *PerlinNoise::ran_float = PerlinGenerator();
int *PerlinNoise::perm_x = PerlinGeneratePerm();
int *PerlinNoise::perm_y = PerlinGeneratePerm();
int *PerlinNoise::perm_z = PerlinGeneratePerm();

