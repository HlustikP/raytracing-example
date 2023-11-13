#include "metal.h"
#include "../shapes/intersection.h"

std::optional<ScatterResult> Metal::scatter(const Ray incoming_ray, const Intersection intersect) const {
    const Vec3 reflected = Vec3::getUnitVector(incoming_ray.direction()).reflect(intersect.normal);

    if (const Ray scattered = { intersect.p, reflected + fuzziness_ * Vec3::generateRandomUnitVector() };
            dot(scattered.direction(), intersect.normal) > 0) {

        return ScatterResult{ albedo_, scattered };
    }
    return std::nullopt;
}
