

#include <iostream>
#include "float.h"

#include "scene_geometry/sphere.h"
#include "scene_geometry/hitable_list.h"
#include "materials/lambertian.h"
#include "materials/metal.h"
#include "materials/dielectric.h"
#include "camera.h"

vec3 color(const ray& r, hitable *world, int depth) {
    hit_record rec;
    if (world->hit(r, 0.001f, MAXFLOAT, rec)) {
        ray scattered;
        vec3 attenuation;

        // auto target = rec.p + rec.normal + random_in_unit_sphere();
        // return 0.5f * color( ray(rec.p, target - rec.p), world);
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * color(scattered, world, depth+1);
        } else {
            return vec3(0.0f, 0.0f, 0.0f);
        }
    } else {
        auto unit_direction = unit_vector(r.direction());
        auto t = 0.5f * (unit_direction.y() + 1.0f);
        return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
    }
}

hitable *random_scene() {
    auto n = 500;
    hitable **list = new hitable*[n+1];
    list[0] = new sphere(vec3(0.0f, -1000.0f, 0.0f), 1000.0f, new lambertian(vec3(0.5f, 0.5f, 0.5f)));
    auto i = 1;
    for (int a= -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = drand48();
            auto center = vec3(a + 0.9f * drand48(), 0.2f, b + 0.9f * drand48());
            if (choose_mat < 0.8) { // diffuse
                list[i++] = new sphere(center, 0.2f, new lambertian(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())));
            } else if (choose_mat < 0.95) { // metal
                list[i++] = new sphere(center, 0.2,
                        new metal(vec3(0.5 * (1.0f + drand48()), 0.5*(1.0f + drand48()), 0.5*(1.0f + drand48())), 0.5 * drand48()));
            } else {  // glass
                list[i++] = new sphere(center, 0.2f, new dielectric(1.5f));
            }
        }
    }

    list[i++] = new sphere(vec3(0.0f, 1.0f, 0.0f), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4.0f, 1.0f, 0.0f), 1.0f, new lambertian(vec3(0.4f, 0.2f, 0.2f)));
    list[i++] = new sphere(vec3(4.0f, 1.0f, 0.0f), 1.0f, new metal(vec3(0.7f, 0.6f, 0.5f), 0.0f));

    return new hitable_list(list, i);
}


int main() {
    auto nx = 200;
    auto ny = 100;
    auto ns = 100;
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";

//    hitable *list[5];
//    list[0] = new sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f, new lambertian(vec3(0.1f, 0.2f, 0.5f)));
//    list[1] = new sphere(vec3(0.0f, -100.5f, -1.0f), 100.0f, new lambertian(vec3(0.8f, 0.8f, 0.0f)));
//    list[2] = new sphere(vec3(1.0f, 0.0f, -1.0f), 0.5f, new metal(vec3(0.8f, 0.6f, 0.2f), 0.3f));
//    list[3] = new sphere(vec3(-1.0f, 0.0f, -1.0f), 0.5f, new dielectric(1.5f));
//    list[4] = new sphere(vec3(-1.0f, 0.0f, -1.0f), -0.45f, new dielectric(1.5f));
//    float R = cos(M_PI / 4.0f);
////    list[0] = new sphere(vec3(-R, 0.0f, -1.0f), R, new lambertian(vec3(0.0f, 0.0f, 1.0f)));
////    list[1] = new sphere(vec3( R, 0.0f, -1.0f), R, new lambertian(vec3(1.0f, 0.0f, 0.0f)));
//    hitable *world = new hitable_list(list, 5);
    hitable *world = random_scene();

    auto lookfrom = vec3(12.0f, 1.5f, 4.0f);
    auto lookat = vec3(0.0f, 0.0f, -1.0f);
    auto dist_to_focus = (lookfrom - lookat).length();
    auto aperture = 0.25f;
    auto cam = camera(lookfrom, lookat, vec3(0.0f, 1.0f, 0.0f),
                      20.0f, float(nx) / float(ny), aperture, dist_to_focus);

    for (int j = ny-1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {

            auto col = vec3(0.0f, 0.0f, 0.0f);
            for (int s=0; s < ns; s++) {
                auto u = float(i + drand48()) / float(nx);
                auto v = float(j + drand48()) / float(ny);
                auto r = cam.get_ray(u, v);
                auto p = r.point_at_parameter(2.0f);
                col += color(r, world, 0);

            }
            col /= float(ns);
            // "gamma correct" - maybe in the future this will be opencolor lut?
            col = vec3(sqrtf(col[0]), sqrtf(col[1]), sqrtf(col[2]));
            auto ir = int(255.99 * col[0]);
            auto ig = int(255.99 * col[1]);
            auto ib = int(255.99 * col[2]);
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}