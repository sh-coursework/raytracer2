cmake_minimum_required(VERSION 3.12)
project(raytracer1)

set(CMAKE_CXX_STANDARD 11)

# add_executable(raytracer1 main.cpp vec3.cpp vec3.h ray.cpp ray.h hitable.cpp hitable.h sphere.cpp sphere.h hitable_list.cpp hitable_list.h)
add_executable(raytracer1 main.cpp
        vec3.h ray.h ray.cpp
        scene_geometry/hitable.h scene_geometry/sphere.h scene_geometry/hitable_list.h
        camera.h camera.cpp
        materials/material.h
        materials/material.cpp
        materials/lambertian.cpp materials/lambertian.h
        materials/metal.cpp materials/metal.h materials/dielectric.cpp materials/dielectric.h)
