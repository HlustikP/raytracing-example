cmake_minimum_required(VERSION 3.26)
project(raytracing)

set(CMAKE_CXX_STANDARD 23)

include_directories(src)

add_executable(raytracing
        src/main.cpp
        src/utils/fileio.cpp
        src/vec3/vec3.cpp
        src/shapes/sphere.cpp
        src/camera/camera.cpp
        src/renderer/renderer.cpp
        src/materials/lambertian.cpp
        src/materials/metal.cpp
        src/materials/dielectric.cpp)
