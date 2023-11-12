#pragma once

#include <optional>

#include "../ray/ray.h"
#include "../vec3/vec3.h"

using Color = Vec3;

struct ScatterResult {
    Color attenuation;
    Ray scattered_ray;
};

class Material {
public:
    virtual ~Material() = default;

    [[nodiscard]] virtual std::optional<ScatterResult> scatter(Ray incoming_ray, Vec3 intersection_normal, Point3 intersection_point) const = 0;
};
