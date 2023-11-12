#include "lambertian.h"

std::optional<ScatterResult> Lambertian::scatter(Ray incoming_ray, Vec3 intersection_normal, Point3 intersection_point) const {
    auto scatter_direction = intersection_normal + Vec3::generateRandomUnitVector();
    if (scatter_direction.isNearZero()) {
        // Non-workable scatter direction generated, so use the normal as a fallback.
        scatter_direction = intersection_normal;
    }

    const Ray scattered = { intersection_point, scatter_direction };

    return std::optional<ScatterResult>({ albedo_, scattered });
}
