# C++23 Raytracing Example

This is a simple raytracing example based on the book [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html) by Peter Shirley et al.

It uses modern c++ features from the 17, 20 and 23 standards
such as `std::optional`, `Class template argument deduction (CTAD)`
`std::expected` and more.

## Prerequisites

- Cmake 3.26 or higher
- Ninja build system
- C++23 compatible compiler (`msvc 19.36+` or `gcc 13+`)

Refer to the [C++23 status page](https://en.cppreference.com/w/cpp/compiler_support#C.2B.2B23_features)
for a list of compilers that support C++23.

## Build

### Generate build files
Windows:
```bash
cmake --preset win-x64-release
```

Linux:
```bash
cmake --preset linux-x64-release
```

For a list of all available presets run:
```bash
cmake --list-presets
```

### Compile and link
```bash
cd ./bin/(win-x64-release|linux-x64-release)
ninja
```

### Run
```bash
./raytracing(.exe)
```
