#pragma once

#include "../utils/utils.h"

class Interval {
public:
    Interval() : min_(+utils::infinity), max_(-utils::infinity) {} // Default interval is empty

    Interval(const double min, const double max) : min_(min), max_(max) {}

    [[nodiscard]] bool contains(const double x) const {
        return min_ <= x && x <= max_;
    }

    [[nodiscard]] bool surrounds(const double x) const {
        return min_ < x && x < max_;
    }

    [[nodiscard]] double min() const {
        return min_;
    }

    [[nodiscard]] double max() const {
        return max_;
    }

private:
    double min_ {};
    double max_ {};
};

const static Interval empty   (+utils::infinity, -utils::infinity);
const static Interval universe(-utils::infinity, +utils::infinity);
