cmake_minimum_required(VERSION 3.12)
project(raytracer2)

set(CMAKE_CXX_STANDARD 11)

find_package( Boost 1.67.0 COMPONENTS program_options REQUIRED )
include_directories(include ${Boost_INCLUDE_DIRS})

find_library(OIIO "OpenImageIO" "/usr/local/lib")

set(SOURCE_CPPS
        src/scene_geometry/sphere.cpp
        src/scene_geometry/moving_sphere.cpp
        src/scene_geometry/rect.cpp
        src/scene_geometry/flip_normals.cpp

        src/materials/constant_material.cpp
        src/materials/lambertian.cpp
        src/materials/metal.cpp
        src/materials/dielectric.cpp

        src/materials/diffuse_light.cpp

        src/textures/checker_texture.cpp
        src/textures/perlin_noise.cpp
        src/textures/noise_texture.cpp
        src/textures/image_texture.cpp

        src/ray_engine/hitable_list.cpp
        src/ray_engine/bvh_node.cpp
        src/ray_engine/aabb.cpp
        src/ray_engine/ray_engine.cpp

        src/camera.cpp
        src/render_settings.cpp
        src/scene_generation.cpp

        src/vec3.cpp
        src/main.cpp
        )

add_executable(raytracer2 ${SOURCE_CPPS})

target_link_libraries(${PROJECT_NAME}
        ${OIIO}
        ${Boost_PROGRAM_OPTIONS_LIBRARY}
        )
