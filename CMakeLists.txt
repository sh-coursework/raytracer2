cmake_minimum_required(VERSION 3.12)
project(raytracer2)

set(CMAKE_CXX_STANDARD 11)

find_package( Boost 1.67.0 REQUIRED )
include_directories(${Boost_INCLUDE_DIRS})

find_library(OIIO "OpenImageIO" "/usr/local/lib")

set(SOURCE_CPPS main.cpp
        vec3.h
        ray.cpp
        scene_geometry/hitable.cpp
        scene_geometry/sphere.cpp
        scene_geometry/moving_sphere.cpp
        scene_geometry/hitable_list.cpp
        scene_geometry/bvh_node.cpp
        camera.cpp
        materials/material.cpp
        materials/lambertian.cpp
        materials/metal.cpp materials/dielectric.cpp
        aabb.cpp
        )
add_executable(raytracer2 ${SOURCE_CPPS})

target_link_libraries(${PROJECT_NAME} ${OIIO})
