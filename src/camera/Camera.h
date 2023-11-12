#pragma once

#include "../ray/ray.h"
#include "../vec3/vec3.h"
#include "../shapes/sphere.h"
#include "../shapes/shape.h"
#include "../shapes/shape_container.h"

using Color = Vec3;
namespace fs = std::filesystem;

enum class PrerenderError {
    GENERAL_ERROR,
    INVALID_IMAGE_HEIGHT,
    INVALID_IMAGE_WIDTH
};

enum class RenderError {
    GENERAL_ERROR,
    INVALID_IMAGE_HEIGHT,
    INVALID_IMAGE_WIDTH
};

class Camera {
public:
    Camera() = delete;
    Camera(const int image_width, const int image_height) : image_width_(image_width), image_height_(image_height) {}

    [[nodiscard]] std::expected<std::string, RenderError> renderImage(const ShapeContainer& world);
    void resize(int image_width, int image_height) noexcept;
    void setSamplesPerPixel(const int samples_per_pixel) noexcept { samples_per_pixel_ = samples_per_pixel; }
    void setMaxDepth(const int max_depth) noexcept { max_depth_ = max_depth; }

    // Image Infos
    inline constexpr static auto header_size = 9;
    inline constexpr static auto bytes_per_pixel = 3;
private:
    std::expected<void, PrerenderError>prepareRendering() noexcept;
    [[nodiscard]] static Color rayColor(const Ray& ray, int depth, const Shape& shape);
    void calculateAndWritePixel(Color pixel, std::string& outfile) const;
    // Get a randomly sampled camera ray for the pixel at location (x, y)
    [[nodiscard]] Ray shootRay(int x, int y) const;
    // Returns a random point in the square surrounding a pixel at the origin.
    [[nodiscard]] Vec3 generatePixelSample() const;


    int image_width_;
    int image_height_;
    Point3 camera_center_ {};
    Point3 pixel00_loc_ {};    // Location of pixel [0, 0]
    Vec3   pixel_delta_u_ {};  // Offset to pixel to the right
    Vec3   pixel_delta_v_ {};  // Offset to pixel below
    int samples_per_pixel_ = 10;
    int max_depth_ = 50; // Maximum number of ray bounces into scene
};

// Overload std::cout to print RenderError
inline std::ostream& operator<<(std::ostream& os, const RenderError& error) {
    switch (error) {
        case RenderError::GENERAL_ERROR:
            os << "GENERAL_ERROR (Code 0)";
        break;
        case RenderError::INVALID_IMAGE_HEIGHT:
            os << "INVALID_IMAGE_HEIGHT (Code 1)";
        break;
        case RenderError::INVALID_IMAGE_WIDTH:
            os << "INVALID_IMAGE_WIDTH (Code 2)";
        break;
        default:
            os << "UNKNOWN_ERROR";
        break;
    }

    return os;
}

// Overload std::cout to print PrerenderError
inline std::ostream& operator<<(std::ostream& os, const PrerenderError& error) {
    switch (error) {
        case PrerenderError::GENERAL_ERROR:
            os << "GENERAL_ERROR (Code 0)";
        break;
        case PrerenderError::INVALID_IMAGE_HEIGHT:
            os << "INVALID_IMAGE_HEIGHT (Code 1)";
        break;
        case PrerenderError::INVALID_IMAGE_WIDTH:
            os << "INVALID_IMAGE_WIDTH (Code 2)";
        break;
        default:
            os << "UNKNOWN_ERROR";
        break;
    }

    return os;
}
