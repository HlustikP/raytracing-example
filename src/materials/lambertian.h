#pragma once

#include "material.h"

/**
 * \brief Class representing a lambertian material.
 */
class Lambertian final : public Material {
public:
    explicit Lambertian(const Color& color) : albedo_(color) {}

    [[nodiscard]] std::optional<ScatterResult> scatter(Ray incoming_ray, Intersection intersect) const override;

private:
    Color albedo_;
};
