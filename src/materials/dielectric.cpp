#include "dielectric.h"
#include "../shapes/intersection.h"

std::optional<ScatterResult> Dielectric::scatter(const Ray incoming_ray, const Intersection intersect) const {
    const auto attenuation = Color{ 1.0, 1.0, 1.0 };
    const double refraction_ratio = intersect.front_face ? (1.0 / refractive_index_) : refractive_index_;

    const Vec3 unit_direction = Vec3::getUnitVector(incoming_ray.direction());

    const double cos_theta = fmin(dot(-unit_direction, intersect.normal), 1.0);
    const double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

    const bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    const Vec3 direction = cannot_refract
        ? intersect.normal.reflect(unit_direction)
        : Vec3::refract(unit_direction, intersect.normal, refraction_ratio);

    auto const scattered = Ray{ intersect.p, direction };
    return ScatterResult{ attenuation, scattered };
}
