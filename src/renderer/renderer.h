#pragma once
#include <camera/Camera.h>
#include <shapes/shape_container.h>

namespace fs = std::filesystem;

/**
 * \brief Class to render an image.
 */
class Renderer {
public:
    Renderer() = delete;
    Renderer(int image_width, int image_height, fs::path&& outfile);

    /**
     * \brief Renders an image.
     * \return 0 on success, 1 on failure
     */
    [[nodiscard]] int renderImage() noexcept;
    /**
     * \brief Change the size of the image to be rendered.
     * \param image_width the new width of the image
     * \param image_height the new height of the image
     */
    void resize(int image_width, int image_height) noexcept;

private:
    int image_width_;
    int image_height_;
    Camera camera_;
    ShapeContainer world_ {};
    fs::path outfile_;
};
