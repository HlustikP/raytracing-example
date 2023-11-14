#include "camera.h"

#include <stack>

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

    for (int y = 0; y < image_height_; ++y) {
        std::cout << "\rScanlines remaining: " << (image_height_ - y) << ' ' << std::flush;
        for (int x = 0; x < image_width_; ++x) {
            Color unscaled_pixel_color(0,0,0);
            for (int sample = 0; sample < samples_per_pixel_; ++sample) {
                const auto ray = shootRay(x, y);
                unscaled_pixel_color += rayColor(ray, max_depth_, world);
            }
            calculateAndWritePixel(unscaled_pixel_color, outfile);
        }
    }

    return outfile;
}

void Camera::resize(const int image_width, const int image_height) noexcept {
    image_width_ = image_width;
    image_height_ = image_height;
}

void Camera::move(const Vec3 look_from, const Vec3 look_at, const Vec3&v_up, const double fov) noexcept {
    look_from_ = look_from;
    look_at_ = look_at;
    v_up_ = v_up;
    fov_ = fov;
}

Vec3 Camera::generateDefocusDiskSample() const {
    auto const sample = Vec3::generate_random_vec_in_unit_sphere();
    return camera_center_ + (sample[0] * defocus_disk_horizental_radius_) + (sample[1] * defocus_disk_vertical_radius_);
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
    camera_center_ = look_from_;
    const auto theta = utils::degrees_to_radians(fov_);
    const auto h = tan(theta / 2);
    const auto viewport_height = 2 * h * focus_distance_;
    const auto viewport_width = viewport_height * (static_cast<double>(image_width_) / image_height_);

    // Calculate the u_, v_, w_ unit basis vectors for the camera coordinate frame.
    w_ = Vec3::getUnitVector(look_from_ - look_at_);
    u_ = Vec3::getUnitVector(v_up_.cross(w_));
    v_ = w_.cross(u_);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    const auto viewport_u = viewport_width * u_;    // Vector across viewport horizontal edge
    const auto viewport_v = viewport_height * -v_;  // Vector down viewport vertical edge

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    pixel_delta_u_ = viewport_u / image_width_;
    pixel_delta_v_ = viewport_v / image_height_;

    // Calculate the location of the upper left pixel.
    const auto viewport_upper_left = camera_center_ - (focus_distance_ * w_) - viewport_u / 2 - viewport_v / 2;
    pixel00_loc_ = viewport_upper_left + 0.5 * (pixel_delta_u_ + pixel_delta_v_);

    // Calculate the camera defocus disk basis vectors.
    const auto defocus_radius = focus_distance_ * tan(utils::degrees_to_radians(defocus_angle_ / 2));
    defocus_disk_horizental_radius_ = u_ * defocus_radius;
    defocus_disk_vertical_radius_ = v_ * defocus_radius;

    return {};
}

Color Camera::rayColor(const Ray& ray, const int depth, const Shape& shape) {
    // If we've exceeded the ray bounce limit, no light is reflected.
    if (depth <= 0) {
        return Color { 0, 0, 0 };
    }

    // Define 0.001 as the minimum considered interval to avoid shadow acne.
    constexpr auto min_interval = 0.001;

    // Handle Collision of Ray with Shape
    if (const auto result = shape.isIntersecting(ray, Interval{ min_interval, utils::infinity }); result.has_value()) {
        const auto scatter_result = result.value().material->scatter(ray, result.value());
        if (scatter_result.has_value()) {
            return scatter_result.value().attenuation * rayColor(scatter_result.value().scattered_ray, depth - 1, shape);
        }
        return Color { 0, 0, 0 };
    }

    const Vec3 unit_direction = Vec3::getUnitVector(ray.direction());
    const auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

void Camera::calculateAndWritePixel(Color pixel, std::string& outfile) const {
    std::stringstream pixel_chunk;

    auto r = pixel.x();
    auto g = pixel.y();
    auto b = pixel.z();

    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    const auto scale = 1.0 / samples_per_pixel_;
    r = sqrt(r * scale);
    g = sqrt(g * scale);
    b = sqrt(b * scale);

    // Write the translated [0,255] value of each color component.
    static const Interval intensity(0.000, 0.999);
    pixel_chunk << static_cast<int>(256 * intensity.clamp(r)) << ' '
                << static_cast<int>(256 * intensity.clamp(g)) << ' '
                << static_cast<int>(256 * intensity.clamp(b)) << '\n';

    outfile.append(pixel_chunk.str());
}

Ray Camera::shootRay(const int x, const int y) const {
    const auto pixel_center = pixel00_loc_ + (x * pixel_delta_u_) + (y * pixel_delta_v_);
    const auto pixel_sample = pixel_center + generatePixelSample();

    auto ray_origin = (defocus_angle_ <= 0) ? camera_center_ : generateDefocusDiskSample();
    auto ray_direction = pixel_sample - ray_origin;

    return { ray_origin, ray_direction };
}

Vec3 Camera::generatePixelSample() const {
    const auto x = -0.5 + utils::getRandomNormalDouble();
    const auto y = -0.5 + utils::getRandomNormalDouble();
    return (x * pixel_delta_u_) + (y * pixel_delta_v_);
}
