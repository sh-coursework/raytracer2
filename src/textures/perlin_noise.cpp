//
// Created by Steve Hwan on 10/23/18.
//

#include "textures/perlin_noise.h"

#include <cmath>
#include <boost/range/irange.hpp>


static Vec3* PerlinGenerate() {
    Vec3 * p = new Vec3[256];
    for (auto i: boost::irange(0, 256))
        p[i] = unit_vector(Vec3(-1.0 + 2.0 * drand48(),
                                -1.0 + 2.0 * drand48(),
                                -1.0 + 2.0 * drand48()
                                ));
    return p;
}

void Permute(int *p, int n) {
    for (int i = n-1; i > 0; i--)
    {
        auto target = int(drand48() * (i + 1));
        auto tmp = p[i];
        p[i] = p[target];
        p[target] = tmp;
    }
}

static int* PerlinGeneratePerm()
{
    int * p = new int[256];
    for (auto i: boost::irange(0, 256))
        p[i] = i;
    Permute(p, 256);
    return p;
}

Vec3 *PerlinNoise::random_vec3_ = PerlinGenerate();
int *PerlinNoise::perm_x_ = PerlinGeneratePerm();
int *PerlinNoise::perm_y_ = PerlinGeneratePerm();
int *PerlinNoise::perm_z_ = PerlinGeneratePerm();


float PerlinNoise::Noise(const Vec3& p) const {
    auto u = p.x() - float(floor(p.x()));
    auto v = p.y() - float(floor(p.y()));
    auto w = p.z() - float(floor(p.z()));
    // More Chapter 4 - Hermite cubic smoothing - moved to trilinearinterp
    // Later in Chapter 4 - smoothed version
    auto i = int(floor(p.x()));
    auto j = int(floor(p.y()));
    auto k = int(floor(p.z()));
    Vec3 c[2][2][2];
    for (auto di: {0, 1})
        for (auto dj: {0, 1})
            for (auto dk: {0, 1})
                c[di][dj][dk] = random_vec3_[perm_x_[(i + di) & 255]
                                            ^ perm_y_[(j + dj) & 255]
                                            ^ perm_z_[(k + dk) & 255]];
    return PerlinInterp(c, u, v, w);
}


float PerlinNoise::Turbulence(const Vec3 &p, int depth) const {
    auto accum = 0.0f;
    auto temp_p = p;
    auto weight = 1.0f;
    for (auto i: boost::irange(0, depth))
    {
        accum += weight * PerlinNoise::Noise(temp_p);
        weight *= 0.5;
        temp_p *= 2.0;
    }
    return std::abs(accum);
}



