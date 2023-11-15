#pragma once

#include <vector>
#include <memory>

#include "shape.h"

/**
 * \brief Class for storing multiple shapes.
 */
class ShapeContainer final : public Shape {
public:
    std::vector<std::shared_ptr<Shape>> objects;

    ShapeContainer() = default;
    explicit ShapeContainer(const std::shared_ptr<Shape>& shape) { add(shape); }

    /**
     * \brief Clears the shape container.
     */
    void clear() { objects.clear(); }

    /**
     * \brief Adds a shape to the shape container.
     * \param object the shape to add
     */
    void add(const std::shared_ptr<Shape>& object) {
        objects.push_back(object);
    }

    /**
     * \brief Checks whether a given ray is intersecting with any of the shapes in the shape container within a given interval.
     * \param ray the ray to check
     * \param interval the interval in which to check for intersections
     * \return The closest intersection on success, otherwise std::nullopt if no intersection was found
     */
    [[nodiscard]] std::optional<Intersection> isIntersecting(const Ray& ray, Interval interval) const override {
        bool hit_anything = false;
        auto closest_so_far = interval.max();
        Intersection intersection;

        for (const auto& object : objects) {
            if (const auto result = object->isIntersecting(ray, Interval {interval.min(), closest_so_far}); result.has_value()) {
                hit_anything = true;
                closest_so_far = intersection.t;
                intersection = result.value();
            }
        }

        return hit_anything ? std::optional(intersection) : std::nullopt;
    }
};
