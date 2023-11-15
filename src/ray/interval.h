#pragma once

#include "../utils/utils.h"

/**
 * Class representing an interval.
 */
class Interval {
public:
    Interval() : min_(+utils::infinity), max_(-utils::infinity) {} // Default interval is empty

    Interval(const double min, const double max) : min_(min), max_(max) {}

    /**
     * \param x the value to check
     * \return true if the interval contains x (inclusive), false otherwise
     */
    [[nodiscard]] bool contains(const double x) const {
        return min_ <= x && x <= max_;
    }

    /**
     * \param x the value to check
     * \return true if the interval surrounds x (exclusive), false otherwise
     */
    [[nodiscard]] bool surrounds(const double x) const {
        return min_ < x && x < max_;
    }

    /**
     * \brief Clamp a value to the interval.
     * \param x the value to clamp
     * \return x if x is in the interval, min if x < min, max if x > max
     */
    [[nodiscard]] double clamp(const double x) const {
        if (x < min_) return min_;
        if (x > max_) return max_;
        return x;
    }

    /**
     * \return the minimum value of the interval
     */
    [[nodiscard]] double min() const {
        return min_;
    }

    /**
     * \return the maximum value of the interval
     */
    [[nodiscard]] double max() const {
        return max_;
    }

private:
    double min_ {};
    double max_ {};
};

// Empty Interval
const static Interval empty   (+utils::infinity, -utils::infinity);
// Universe Interval (all real numbers are in this interval)
const static Interval universe(-utils::infinity, +utils::infinity);
