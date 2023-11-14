#pragma once
#include <camera/camera.h>
#include <shapes/shape_container.h>

namespace fs = std::filesystem;

class Renderer {
public:
    Renderer() = delete;
    Renderer(int image_width, int image_height, fs::path&& outfile);

    [[nodiscard]] int renderImage() noexcept;
    void resize(int image_width, int image_height) noexcept;

private:
    int image_width_;
    int image_height_;
    Camera camera_;
    ShapeContainer world_ {};
    fs::path outfile_;
};
