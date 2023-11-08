#include "utils.h"

namespace utils {

using Color = raytracer::Vec3;

[[nodiscard]] Color ray_color(const raytracer::Ray& ray, const raytracer::Shape& shape) noexcept {
    const auto result = shape.isIntersecting(ray, 0, infinity);
    if (result.has_value()) {
        return 0.5 * (result.value().normal + Color(1, 1, 1));
    }

    raytracer::Vec3 unit_direction = ray.direction().unit_vector();
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

int renderImage(int image_width, double aspect_ratio, const utils::fs::path &outfile_path) {
    // Image Infos
    constexpr auto header_size = 9;
    constexpr auto bytes_per_pixel = 3;

    // Calculate the image height, and ensure that it's at least 1.
    assert(image_width > 0);
    int image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    std::string outfile;
    outfile.reserve(image_height * image_width * bytes_per_pixel + header_size);

    // Add Header
    std::stringstream header;
    header << "P3\n" << image_width << " " << image_height << "\n255\n";
    outfile.append(header.str());

    // Add World Items
    raytracer::ShapeContainer world;
    world.add(std::make_shared<raytracer::Sphere>(raytracer::Point3(0, 0, -1), 0.5));
    world.add(std::make_shared<raytracer::Sphere>(raytracer::Point3(0,-100.5,-1), 100));

    // Image
    constexpr auto focal_length = 1.0;
    constexpr auto viewport_height = 2.0;
    const auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);
    const auto camera_center = raytracer::Point3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    const auto viewport_u = raytracer::Vec3(viewport_width, 0, 0);
    const auto viewport_v = raytracer::Vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    const auto pixel_delta_u = viewport_u / image_width;
    const auto pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    const auto viewport_upper_left = camera_center - raytracer::Vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    const auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Render Image
    for (int j = 0; j < image_height; ++j) {
        std::cout << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            raytracer::Ray ray(camera_center, ray_direction);

            const auto pixel = ray_color(ray, world);

            std::stringstream pixel_chunk;
            pixel_chunk << static_cast<int>(255.999 * pixel.x()) <<
                        " " << static_cast<int>(255.999 * pixel.y()) <<
                        " " << static_cast<int>(255.999 * pixel.z()) << "\n";

            outfile.append(pixel_chunk.str());
        }
    }

    std::cout << "\rDone.                 \n";

    // Write to file
    auto const write_result = utils::writeFile(outfile_path, outfile.data(), outfile.length());
    if(!write_result) {
        std::cerr << "[ERROR] [FILE] Writing to file failed with reason " << static_cast<int>(write_result.error())
                  << std::endl;
        return 1;
    } else {
        std::cout << "[INFO] [FILE] Output written to file " << *(write_result.value()) << std::endl;
    }

    return 0;
}

} // namespace utils
