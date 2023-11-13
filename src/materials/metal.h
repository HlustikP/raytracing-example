#pragma once

#include "material.h"

class Metal final : public Material {
public:
    explicit Metal(const Color& color, double fuzziness) : albedo_(color), fuzziness_(fuzziness < 1 ? fuzziness : 1) {}

    [[nodiscard]] std::optional<ScatterResult> scatter(Ray incoming_ray, Intersection intersect) const override;

private:
    Color albedo_;
    double fuzziness_;
};

