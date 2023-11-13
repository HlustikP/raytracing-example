#pragma once

#include "material.h"

class Dielectric final : public Material {
public:
    explicit Dielectric(const double refractive_index) : refractive_index_(refractive_index) {}

    [[nodiscard]] std::optional<ScatterResult> scatter(Ray incoming_ray, Intersection intersect) const override;

private:
    double refractive_index_ {};
};
