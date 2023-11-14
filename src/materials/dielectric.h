#pragma once

#include "material.h"

class Dielectric final : public Material {
public:
    explicit Dielectric(const double refractive_index) : refractive_index_(refractive_index) {}

    [[nodiscard]] std::optional<ScatterResult> scatter(Ray incoming_ray, Intersection intersect) const override;

private:
    static double reflectance(double cosine, double reflectance_ratio);

    double refractive_index_ {};
};
