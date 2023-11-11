#pragma once

#include <vector>
#include <memory>

#include "shape.h"

class ShapeContainer final : public Shape {
public:
    std::vector<std::shared_ptr<Shape>> objects;

    ShapeContainer() = default;
    explicit ShapeContainer(const std::shared_ptr<Shape>& shape) { add(shape); }

    void clear() { objects.clear(); }

    void add(const std::shared_ptr<Shape>& object) {
        objects.push_back(object);
    }

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
