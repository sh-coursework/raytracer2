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
    auto i = int(4.0f * p.x()) & 255;
    auto j = int(4.0f * p.y()) & 255;
    auto k = int(4.0f * p.z()) & 255;
    return ran_float[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
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

