#pragma once

#include "intersection.h"
#include "../ray/ray.h"
#include "../ray/interval.h"

class Shape {
public:
    virtual ~Shape() = default;

    virtual std::optional<Intersection> isIntersecting(const Ray& ray, Interval interval) const = 0;

    std::shared_ptr<Material> material_ {};
};
