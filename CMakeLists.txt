cmake_minimum_required(VERSION 3.12)
project(raytracer2)

set(CMAKE_CXX_STANDARD 11)

find_package( Boost 1.67.0 COMPONENTS program_options REQUIRED )
include_directories(include ${Boost_INCLUDE_DIRS})

find_library(OIIO "OpenImageIO" "/usr/local/lib")

set(SOURCE_CPPS
        src/scene_geometry/sphere.cpp
        src/scene_geometry/moving_sphere.cpp

        src/materials/material.cpp
        src/materials/lambertian.cpp
        src/materials/metal.cpp
        src/materials/dielectric.cpp
        src/textures/checker_texture.cpp

        src/ray_engine/ray.cpp
        src/ray_engine/hitable_list.cpp
        src/ray_engine/bvh_node.cpp
        src/ray_engine/aabb.cpp
        src/ray_engine/ray_engine.cpp

        src/camera.cpp
        src/render_settings.cpp
        src/main.cpp
        )

add_executable(raytracer2 ${SOURCE_CPPS})

target_link_libraries(${PROJECT_NAME}
        ${OIIO}
        ${Boost_PROGRAM_OPTIONS_LIBRARY}
        )
