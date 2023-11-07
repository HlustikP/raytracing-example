cmake_minimum_required(VERSION 3.26)
project(raytracing)

set(CMAKE_CXX_STANDARD 23)

add_executable(raytracing src/main.cpp
        src/utils/utils.h
        src/utils/fileio.cpp
        src/utils/render.cpp
        src/vec3/vec3.cpp
        src/vec3/vec3.h
        src/ray/ray.h)
