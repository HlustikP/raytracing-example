#pragma once

#include "material.h"

/**
 * \brief Class representing a dielectric material (e.g. glass).
 */
class Dielectric final : public Material {
public:
    explicit Dielectric(const double refractive_index) : refractive_index_(refractive_index) {}

    [[nodiscard]] std::optional<ScatterResult> scatter(Ray incoming_ray, Intersection intersect) const override;

private:
    /**
     * \brief Calculates the reflectance of a ray hitting a dielectric material.
     * \param cosine the cosine of the angle between the ray and the intersection normal
     * \param reflectance_ratio the ratio of the refractive indices
     * \return the reflectance
     */
    static double reflectance(double cosine, double reflectance_ratio);

    double refractive_index_ {};
};
