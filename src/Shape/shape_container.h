#pragma once

#include <vector>
#include <memory>

#include "shape.h"

namespace raytracer {

class ShapeContainer : public Shape {
public:
    std::vector<std::shared_ptr<Shape>> objects;

    ShapeContainer() = default;
    explicit ShapeContainer(const std::shared_ptr<Shape>& shape) { add(shape); }

    void clear() { objects.clear(); }

    void add(const std::shared_ptr<Shape>& object) {
        objects.push_back(object);
    }

    [[nodiscard]] std::optional<Intersection> isIntersecting(const Ray& ray, double ray_tmin, double ray_tmax) const override {
        bool hit_anything = false;
        auto closest_so_far = ray_tmax;
        Intersection intersection;

        for (const auto& object : objects) {
            const auto result = object->isIntersecting(ray, ray_tmin, closest_so_far);
            if (result.has_value()) {
                hit_anything = true;
                closest_so_far = intersection.t;
                intersection = result.value();
            }
        }

        return hit_anything ? std::optional<Intersection>(intersection) : std::nullopt;
    }
};

} // namespace raytracer
