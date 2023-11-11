#include "Camera.h"

std::expected<std::string, RenderError> Camera::renderImage(const ShapeContainer& world) {
    const auto prerenderResult = prepareRendering();

    // Pass error from pre-rendering
    if (!prerenderResult.has_value()) {
        switch (prerenderResult.error()) {
            case PrerenderError::INVALID_IMAGE_HEIGHT:
                return std::unexpected(RenderError::INVALID_IMAGE_HEIGHT);
            case PrerenderError::INVALID_IMAGE_WIDTH:
                return std::unexpected(RenderError::INVALID_IMAGE_WIDTH);
            default:
                return std::unexpected(RenderError::GENERAL_ERROR);
        }
    }

    std::string outfile;
    outfile.reserve(image_height_ * image_width_ * bytes_per_pixel + header_size);

    // Add Header
    std::stringstream header;
    header << "P3\n" << image_width_ << " " << image_height_ << "\n255\n";
    outfile.append(header.str());

    for (int j = 0; j < image_height_; ++j) {
        std::cout << "\rScanlines remaining: " << (image_height_ - j) << ' ' << std::flush;
        for (int i = 0; i < image_width_; ++i) {
            auto pixel_center = pixel00_loc_ + (i * pixel_delta_u_) + (j * pixel_delta_v_);
            auto ray_direction = pixel_center - camera_center_;
            Ray ray(camera_center_, ray_direction);

            const auto pixel = rayColor(ray, world);

            std::stringstream pixel_chunk;
            pixel_chunk << static_cast<int>(255.999 * pixel.x()) <<
                        " " << static_cast<int>(255.999 * pixel.y()) <<
                        " " << static_cast<int>(255.999 * pixel.z()) << "\n";

            outfile.append(pixel_chunk.str());
        }
    }

    return outfile;
}

void Camera::resize(const int image_width, const int image_height) noexcept {
    image_width_ = image_width;
    image_height_ = image_height;
}

std::expected<void, PrerenderError> Camera::prepareRendering() noexcept {
    // Calculate the image height
    if (image_width_ < 1) {
        return std::unexpected(PrerenderError::INVALID_IMAGE_WIDTH);
    }
    if (image_height_ < 1) {
        return std::unexpected(PrerenderError::INVALID_IMAGE_HEIGHT);
    }

    // Image
    constexpr auto focal_length = 1.0;
    constexpr auto viewport_height = 2.0;
    const auto viewport_width = viewport_height * (static_cast<double>(image_width_) / image_height_);
    camera_center_ = Point3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    const auto viewport_u = Vec3(viewport_width, 0, 0);
    const auto viewport_v = Vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    pixel_delta_u_ = viewport_u / image_width_;
    pixel_delta_v_ = viewport_v / image_height_;

    // Calculate the location of the upper left pixel.
    const auto viewport_upper_left = camera_center_ - Vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    pixel00_loc_ = viewport_upper_left + 0.5 * (pixel_delta_u_ + pixel_delta_v_);

    return {};
}

Color Camera::rayColor(const Ray& ray, const Shape& shape) {
    if (const auto result = shape.isIntersecting(ray, Interval{ 0, utils::infinity }); result.has_value()) {
        return 0.5 * (result.value().normal + Color(1, 1, 1));
    }

    const Vec3 unit_direction = ray.direction().unit_vector();
    const auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}
