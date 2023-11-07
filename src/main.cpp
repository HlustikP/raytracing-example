#include "utils/utils.h"

int main(int argc, char** argv) {
    constexpr auto default_image_width = 256;
    constexpr auto default_image_height = 256;
    const auto default_outfile = utils::fs::path("out.ppm");

    const auto result = utils::renderImage(default_image_width, default_image_height, default_outfile);
    return result;
}
