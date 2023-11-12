#include "camera/Camera.h"
#include "utils/utils.h"

int main(int argc, char** argv) {
    constexpr auto default_image_width = 720;
    constexpr auto default_image_height = 480;
    const auto default_outfile = utils::fs::path("out.ppm");

    ShapeContainer world;
    world.add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));

    const auto now = std::chrono::high_resolution_clock::now();

    Camera camera(default_image_width, default_image_height);
    const auto renderResult = camera.renderImage(world);

    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - now).count();

    if (renderResult.has_value()) {
        std::cout << "Rendered image in " << duration << "ms" << std::endl;

        const auto writeResult = utils::writeFile(default_outfile,
            renderResult.value().data(), renderResult.value().size());
        if (writeResult.has_value()) {
            std::cout << "Successfully wrote image to " << default_outfile << std::endl;
            return 0;
        }
        return 1;
    }
    return 1;
}
