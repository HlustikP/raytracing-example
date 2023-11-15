#pragma once

#include <optional>

#include "../ray/ray.h"
#include "../vec3/vec3.h"

class Intersection;
using Color = Vec3;

/**
 * \brief The result of a scatter operation.
 */
struct ScatterResult {
    Color attenuation;
    Ray scattered_ray;
};

/**
 * \brief Class representing the material of a shape.
 */
class Material {
public:
    virtual ~Material() = default;

    /**
     * \brief Calculates the color and scattered ray of a given input ray hitting a shape with a certain material.
     * \param incoming_ray the incoming ray
     * \param intersect the intersection between the incoming ray and the shape
     * \return the color and scattered ray on success, otherwise std::nullopt if no scatter could be calculated
     */
    [[nodiscard]] virtual std::optional<ScatterResult> scatter(Ray incoming_ray, Intersection intersect) const = 0;
};
