#include "sphere.h"

namespace raytracer {

std::optional<Intersection> Sphere::isIntersecting(const Ray &ray, double ray_tmin, double ray_tmax) const {
    Vec3 oc = ray.origin() - center_;
    auto a = ray.direction().length_squared();
    auto half_b = dot(oc, ray.direction());
    auto c = oc.length_squared() - radius_ * radius_;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) {
        return std::nullopt;
    }
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root <= ray_tmin || ray_tmax <= root) {
        root = (-half_b + sqrtd) / a;
        if (root <= ray_tmin || ray_tmax <= root) {
            return std::nullopt;
        }
    }

    raytracer::Intersection intersect {};
    intersect.t = root;
    intersect.p = ray.at(intersect.t);
    const auto outward_normal = (intersect.p - center_) / radius_;
    intersect.setFaceNormal(ray, outward_normal);

    return intersect;
}

} // namespace raytracer
