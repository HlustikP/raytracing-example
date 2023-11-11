#include "renderer.h"

#include <utility>

Renderer::Renderer(const int image_width, const int image_height, fs::path&& outfile) : image_width_{image_width},
                                                                                        image_height_{image_height},
                                                                                        camera_ { image_width, image_height },
                                                                                        outfile_ {outfile} {}

int Renderer::renderImage() noexcept {
    const auto renderResult = camera_.renderImage(world_);

    if (renderResult.has_value()) {
        const auto writeResult = utils::writeFile(outfile_,
            renderResult.value().data(), renderResult.value().size());
        if (writeResult.has_value()) {
            std::cout << "Successfully wrote image to " << writeResult.value() << std::endl;
            return 0;
        }

        std::cerr << "[ERROR] [FILE] Failed to write image with error code " << writeResult.error() << std::endl;
        return 1;
    }

    std::cerr << "[ERROR] [RENDERER] Failed to render image with error code " << renderResult.error() << std::endl;
    return 1;
}

void Renderer::resize(const int image_width, const int image_height) noexcept {
    image_height_ = image_height;
    image_width_ = image_width;
    camera_.resize(image_width, image_height);
}
