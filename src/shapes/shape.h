#pragma once

#include "intersection.h"
#include "../ray/ray.h"
#include "../ray/interval.h"

/**
 * \brief Abstract class representing a shape.
 */
class Shape {
public:
    virtual ~Shape() = 0;

    /**
     * \brief Checks whether the shape is intersecting with a given ray.
     * \param ray the ray to check
     * \param interval the interval in which to check for intersections
     * \return an Intersection on success, otherwise std::nullopt if no intersection was found
     */
    virtual std::optional<Intersection> isIntersecting(const Ray& ray, Interval interval) const = 0;

    /**
     * \brief The material of the shape.
     */
    std::shared_ptr<Material> material_ {};
};
