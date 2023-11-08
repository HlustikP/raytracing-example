#pragma once

#include <optional>
#include "../vec3/vec3.h"
#include "../ray/ray.h"
#include "../shape/shape.h"

namespace raytracer {

class Sphere : public Shape {
public:
    Sphere() = delete;

    Sphere(const Point3 &center, double radius) : center_(center), radius_(radius) {}

    [[nodiscard]] const Point3& center() const { return center_; }
    [[nodiscard]] double radius() const { return radius_; }

    [[nodiscard]] std::optional<Intersection> isIntersecting(const Ray& ray, double ray_tmin, double ray_tmax) const override;

private:
    Point3 center_ {};
    double radius_ {};
};

} // namespace raytracer
