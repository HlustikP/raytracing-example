#include "utils.h"
#include "../vec3/vec3.h"
#include "../ray/ray.h"

namespace utils {

using Color = raytracer::Vec3;

[[nodiscard]] Color ray_color(const raytracer::Ray& r) {
    raytracer::Vec3 unit_direction = r.direction().unit_vector();
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0-a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

int renderImage(int image_width, int image_height, const utils::fs::path &outfile_path) {
    // Image Infos
    constexpr auto header_size = 9;
    constexpr auto bytes_per_pixel = 3;

    std::string outfile;
    outfile.reserve(image_height * image_width * bytes_per_pixel + header_size);

    // Add Header
    std::stringstream header;
    header << "P3\n" << image_width << " " << image_height << "\n255\n";
    outfile.append(header.str());

    // Render Image
    for (int j = 0; j < image_height; ++j) {
        std::cout << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto pixel = raytracer::Vec3(double(i)/(image_width-1), double(j)/(image_height-1), 0);

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
