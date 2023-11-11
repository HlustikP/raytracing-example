#pragma once

#include "../vec3/vec3.h"
#include "../ray/ray.h"
#include "../ray/interval.h"

class Intersection {
public:
    Point3 p {};
    Vec3 normal {};
    double t {};
    bool front_face {};

    // Sets the hit record normal vector.
    void setFaceNormal(const Ray& ray, const Vec3& outward_normal) {
        // NOTE: the parameter `outward_normal` is assumed to have unit length.

        front_face = dot(ray.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Shape {
public:
    virtual ~Shape() = default;

    virtual std::optional<Intersection> isIntersecting(const Ray& ray, Interval interval) const = 0;
};
