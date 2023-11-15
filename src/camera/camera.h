#pragma once

#include "../ray/ray.h"
#include "../vec3/vec3.h"
#include "../shapes/sphere.h"
#include "../shapes/shape.h"
#include "../shapes/shape_container.h"

using Color = Vec3;
namespace fs = std::filesystem;

/**
 * \brief Error codes for the pre-rendering phase
 */
enum class PrerenderError {
    GENERAL_ERROR,
    INVALID_IMAGE_HEIGHT,
    INVALID_IMAGE_WIDTH
};

/**
 * \brief Error codes for the rendering phase
 */
enum class RenderError {
    GENERAL_ERROR,
    INVALID_IMAGE_HEIGHT,
    INVALID_IMAGE_WIDTH
};

/**
 * \brief Camera class to handle ray casting and image generation.
 */
class Camera {
public:
    Camera() = delete;
    Camera(const int image_width, const int image_height) : image_width_(image_width), image_height_(image_height) {}

    /**
     * \brief Renders an image by shooting rays into the scene.
     * \param world the scene with it's objects to render
     * \return the rendered image as a string in the ppm format on success, an error code on failure
     */
    [[nodiscard]] std::expected<std::string, RenderError> renderImage(const ShapeContainer& world);
    /**
     * \brief Changes the size of the image to be rendered.
     * \param image_width the new width of the image
     * \param image_height the new height of the image
     */
    void resize(int image_width, int image_height) noexcept;
    /**
     * \brief Sets the number of samples per pixel.
     * \param samples_per_pixel the number of samples per pixel
     */
    void setSamplesPerPixel(const int samples_per_pixel) noexcept { samples_per_pixel_ = samples_per_pixel; }
    /**
     * \brief Sets the maximum number of ray bounces into the scene.
     * \param max_depth the maximum number of ray bounces into the scene
     */
    void setMaxDepth(const int max_depth) noexcept { max_depth_ = max_depth; }
    /**
     * \brief Moves the camera to a new position.
     * \param look_from Sets the camera position
     * \param look_at Sets the point the camera is looking at
     * \param v_up Sets the camera-relative "up" direction
     * \param fov Sets the vertical view angle (field of view)
     */
    void move(Vec3 look_from, Vec3 look_at, const Vec3&v_up, double fov) noexcept;
    /**
     * \return a random point in the camera defocus disk.
     */
    [[nodiscard]] Vec3 generateDefocusDiskSample() const;

    // Image Infos
    inline constexpr static auto header_size = 9;
    inline constexpr static auto bytes_per_pixel = 3;
private:
    /**
     * \brief Executes the pre-rednering stage
     * \return void on success, an error code on failure
     */
    std::expected<void, PrerenderError>prepareRendering() noexcept;
    /**
     * \brief Calculates the ray color resulting from the collision of a ray with an object in the scene.
     * \param ray the ray to cast into the scene
     * \param depth the current depth of the ray
     * \param shape the object the ray collided with
     */
    [[nodiscard]] static Color rayColor(const Ray& ray, int depth, const Shape& shape);
    /**
     * \brief Calculates the rendered color of a pixel and writes it to the image file.
     * \param pixel the raw, unscaled color of the pixel
     * \param outfile the image file
     */
    void calculateAndWritePixel(Color pixel, std::string& outfile) const;
    /**
     * \brief Get a randomly sampled camera ray for the pixel at location (x, y), originating from the defocused camera lens.
     * \param x the x coordinate of the pixel
     * \param y the y coordinate of the pixel
     * \return the shot ray
     */
    [[nodiscard]] Ray shootRay(int x, int y) const;
    /**
     * \return a random point in the square surrounding a pixel at the origin.
     */
    [[nodiscard]] Vec3 generatePixelSample() const;


    int image_width_;
    int image_height_;

    Point3 camera_center_ {};
    Point3 pixel00_loc_ {};    // Location of pixel [0, 0]
    Vec3   pixel_delta_u_ {};  // Offset to pixel to the right
    Vec3   pixel_delta_v_ {};  // Offset to pixel below
    int samples_per_pixel_ = 10;
    int max_depth_ = 50; // Maximum number of ray bounces into scene

    double fov_ = 90;  // Vertical view angle (field of view)
    Point3 look_from_ = Point3(0, 0, -1); // Point camera is looking from
    Point3 look_at_ = Point3(0, 0, 0); // Point camera is looking at

    Vec3 v_up_ = Vec3(0, 1, 0); // Camera-relative "up" direction
    Vec3 u_ {}; // Camera horizontal frame basis vectors
    Vec3 v_ {}; // Camera vertical frame basis vectors
    Vec3 w_ {}; // Camera look direction frame basis vectors

    double defocus_angle_ = 0.6; // Variation angle of rays through each pixel
    double focus_distance_ = 10.0; // Distance from camera look_from_ point to plane of perfect focus
    Vec3 defocus_disk_horizental_radius_ {};
    Vec3 defocus_disk_vertical_radius_ {};
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
