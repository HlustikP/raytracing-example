#include "metal.h"

std::optional<ScatterResult> Metal::scatter(Ray incoming_ray, Vec3 intersection_normal, Point3 intersection_point) const {
    const Vec3 reflected = incoming_ray.direction().reflect(intersection_normal);
    const Ray scattered = { intersection_point, reflected + fuzziness_ * Vec3::generateRandomUnitVector() };

    if (dot(scattered.direction(), intersection_normal) > 0) {
        return std::optional<ScatterResult>({ albedo_, scattered });
    }
    return std::nullopt;
};
