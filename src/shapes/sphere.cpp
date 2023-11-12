#include "sphere.h"

std::optional<Intersection> Sphere::isIntersecting(const Ray &ray, Interval interval) const {
    const Vec3 oc = ray.origin() - center_;
    const auto a = ray.direction().lengthSquared();
    const auto half_b = dot(oc, ray.direction());
    const auto c = oc.lengthSquared() - radius_ * radius_;

    const auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) {
        return std::nullopt;
    }
    const auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (!interval.surrounds(root)) {
        root = (-half_b + sqrtd) / a;
        if (!interval.surrounds(root)) {
            return std::nullopt;
        }
    }

    Intersection intersect {};
    intersect.t = root;
    intersect.p = ray.at(intersect.t);
    const auto outward_normal = (intersect.p - center_) / radius_;
    intersect.setFaceNormal(ray, outward_normal);

    return intersect;
}
