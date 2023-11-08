#include "utils/utils.h"

int main(int argc, char** argv) {
    constexpr auto default_image_width = 400;
    constexpr auto aspect_ratio = 16.0 / 9.0;
    const auto default_outfile = utils::fs::path("out.ppm");

    const auto result = utils::renderImage(default_image_width, aspect_ratio, default_outfile);
    return result;
}
