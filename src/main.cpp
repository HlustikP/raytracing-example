#include <materials/dielectric.h>

#include "camera/Camera.h"
#include "materials/lambertian.h"
#include "materials/metal.h"
#include "utils/utils.h"

int main(int argc, char** argv) {
    // TODO: Get User Input

    constexpr auto default_image_width = 720;
    constexpr auto default_image_height = 480;
    const auto default_outfile = utils::fs::path("out.ppm");

    ShapeContainer world {};

    auto material_ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    auto material_left   = std::make_shared<Dielectric>(1.5);
    auto material_right  = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.0);

    world.add(std::make_shared<Sphere>(Point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(std::make_shared<Sphere>(Point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(std::make_shared<Sphere>(Point3(-1.0,    0.0, -1.0),  -0.4, material_left));
    world.add(std::make_shared<Sphere>(Point3( 1.0,    0.0, -1.0),   0.5, material_right));
    world.add(std::make_shared<Sphere>(Point3( 0.0, -100.5, -1.0), 100.0, material_ground));

    const auto now = std::chrono::high_resolution_clock::now();

    Camera camera(default_image_width, default_image_height);
    camera.move({-2, 2, 1}, {0, 0, -1}, {0, 1, 0}, 90);
    const auto renderResult = camera.renderImage(world);

    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - now).count();

    if (renderResult.has_value()) {
        std::cout << "\nRendered image in " << duration << "ms" << std::endl;

        if (const auto writeResult = utils::writeFile(default_outfile,
                                                      renderResult.value().data(), renderResult.value().size()); writeResult.has_value()) {
            std::cout << "Successfully wrote image to " << default_outfile << std::endl;
            return 0;
        }
        return 1;
    }
    return 1;
}
