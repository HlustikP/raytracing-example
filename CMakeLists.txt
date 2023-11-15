cmake_minimum_required(VERSION 3.26)
project(raytracing)

set(CMAKE_CXX_STANDARD 23)

# library target
add_library(raytracing_lib
        src/utils/fileio.cpp
        src/vec3/vec3.cpp
        src/shapes/sphere.cpp
        src/camera/camera.cpp
        src/renderer/renderer.cpp
        src/materials/lambertian.cpp
        src/materials/metal.cpp
        src/materials/dielectric.cpp
)

target_include_directories(raytracing_lib PUBLIC ${CMAKE_SOURCE_DIR}/src)

# Main executable
add_executable(raytracing
        src/main.cpp
)

target_link_libraries(raytracing PRIVATE raytracing_lib)

# Unit Tests
add_subdirectory(tests)
