#pragma once

#include "material.h"

class Lambertian final : public Material {
public:
    explicit Lambertian(const Color& color) : albedo_(color) {}

    [[nodiscard]] std::optional<ScatterResult> scatter(Ray incoming_ray, Vec3 intersection_normal, Point3 intersection_point) const override;

private:
    Color albedo_;
};
