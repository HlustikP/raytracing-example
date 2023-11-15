#pragma once

#include <optional>

#include "../materials/material.h"
#include "../vec3/vec3.h"
#include "../ray/ray.h"
#include "../shapes/shape.h"

/**
 * \brief Class representing a spherical object in the scene.
 */
class Sphere : public Shape {
public:
    Sphere() = delete;

    Sphere(const Point3 &center, double radius, const std::shared_ptr<Material>& material) :
        center_(center), radius_(radius) { material_ = material; }

    /**
     * \return the center of the sphere
     */
    [[nodiscard]] const Point3& center() const { return center_; }
    /**
     * \return the radius of the sphere
     */
    [[nodiscard]] double radius() const { return radius_; }
    /**
     * \brief Checks whether a given ray is intersecting with the sphere within a given interval.
     * \param ray the ray to check
     * \param interval the interval in which to check for intersections
     * \return The Intersection on success, otherwise std::nullopt if no intersection was found
     */
    [[nodiscard]] std::optional<Intersection> isIntersecting(const Ray& ray, Interval interval) const override;

private:
    Point3 center_ {};
    double radius_ {};
};
