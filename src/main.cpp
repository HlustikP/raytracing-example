#include <numeric>
#include <materials/dielectric.h>

#include "camera/Camera.h"
#include "materials/lambertian.h"
#include "materials/metal.h"
#include "utils/utils.h"

int main() {
    constexpr auto default_image_width = 720;
    constexpr auto default_image_height = 480;
    const auto default_outfile = utils::fs::path("out.ppm");
    constexpr auto default_small_object_count = 20;

    ShapeContainer world {};

    utils::InputHelper<int> smallObjectCount("Enter the number of small objects to render "
        "(number will be taken to the power of 2)", default_small_object_count);
    const auto small_object_count_result = smallObjectCount.get().value_or(default_small_object_count);
    std::vector<int> small_object_range(small_object_count_result);
    // Generate range from -(X / 2) to (X / 2)
    std::iota(small_object_range.begin(), small_object_range.end(), -(small_object_count_result / 2));

    utils::InputHelper<int> render_big_balls("Render big balls? (1 = yes, 0 = no)", 1);
    const auto render_big_balls_result = render_big_balls.get().value_or(1);

    utils::InputHelper<std::string> out_file("Enter the output file path", default_outfile.string());
    const auto out_file_result = out_file.get().value_or(default_outfile.string());

    std::cout << "Rendering image with " << small_object_count_result << " small objects and "
        << (render_big_balls_result ? "big balls" : "no big balls") << " to " << out_file_result << std::endl;

    for (int a : small_object_range) {
        for (int b : small_object_range) {
            auto choose_mat = utils::getRandomNormalDouble();
            const Point3 center(a + 0.9 * utils::getRandomNormalDouble(), 0.2, b + 0.9 * utils::getRandomNormalDouble());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto const albedo = Color(utils::getRandomDouble(0, 0.5),
                        utils::getRandomDouble(0, 0.5),
                        utils::getRandomDouble(0, 0.5));
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color::generateRandomVec3();
                    auto fuzz = utils::getRandomDouble(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    if (render_big_balls_result) {
        auto material1 = std::make_shared<Dielectric>(1.5);
        world.add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

        auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
        world.add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

        auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
        world.add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));
    }

    auto ground_material = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(std::make_shared<Sphere>(Point3(0,-1000,0), 1000, ground_material));

    const auto now = std::chrono::high_resolution_clock::now();

    Camera camera(default_image_width, default_image_height);
    camera.move({13, 2, 3}, {0, 0, 0}, {0, 1, 0}, 20);
    const auto renderResult = camera.renderImage(world);

    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - now).count();

    if (renderResult.has_value()) {
        std::cout << "\nRendered image in " << duration << "ms" << std::endl;

        if (const auto writeResult = utils::writeFile(out_file_result,
                                                      renderResult.value().data(), renderResult.value().size()); writeResult.has_value()) {
            std::cout << "Successfully wrote image to " << out_file_result << std::endl;
            return 0;
        }
        return 1;
    }
    return 1;
}
