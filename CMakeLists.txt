cmake_minimum_required(VERSION 3.12)
project(raytracer2)

set(CMAKE_CXX_STANDARD 11)

find_package( Boost 1.67.0 REQUIRED )
include_directories(include ${Boost_INCLUDE_DIRS})

find_library(OIIO "OpenImageIO" "/usr/local/lib")

set(SOURCE_CPPS src/main.cpp
        src/ray.cpp
        src/scene_geometry/hitable.cpp
        src/scene_geometry/sphere.cpp
        src/scene_geometry/moving_sphere.cpp
        src/scene_geometry/hitable_list.cpp
        src/scene_geometry/bvh_node.cpp
        src/camera.cpp
        src/materials/material.cpp
        src/materials/lambertian.cpp
        src/materials/metal.cpp src/materials/dielectric.cpp
        src/aabb.cpp
        )
add_executable(raytracer2 ${SOURCE_CPPS})

target_link_libraries(${PROJECT_NAME} ${OIIO})
