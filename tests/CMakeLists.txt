cmake_minimum_required(VERSION 3.26)
project(raytracing_tests)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Include the source files
include_directories(${CMAKE_SOURCE_DIR}/src)

# Add a test executable
add_executable(raytracing_tests
        utils.test.cpp
)

# Link against GTest and the project libraries
if (WIN32)
    target_link_libraries(raytracing_tests gtest_main raytracing_lib)
else()
    target_link_libraries(raytracing_tests gtest_main raytracing_lib pthread)
endif()

# Register tests
enable_testing()
add_test(NAME raytracing_tests COMMAND raytracing_tests)

# Copy test_input.txt to the GTest binary directory
configure_file(${CMAKE_SOURCE_DIR}/tests/test_input.txt ${CMAKE_BINARY_DIR}/tests COPYONLY)
