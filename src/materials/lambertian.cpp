#include "lambertian.h"
#include "../shapes/intersection.h"

std::optional<ScatterResult> Lambertian::scatter(Ray incoming_ray, const Intersection intersect) const {
    auto scatter_direction = intersect.normal + Vec3::generateRandomUnitVector();
    if (scatter_direction.isNearZero()) {
        // Non-workable scatter direction generated, so use the normal as a fallback.
        scatter_direction = intersect.normal;
    }

    const Ray scattered = { intersect.p, scatter_direction };

    return ScatterResult{ albedo_, scattered };
}
